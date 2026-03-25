#include <stdio.h>

#include "adq_codec.h"
#include "base_app.h"

int main(void) {
    adq_frame_t tx;
    adq_frame_t rx;
    base_stats_t stats;
    uint8_t bytes[300];
    size_t len = 0;

    tx.start = ADQ_FRAME_START;
    tx.proto_version = ADQ_PROTO_VERSION;
    tx.msg_type = ADQ_MSG_HEARTBEAT;
    tx.flags = 0;
    tx.node_id = 0x0101;
    tx.seq = 1;
    tx.timestamp_us = 123456789u;
    tx.payload_len = 0;

    if (adq_encode_frame(&tx, bytes, sizeof(bytes), &len) != 0) {
        printf("base encode test failed\n");
        return 1;
    }

    base_init(&stats);
    if (base_process_bytes(&stats, bytes, len, &rx) != 0) {
        printf("base decode failed\n");
        return 1;
    }

    printf("base rx_ok=%u msg_type=0x%02X\n", stats.rx_ok, rx.msg_type);
    return 0;
}
