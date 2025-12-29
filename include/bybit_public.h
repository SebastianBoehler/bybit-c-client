#ifndef BYBIT_PUBLIC_H
#define BYBIT_PUBLIC_H

#include "bybit_client.h"

char *bybit_public_build_ticker_query(const char *category, const char *symbol);
char *bybit_public_build_instruments_query(const char *category, const char *symbol);

int bybit_get_server_time(BybitClient *client,
                          char **out_response,
                          long *out_status_code);

int bybit_get_ticker(BybitClient *client,
                     const char *category,
                     const char *symbol,
                     char **out_response,
                     long *out_status_code);

int bybit_get_instruments_info(BybitClient *client,
                               const char *category,
                               const char *symbol,
                               char **out_response,
                               long *out_status_code);

char *bybit_public_build_orderbook_query(const char *category,
                                         const char *symbol,
                                         const char *limit);

int bybit_get_orderbook(BybitClient *client,
                        const char *category,
                        const char *symbol,
                        const char *limit,
                        char **out_response,
                        long *out_status_code);

char *bybit_public_build_kline_query(const char *category,
                                     const char *symbol,
                                     const char *interval,
                                     const char *limit,
                                     const char *start,
                                     const char *end);

int bybit_get_kline(BybitClient *client,
                    const char *category,
                    const char *symbol,
                    const char *interval,
                    const char *limit,
                    const char *start,
                    const char *end,
                    char **out_response,
                    long *out_status_code);

char *bybit_public_build_recent_trades_query(const char *category,
                                             const char *symbol,
                                             const char *limit);

int bybit_get_recent_trades(BybitClient *client,
                            const char *category,
                            const char *symbol,
                            const char *limit,
                            char **out_response,
                            long *out_status_code);

char *bybit_public_build_funding_query(const char *category,
                                       const char *symbol,
                                       const char *limit);

int bybit_get_funding_history(BybitClient *client,
                              const char *category,
                              const char *symbol,
                              const char *limit,
                              char **out_response,
                              long *out_status_code);

char *bybit_public_build_open_interest_query(const char *category,
                                             const char *symbol,
                                             const char *interval_time);

int bybit_get_open_interest(BybitClient *client,
                            const char *category,
                            const char *symbol,
                            const char *interval_time,
                            char **out_response,
                            long *out_status_code);

#endif /* BYBIT_PUBLIC_H */
