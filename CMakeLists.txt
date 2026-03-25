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
endif()
