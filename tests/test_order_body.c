#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bybit_private.h"

static void test_minimal_body(void)
{
    char *body = bybit_format_order_body("linear", "BTCUSDT", "Buy", "Limit", "0.01", NULL, NULL, NULL, NULL);
    assert(body != NULL);
    const char *expected = "{\"category\":\"linear\",\"symbol\":\"BTCUSDT\",\"side\":\"Buy\",\"orderType\":\"Limit\",\"qty\":\"0.01\"}";
    if (strcmp(body, expected) != 0)
    {
        fprintf(stderr, "Minimal body mismatch. Got: %s\n", body);
        free(body);
        exit(1);
    }
    free(body);
}

static void test_full_body(void)
{
    char *body = bybit_format_order_body("linear", "BTCUSDT", "Sell", "Limit", "0.5",
                                         "45000", "GTC", "link123", "1");
    assert(body != NULL);
    const char *expected = "{\"category\":\"linear\",\"symbol\":\"BTCUSDT\",\"side\":\"Sell\",\"orderType\":\"Limit\",\"qty\":\"0.5\",\"price\":\"45000\",\"timeInForce\":\"GTC\",\"orderLinkId\":\"link123\",\"positionIdx\":\"1\"}";
    if (strcmp(body, expected) != 0)
    {
        fprintf(stderr, "Full body mismatch. Got: %s\n", body);
        free(body);
        exit(1);
    }
    free(body);
}

static void test_cancel_body(void)
{
    char *body = bybit_format_cancel_order_body("linear", "BTCUSDT", "123", NULL, "1");
    assert(body != NULL);
    const char *expected = "{\"category\":\"linear\",\"symbol\":\"BTCUSDT\",\"orderId\":\"123\",\"positionIdx\":\"1\"}";
    if (strcmp(body, expected) != 0)
    {
        fprintf(stderr, "Cancel body mismatch. Got: %s\n", body);
        free(body);
        exit(1);
    }
    free(body);
}

int main(void)
{
    test_minimal_body();
    test_full_body();
    test_cancel_body();
    return 0;
}
