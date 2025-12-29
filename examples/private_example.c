#include <stdio.h>
#include <stdlib.h>

#include "bybit_private.h"

int main(void)
{
    const char *base_url = "https://api.bybit.com";
    const char *api_key = "REPLACE_WITH_API_KEY";
    const char *api_secret = "REPLACE_WITH_API_SECRET";

    BybitClient *client = bybit_client_create(base_url, api_key, api_secret);
    if (!client)
    {
        fprintf(stderr, "Failed to create client\n");
        return 1;
    }

    char *resp = NULL;
    long status = 0;
    int rc = bybit_place_order(client,
                               "linear",
                               "BTCUSDT",
                               "Buy",
                               "Limit",
                               "0.001",
                               "30000",
                               "GTC",
                               "example-link-id",
                               "0",
                               &resp,
                               &status);
    if (rc == 0)
    {
        printf("Order status=%ld body=%s\n", status, resp ? resp : "");
        free(resp);
    }
    else
    {
        fprintf(stderr, "Failed to place order\n");
    }

    bybit_client_free(client);
    return 0;
}
