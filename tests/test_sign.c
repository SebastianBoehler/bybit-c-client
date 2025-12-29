#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bybit_client.h"

int main(void)
{
    const char *secret = "test_secret";
    const char *payload = "1700000000000test_api_key5000symbol=BTCUSDT";
    const char *expected = "09e864a4fc40ccaecc47d014e08ca37d9c133c356317c99ef28c7219e589bd41";

    char *sig = bybit_sign(secret, payload);
    assert(sig != NULL);
    if (strcmp(sig, expected) != 0)
    {
        fprintf(stderr, "Signature mismatch. Got %s\\n", sig);
        free(sig);
        return 1;
    }
    free(sig);
    return 0;
}
