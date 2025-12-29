#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bybit_public.h"

int main(void)
{
    char *q = bybit_public_build_instruments_query("linear", "BTCUSDT");
    assert(q != NULL);
    const char *expected = "category=linear&symbol=BTCUSDT";
    if (strcmp(q, expected) != 0)
    {
        fprintf(stderr, "Instruments query mismatch. Got: %s\n", q);
        free(q);
        return 1;
    }
    free(q);
    return 0;
}
