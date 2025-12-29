# Bybit C Client

Pure-C Bybit REST client (C11) with public and private endpoint helpers, built on libcurl + OpenSSL.

## Features

- Core request signer (HMAC SHA-256) and HTTP wrapper.
- Public endpoints: server time, tickers, instruments info, orderbook, klines.
- Private endpoints: place order, cancel order, open orders, positions, wallet balance.
- Examples: public and private usage.
- Unit tests for signing, payload builders, and query builders.

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run tests

```bash
ctest --test-dir build
```

## Examples

```bash
./build/example_public
./build/example_private   # set API key/secret in examples/private_example.c
```

## API helpers

- Public: `bybit_get_server_time`, `bybit_get_ticker`, `bybit_get_instruments_info`, `bybit_get_orderbook`, `bybit_get_kline`.
- Private: `bybit_place_order`, `bybit_cancel_order`, `bybit_get_open_orders`, `bybit_get_positions`, `bybit_get_wallet_balance`.

## Usage (snippet)

```c
BybitClient *c = bybit_client_create("https://api.bybit.com", api_key, api_secret);
char *resp = NULL; long status = 0;
bybit_get_ticker(c, "linear", "BTCUSDT", &resp, &status);
printf("%ld %s\n", status, resp);
free(resp);
bybit_client_free(c);
```

## Dependencies

- libcurl
- OpenSSL (crypto)

On macOS (Homebrew): `brew install openssl curl`.
