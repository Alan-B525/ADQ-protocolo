add_library(adq_common
    src/adq_crc16.c
    src/adq_ring_buffer.c
    src/adq_codec.c
    src/adq_time_sync.c
    src/adq_ack.c
    src/adq_link.c
    src/adq_rf_hal.c
    src/adq_fragment.c
    src/adq_stream_parser.c
    src/adq_mac_scheduler.c
)

target_include_directories(adq_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
