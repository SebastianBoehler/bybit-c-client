#ifndef BYBIT_PRIVATE_H
#define BYBIT_PRIVATE_H

#include "bybit_client.h"

char *bybit_format_order_body(const char *category,
                              const char *symbol,
                              const char *side,
                              const char *order_type,
                              const char *qty,
                              const char *price,
                              const char *time_in_force,
                              const char *order_link_id,
                              const char *position_idx);

char *bybit_format_cancel_order_body(const char *category,
                                     const char *symbol,
                                     const char *order_id,
                                     const char *order_link_id,
                                     const char *position_idx);

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
                      long *out_status_code);

int bybit_cancel_order(BybitClient *client,
                       const char *category,
                       const char *symbol,
                       const char *order_id,
                       const char *order_link_id,
                       const char *position_idx,
                       char **out_response,
                       long *out_status_code);

int bybit_get_open_orders(BybitClient *client,
                          const char *category,
                          const char *symbol,
                          char **out_response,
                          long *out_status_code);

int bybit_get_positions(BybitClient *client,
                        const char *category,
                        const char *symbol,
                        char **out_response,
                        long *out_status_code);

int bybit_get_wallet_balance(BybitClient *client,
                             const char *account_type,
                             const char *coin,
                             char **out_response,
                             long *out_status_code);

#endif /* BYBIT_PRIVATE_H */
