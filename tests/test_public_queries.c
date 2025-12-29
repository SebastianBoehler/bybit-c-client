#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bybit_public.h"

static void test_orderbook_query(void)
{
    char *q = bybit_public_build_orderbook_query("linear", "BTCUSDT", "50");
    assert(q != NULL);
    const char *expected = "category=linear&symbol=BTCUSDT&limit=50";
    if (strcmp(q, expected) != 0)
    {
        fprintf(stderr, "Orderbook query mismatch. Got: %s\n", q);
        free(q);
        exit(1);
    }
    free(q);
}

static void test_kline_query(void)
{
    char *q = bybit_public_build_kline_query("linear", "BTCUSDT", "1", "200", "123", "456");
    assert(q != NULL);
    const char *expected = "category=linear&symbol=BTCUSDT&interval=1&limit=200&start=123&end=456";
    if (strcmp(q, expected) != 0)
    {
        fprintf(stderr, "Kline query mismatch. Got: %s\n", q);
        free(q);
        exit(1);
    }
    free(q);
}

static void test_recent_trades_query(void)
{
    char *q = bybit_public_build_recent_trades_query("linear", "BTCUSDT", "100");
    assert(q != NULL);
    const char *expected = "category=linear&symbol=BTCUSDT&limit=100";
    if (strcmp(q, expected) != 0)
    {
        fprintf(stderr, "Recent trades query mismatch. Got: %s\n", q);
        free(q);
        exit(1);
    }
    free(q);
}

static void test_funding_query(void)
{
    char *q = bybit_public_build_funding_query("linear", "BTCUSDT", "50");
    assert(q != NULL);
    const char *expected = "category=linear&symbol=BTCUSDT&limit=50";
    if (strcmp(q, expected) != 0)
    {
        fprintf(stderr, "Funding query mismatch. Got: %s\n", q);
        free(q);
        exit(1);
    }
    free(q);
}

static void test_open_interest_query(void)
{
    char *q = bybit_public_build_open_interest_query("linear", "BTCUSDT", "5m");
    assert(q != NULL);
    const char *expected = "category=linear&symbol=BTCUSDT&intervalTime=5m";
    if (strcmp(q, expected) != 0)
    {
        fprintf(stderr, "Open interest query mismatch. Got: %s\n", q);
        free(q);
        exit(1);
    }
    free(q);
}

int main(void)
{
    test_orderbook_query();
    test_kline_query();
    test_recent_trades_query();
    test_funding_query();
    test_open_interest_query();
    return 0;
}
