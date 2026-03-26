#include <stdio.h>

#include "adq_codec.h"
#include "adq_link.h"
#include "base_app.h"

int main(void) {
    adq_frame_t tx;
    adq_frame_t rx;
    adq_frame_t beacon;
    adq_link_params_t link;
    base_stats_t stats;
    uint8_t assigned_slot = 0;
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
    adq_link_params_default_100m(&link);
    if (base_set_link_params(&stats, &link) != 0) {
        printf("base link config failed\n");
        return 1;
    }

    if (base_register_node(&stats, 0x0101, 123456789u, &assigned_slot) != 0) {
        printf("base register node failed\n");
        return 1;
    }

    if (base_build_beacon_for_node(&stats, 0x0101, 123456789u, &beacon) != 0) {
        printf("base build beacon failed\n");
        return 1;
    }

    if (base_process_bytes(&stats, bytes, len, &rx) != 0) {
        printf("base decode failed\n");
        return 1;
    }

    printf("base link=%s range=%um slot=%u beacon_seq=%u rx_ok=%u msg_type=0x%02X\n",
           adq_rf_stack_name(stats.link_params.stack),
           stats.link_params.target_range_m,
           assigned_slot,
           beacon.seq,
           stats.rx_ok,
           rx.msg_type);
    return 0;
}
