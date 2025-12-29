#ifndef BYBIT_CLIENT_H
#define BYBIT_CLIENT_H

#include <stddef.h>

typedef struct BybitClient BybitClient;

BybitClient *bybit_client_create(const char *base_url,
                                 const char *api_key,
                                 const char *api_secret);

void bybit_client_free(BybitClient *client);

int bybit_client_request(BybitClient *client,
                         const char *method,
                         const char *path,
                         const char *query,
                         const char *body,
                         long recv_window_ms,
                         char **out_response,
                         long *out_status_code);

char *bybit_sign(const char *secret, const char *payload);

#endif /* BYBIT_CLIENT_H */
