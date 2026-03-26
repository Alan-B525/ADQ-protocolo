cmake_minimum_required(VERSION 3.16)
project(adq_protocolo C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(ADQ_BUILD_TESTS "Build unit tests" ON)

add_subdirectory(firmware/common)
add_subdirectory(firmware/node)
add_subdirectory(firmware/base)

if(ADQ_BUILD_TESTS)
    enable_testing()
    add_executable(test_crc16 tests/unit/test_crc16.c)
    target_link_libraries(test_crc16 PRIVATE adq_common)
    add_test(NAME crc16 COMMAND test_crc16)

    add_executable(test_link_params tests/unit/test_link_params.c)
    target_link_libraries(test_link_params PRIVATE adq_common)
    add_test(NAME link_params COMMAND test_link_params)

    add_executable(test_rf_hal tests/unit/test_rf_hal.c)
    target_link_libraries(test_rf_hal PRIVATE adq_common)
    add_test(NAME rf_hal COMMAND test_rf_hal)

    add_executable(test_fragment tests/unit/test_fragment.c)
    target_link_libraries(test_fragment PRIVATE adq_common)
    add_test(NAME fragment COMMAND test_fragment)

    add_executable(test_stream_parser tests/unit/test_stream_parser.c)
    target_link_libraries(test_stream_parser PRIVATE adq_common)
    add_test(NAME stream_parser COMMAND test_stream_parser)

    add_executable(test_ack tests/unit/test_ack.c)
    target_link_libraries(test_ack PRIVATE adq_common)
    add_test(NAME ack COMMAND test_ack)

    add_executable(test_mac_scheduler tests/unit/test_mac_scheduler.c)
    target_link_libraries(test_mac_scheduler PRIVATE adq_common)
    add_test(NAME mac_scheduler COMMAND test_mac_scheduler)

    add_executable(test_node_slot
        tests/unit/test_node_slot.c
        firmware/node/src/node_app.c
    )
    target_include_directories(test_node_slot PRIVATE firmware/node/include)
    target_link_libraries(test_node_slot PRIVATE adq_common)
    add_test(NAME node_slot COMMAND test_node_slot)

    add_executable(test_node_txn
        tests/unit/test_node_txn.c
        firmware/node/src/node_app.c
    )
    target_include_directories(test_node_txn PRIVATE firmware/node/include)
    target_link_libraries(test_node_txn PRIVATE adq_common)
    add_test(NAME node_txn COMMAND test_node_txn)
endif()
