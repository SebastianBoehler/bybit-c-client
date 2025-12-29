#include "bybit_client.h"

#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

struct BybitClient
{
    char *base_url;
    char *api_key;
    char *api_secret;
};

struct MemoryBuffer
{
    char *data;
    size_t size;
};

static char *dup_string(const char *src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *copy = (char *)malloc(len + 1);
    if (copy)
    {
        memcpy(copy, src, len);
        copy[len] = '\0';
    }
    return copy;
}

BybitClient *bybit_client_create(const char *base_url,
                                 const char *api_key,
                                 const char *api_secret)
{
    if (!base_url || !api_key || !api_secret)
        return NULL;
    BybitClient *client = (BybitClient *)calloc(1, sizeof(BybitClient));
    if (!client)
        return NULL;
    client->base_url = dup_string(base_url);
    client->api_key = dup_string(api_key);
    client->api_secret = dup_string(api_secret);
    if (!client->base_url || !client->api_key || !client->api_secret)
    {
        bybit_client_free(client);
        return NULL;
    }
    curl_global_init(CURL_GLOBAL_DEFAULT);
    return client;
}

void bybit_client_free(BybitClient *client)
{
    if (!client)
        return;
    free(client->base_url);
    free(client->api_key);
    free(client->api_secret);
    free(client);
    curl_global_cleanup();
}

char *bybit_sign(const char *secret, const char *payload)
{
    if (!secret || !payload)
        return NULL;
    unsigned char *digest = HMAC(EVP_sha256(),
                                 secret, (int)strlen(secret),
                                 (const unsigned char *)payload, strlen(payload),
                                 NULL, NULL);
    if (!digest)
        return NULL;
    char *hex = (char *)malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (!hex)
        return NULL;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        sprintf(hex + (i * 2), "%02x", digest[i]);
    }
    hex[SHA256_DIGEST_LENGTH * 2] = '\0';
    return hex;
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryBuffer *mem = (struct MemoryBuffer *)userp;
    char *ptr = (char *)realloc(mem->data, mem->size + realsize + 1);
    if (!ptr)
        return 0;
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    return realsize;
}

static long current_timestamp_ms(void)
{
    struct timespec ts;
#if defined(_WIN32)
    timespec_get(&ts, TIME_UTC);
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
    return (long)(ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL);
}

int bybit_client_request(BybitClient *client,
                         const char *method,
                         const char *path,
                         const char *query,
                         const char *body,
                         long recv_window_ms,
                         char **out_response,
                         long *out_status_code)
{
    if (!client || !method || !path)
        return -1;
    if (out_response)
        *out_response = NULL;
    if (out_status_code)
        *out_status_code = 0;

    long timestamp = current_timestamp_ms();

    char timestamp_buf[32];
    snprintf(timestamp_buf, sizeof(timestamp_buf), "%ld", timestamp);

    char recv_buf[32];
    snprintf(recv_buf, sizeof(recv_buf), "%ld", recv_window_ms > 0 ? recv_window_ms : 5000);

    const char *body_payload = body ? body : "";
    size_t payload_len = strlen(timestamp_buf) + strlen(client->api_key) + strlen(recv_buf) + strlen(body_payload) + 1;
    char *payload = (char *)malloc(payload_len);
    if (!payload)
        return -1;
    snprintf(payload, payload_len, "%s%s%s%s", timestamp_buf, client->api_key, recv_buf, body_payload);

    char *signature = bybit_sign(client->api_secret, payload);
    free(payload);
    if (!signature)
        return -1;

    size_t url_len = strlen(client->base_url) + strlen(path) + (query ? strlen(query) + 1 : 0) + 1;
    char *url = (char *)malloc(url_len);
    if (!url)
    {
        free(signature);
        return -1;
    }
    if (query)
    {
        snprintf(url, url_len, "%s%s?%s", client->base_url, path, query);
    }
    else
    {
        snprintf(url, url_len, "%s%s", client->base_url, path);
    }

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        free(signature);
        free(url);
        return -1;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    char sign_header[128];
    snprintf(sign_header, sizeof(sign_header), "X-BAPI-SIGN=%s", signature);
    headers = curl_slist_append(headers, sign_header);

    char key_header[128];
    snprintf(key_header, sizeof(key_header), "X-BAPI-API-KEY=%s", client->api_key);
    headers = curl_slist_append(headers, key_header);

    char ts_header[128];
    snprintf(ts_header, sizeof(ts_header), "X-BAPI-TIMESTAMP=%s", timestamp_buf);
    headers = curl_slist_append(headers, ts_header);

    char recv_header[128];
    snprintf(recv_header, sizeof(recv_header), "X-BAPI-RECV-WINDOW=%s", recv_buf);
    headers = curl_slist_append(headers, recv_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    struct MemoryBuffer chunk = {0};
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    if (strcmp(method, "POST") == 0)
    {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        if (body)
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        }
    }
    else if (strcmp(method, "GET") != 0)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
        if (body)
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        }
    }

    CURLcode res = curl_easy_perform(curl);
    long status = 0;
    if (res == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
    }

    if (out_response && chunk.data)
    {
        *out_response = chunk.data;
    }
    else
    {
        free(chunk.data);
    }

    if (out_status_code)
    {
        *out_status_code = status;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(signature);
    free(url);

    return res == CURLE_OK ? 0 : -1;
}
