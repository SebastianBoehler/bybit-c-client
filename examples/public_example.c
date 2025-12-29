#include <stdio.h>
#include <stdlib.h>

#include "bybit_public.h"

int main(void)
{
    const char *base_url = "https://api.bybit.com";
    BybitClient *client = bybit_client_create(base_url, "", "");
    if (!client)
    {
        fprintf(stderr, "Failed to create client\n");
        return 1;
    }

    char *resp = NULL;
    long status = 0;
    if (bybit_get_server_time(client, &resp, &status) == 0)
    {
        printf("Server time status=%ld body=%s\n", status, resp ? resp : "");
        free(resp);
    }
    else
    {
        fprintf(stderr, "Failed to get server time\n");
    }

    resp = NULL;
    status = 0;
    if (bybit_get_instruments_info(client, "linear", "BTCUSDT", &resp, &status) == 0)
    {
        printf("Instruments info status=%ld body=%s\n", status, resp ? resp : "");
        free(resp);
    }
    else
    {
        fprintf(stderr, "Failed to get instruments info\n");
    }

    bybit_client_free(client);
    return 0;
}
