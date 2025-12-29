#include "bybit_private.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *bybit_format_order_body(const char *category,
                              const char *symbol,
                              const char *side,
                              const char *order_type,
                              const char *qty,
                              const char *price,
                              const char *time_in_force,
                              const char *order_link_id,
                              const char *position_idx)
{
    if (!category || !symbol || !side || !order_type || !qty)
        return NULL;

    const char *tmpl = "{"
                       "\"category\":\"%s\","
                       "\"symbol\":\"%s\","
                       "\"side\":\"%s\","
                       "\"orderType\":\"%s\","
                       "\"qty\":\"%s\"";

    size_t base_len = snprintf(NULL, 0, tmpl, category, symbol, side, order_type, qty);
    size_t opt_len = 0;
    if (price)
        opt_len += strlen(",\"price\":\"\"") + strlen(price);
    if (time_in_force)
        opt_len += strlen(",\"timeInForce\":\"\"") + strlen(time_in_force);
    if (order_link_id)
        opt_len += strlen(",\"orderLinkId\":\"\"") + strlen(order_link_id);
    if (position_idx)
        opt_len += strlen(",\"positionIdx\":\"\"") + strlen(position_idx);

    size_t total = base_len + opt_len + 2; /* closing brace and null */
    char *buf = (char *)malloc(total);
    if (!buf)
        return NULL;

    int written = snprintf(buf, total, tmpl, category, symbol, side, order_type, qty);
    size_t offset = (written > 0) ? (size_t)written : 0;

    if (price)
    {
        offset += snprintf(buf + offset, total - offset, ",\"price\":\"%s\"", price);
    }
    if (time_in_force)
    {
        offset += snprintf(buf + offset, total - offset, ",\"timeInForce\":\"%s\"", time_in_force);
    }
    if (order_link_id)
    {
        offset += snprintf(buf + offset, total - offset, ",\"orderLinkId\":\"%s\"", order_link_id);
    }
    if (position_idx)
    {
        offset += snprintf(buf + offset, total - offset, ",\"positionIdx\":\"%s\"", position_idx);
    }

    snprintf(buf + offset, total - offset, "}");
    return buf;
}

char *bybit_format_cancel_order_body(const char *category,
                                     const char *symbol,
                                     const char *order_id,
                                     const char *order_link_id,
                                     const char *position_idx)
{
    if (!category || !symbol || (!order_id && !order_link_id))
        return NULL;

    const char *tmpl = "{"
                       "\"category\":\"%s\","
                       "\"symbol\":\"%s\"";
    size_t base_len = snprintf(NULL, 0, tmpl, category, symbol);
    size_t opt_len = 0;
    if (order_id)
        opt_len += strlen(",\"orderId\":\"\"") + strlen(order_id);
    if (order_link_id)
        opt_len += strlen(",\"orderLinkId\":\"\"") + strlen(order_link_id);
    if (position_idx)
        opt_len += strlen(",\"positionIdx\":\"\"") + strlen(position_idx);

    size_t total = base_len + opt_len + 2;
    char *buf = (char *)malloc(total);
    if (!buf)
        return NULL;

    int written = snprintf(buf, total, tmpl, category, symbol);
    size_t offset = written > 0 ? (size_t)written : 0;
    if (order_id)
        offset += snprintf(buf + offset, total - offset, ",\"orderId\":\"%s\"", order_id);
    if (order_link_id)
        offset += snprintf(buf + offset, total - offset, ",\"orderLinkId\":\"%s\"", order_link_id);
    if (position_idx)
        offset += snprintf(buf + offset, total - offset, ",\"positionIdx\":\"%s\"", position_idx);
    snprintf(buf + offset, total - offset, "}");
    return buf;
}

int bybit_place_order(BybitClient *client,
                      const char *category,
                      const char *symbol,
                      const char *side,
                      const char *order_type,
                      const char *qty,
                      const char *price,
                      const char *time_in_force,
                      const char *order_link_id,
                      const char *position_idx,
                      char **out_response,
                      long *out_status_code)
{
    char *body = bybit_format_order_body(category, symbol, side, order_type, qty,
                                         price, time_in_force, order_link_id, position_idx);
    if (!body)
        return -1;
    int rc = bybit_client_request(client,
                                  "POST",
                                  "/v5/order/create",
                                  NULL,
                                  body,
                                  5000,
                                  out_response,
                                  out_status_code);
    free(body);
    return rc;
}

int bybit_cancel_order(BybitClient *client,
                       const char *category,
                       const char *symbol,
                       const char *order_id,
                       const char *order_link_id,
                       const char *position_idx,
                       char **out_response,
                       long *out_status_code)
{
    char *body = bybit_format_cancel_order_body(category, symbol, order_id, order_link_id, position_idx);
    if (!body)
        return -1;
    int rc = bybit_client_request(client,
                                  "POST",
                                  "/v5/order/cancel",
                                  NULL,
                                  body,
                                  5000,
                                  out_response,
                                  out_status_code);
    free(body);
    return rc;
}

static char *build_category_symbol_query(const char *category, const char *symbol)
{
    if (!category)
        return NULL;
    size_t len = strlen("category=") + strlen(category) + 1;
    if (symbol)
        len += strlen("&symbol=") + strlen(symbol);
    char *buf = (char *)malloc(len);
    if (!buf)
        return NULL;
    if (symbol)
        snprintf(buf, len, "category=%s&symbol=%s", category, symbol);
    else
        snprintf(buf, len, "category=%s", category);
    return buf;
}

int bybit_get_open_orders(BybitClient *client,
                          const char *category,
                          const char *symbol,
                          char **out_response,
                          long *out_status_code)
{
    char *query = build_category_symbol_query(category, symbol);
    if (!query)
        return -1;
    int rc = bybit_client_request(client,
                                  "GET",
                                  "/v5/order/realtime",
                                  query,
                                  NULL,
                                  5000,
                                  out_response,
                                  out_status_code);
    free(query);
    return rc;
}

int bybit_get_positions(BybitClient *client,
                        const char *category,
                        const char *symbol,
                        char **out_response,
                        long *out_status_code)
{
    char *query = build_category_symbol_query(category, symbol);
    if (!query)
        return -1;
    int rc = bybit_client_request(client,
                                  "GET",
                                  "/v5/position/list",
                                  query,
                                  NULL,
                                  5000,
                                  out_response,
                                  out_status_code);
    free(query);
    return rc;
}

int bybit_get_wallet_balance(BybitClient *client,
                             const char *account_type,
                             const char *coin,
                             char **out_response,
                             long *out_status_code)
{
    if (!account_type)
        return -1;
    size_t len = strlen("accountType=") + strlen(account_type) + 1;
    if (coin)
        len += strlen("&coin=") + strlen(coin);
    char *query = (char *)malloc(len);
    if (!query)
        return -1;
    if (coin)
        snprintf(query, len, "accountType=%s&coin=%s", account_type, coin);
    else
        snprintf(query, len, "accountType=%s", account_type);

    int rc = bybit_client_request(client,
                                  "GET",
                                  "/v5/account/wallet-balance",
                                  query,
                                  NULL,
                                  5000,
                                  out_response,
                                  out_status_code);
    free(query);
    return rc;
}
