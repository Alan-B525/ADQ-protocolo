add_library(adq_common
    src/adq_crc16.c
    src/adq_ring_buffer.c
    src/adq_codec.c
    src/adq_time_sync.c
)

target_include_directories(adq_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
