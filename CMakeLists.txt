cmake_minimum_required(VERSION 3.15)
project(bybit_c_client C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(bybit_c_client STATIC
    src/bybit_client.c
    src/bybit_public.c
    src/bybit_private.c
)

target_link_libraries(bybit_c_client
    CURL::libcurl
    OpenSSL::Crypto
)

if(BUILD_TESTS)
    enable_testing()
    add_executable(test_sign tests/test_sign.c)
    target_link_libraries(test_sign bybit_c_client)
    add_test(NAME sign_test COMMAND test_sign)

    add_executable(test_order_body tests/test_order_body.c)
    target_link_libraries(test_order_body bybit_c_client)
    add_test(NAME order_body_test COMMAND test_order_body)

    add_executable(test_instruments_query tests/test_instruments_query.c)
    target_link_libraries(test_instruments_query bybit_c_client)
    add_test(NAME instruments_query_test COMMAND test_instruments_query)

    add_executable(test_public_queries tests/test_public_queries.c)
    target_link_libraries(test_public_queries bybit_c_client)
    add_test(NAME public_queries_test COMMAND test_public_queries)
endif()

if(BUILD_EXAMPLES)
    add_executable(example_public examples/public_example.c)
    target_link_libraries(example_public bybit_c_client)

    add_executable(example_private examples/private_example.c)
    target_link_libraries(example_private bybit_c_client)
endif()
