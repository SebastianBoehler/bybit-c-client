#include "bybit_public.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char *build_two_param_query(const char *category, const char *symbol)
{
    if (!category || !symbol)
        return NULL;
    const char *fmt = "category=%s&symbol=%s";
    size_t len = strlen(fmt) - 4 + strlen(category) + strlen(symbol) + 1; /* -4 for two %s */
    char *buf = (char *)malloc(len);
    if (!buf)
        return NULL;
    snprintf(buf, len, fmt, category, symbol);
    return buf;
}

char *bybit_public_build_ticker_query(const char *category, const char *symbol)
{
    return build_two_param_query(category, symbol);
}

char *bybit_public_build_instruments_query(const char *category, const char *symbol)
{
    return build_two_param_query(category, symbol);
}

char *bybit_public_build_orderbook_query(const char *category,
                                         const char *symbol,
                                         const char *limit)
{
    if (!category || !symbol)
        return NULL;
    size_t len = strlen("category=") + strlen(category) + strlen("&symbol=") + strlen(symbol) + 1;
    if (limit)
    {
        len += strlen("&limit=") + strlen(limit);
    }
    char *buf = (char *)malloc(len);
    if (!buf)
        return NULL;
    if (limit)
    {
        snprintf(buf, len, "category=%s&symbol=%s&limit=%s", category, symbol, limit);
    }
    else
    {
        snprintf(buf, len, "category=%s&symbol=%s", category, symbol);
    }
    return buf;
}

char *bybit_public_build_kline_query(const char *category,
                                     const char *symbol,
                                     const char *interval,
                                     const char *limit,
                                     const char *start,
                                     const char *end)
{
    if (!category || !symbol || !interval)
        return NULL;
    size_t len = strlen("category=") + strlen(category) +
                 strlen("&symbol=") + strlen(symbol) +
                 strlen("&interval=") + strlen(interval) + 1;
    if (limit)
        len += strlen("&limit=") + strlen(limit);
    if (start)
        len += strlen("&start=") + strlen(start);
    if (end)
        len += strlen("&end=") + strlen(end);

    char *buf = (char *)malloc(len);
    if (!buf)
        return NULL;

    int written = snprintf(buf, len, "category=%s&symbol=%s&interval=%s", category, symbol, interval);
    size_t offset = written > 0 ? (size_t)written : 0;
    if (limit)
        offset += snprintf(buf + offset, len - offset, "&limit=%s", limit);
    if (start)
        offset += snprintf(buf + offset, len - offset, "&start=%s", start);
    if (end)
        snprintf(buf + offset, len - offset, "&end=%s", end);
    return buf;
}

char *bybit_public_build_recent_trades_query(const char *category,
                                             const char *symbol,
                                             const char *limit)
{
    if (!category || !symbol)
        return NULL;
    size_t len = strlen("category=") + strlen(category) +
                 strlen("&symbol=") + strlen(symbol) + 1;
    if (limit)
        len += strlen("&limit=") + strlen(limit);
    char *buf = (char *)malloc(len);
    if (!buf)
        return NULL;
    int written = snprintf(buf, len, "category=%s&symbol=%s", category, symbol);
    size_t offset = written > 0 ? (size_t)written : 0;
    if (limit)
        snprintf(buf + offset, len - offset, "&limit=%s", limit);
    return buf;
}

char *bybit_public_build_funding_query(const char *category,
                                       const char *symbol,
                                       const char *limit)
{
    if (!category || !symbol)
        return NULL;
    size_t len = strlen("category=") + strlen(category) +
                 strlen("&symbol=") + strlen(symbol) + 1;
    if (limit)
        len += strlen("&limit=") + strlen(limit);
    char *buf = (char *)malloc(len);
    if (!buf)
        return NULL;
    int written = snprintf(buf, len, "category=%s&symbol=%s", category, symbol);
    size_t offset = written > 0 ? (size_t)written : 0;
    if (limit)
        snprintf(buf + offset, len - offset, "&limit=%s", limit);
    return buf;
}

char *bybit_public_build_open_interest_query(const char *category,
                                             const char *symbol,
                                             const char *interval_time)
{
    if (!category || !symbol || !interval_time)
        return NULL;
    size_t len = strlen("category=") + strlen(category) +
                 strlen("&symbol=") + strlen(symbol) +
                 strlen("&intervalTime=") + strlen(interval_time) + 1;
    char *buf = (char *)malloc(len);
    if (!buf)
        return NULL;
    snprintf(buf, len, "category=%s&symbol=%s&intervalTime=%s", category, symbol, interval_time);
    return buf;
}

int bybit_get_server_time(BybitClient *client,
                          char **out_response,
                          long *out_status_code)
{
    return bybit_client_request(client,
                                "GET",
                                "/v5/market/time",
                                NULL,
                                NULL,
                                0,
                                out_response,
                                out_status_code);
}

int bybit_get_ticker(BybitClient *client,
                     const char *category,
                     const char *symbol,
                     char **out_response,
                     long *out_status_code)
{
    char *query = bybit_public_build_ticker_query(category, symbol);
    if (!query)
        return -1;
    int rc = bybit_client_request(client,
                                  "GET",
                                  "/v5/market/tickers",
                                  query,
                                  NULL,
                                  0,
                                  out_response,
                                  out_status_code);
    free(query);
    return rc;
}

int bybit_get_orderbook(BybitClient *client,
                        const char *category,
                        const char *symbol,
                        const char *limit,
                        char **out_response,
                        long *out_status_code)
{
    char *query = bybit_public_build_orderbook_query(category, symbol, limit);
    if (!query)
        return -1;
    int rc = bybit_client_request(client,
                                  "GET",
                                  "/v5/market/orderbook",
                                  query,
                                  NULL,
                                  0,
                                  out_response,
                                  out_status_code);
    free(query);
    return rc;
}

int bybit_get_kline(BybitClient *client,
                    const char *category,
                    const char *symbol,
                    const char *interval,
                    const char *limit,
                    const char *start,
                    const char *end,
                    char **out_response,
                    long *out_status_code)
{
    char *query = bybit_public_build_kline_query(category, symbol, interval, limit, start, end);
    if (!query)
        return -1;
    int rc = bybit_client_request(client,
                                  "GET",
                                  "/v5/market/kline",
                                  query,
                                  NULL,
                                  0,
                                  out_response,
                                  out_status_code);
    free(query);
    return rc;
}

int bybit_get_instruments_info(BybitClient *client,
                               const char *category,
                               const char *symbol,
                               char **out_response,
                               long *out_status_code)
{
    char *query = bybit_public_build_instruments_query(category, symbol);
    if (!query)
        return -1;
    int rc = bybit_client_request(client,
                                  "GET",
                                  "/v5/market/instruments-info",
                                  query,
                                  NULL,
                                  0,
                                  out_response,
                                  out_status_code);
    free(query);
    return rc;
}
