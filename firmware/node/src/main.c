#include <stdio.h>

#include "adq_codec.h"
#include "adq_link.h"
#include "node_app.h"

int main(void) {
    node_ctx_t ctx;
    adq_link_params_t link;
    adq_frame_t beacon;
    adq_frame_t frame;
    uint8_t encoded[300];
    size_t encoded_len = 0;

    node_init(&ctx, 0x0101);
    adq_link_params_default_100m(&link);
    if (node_configure_link(&ctx, &link) != 0) {
        printf("node link config failed\n");
        return 1;
    }

    beacon.start = ADQ_FRAME_START;
    beacon.proto_version = ADQ_PROTO_VERSION;
    beacon.msg_type = ADQ_MSG_BEACON;
    beacon.flags = 0;
    beacon.node_id = 0;
    beacon.seq = 0;
    beacon.timestamp_us = 1000000u;
    beacon.payload[0] = ADQ_BEACON_PAYLOAD_VERSION;
    beacon.payload[1] = 0;
    beacon.payload[2] = 0xA0;
    beacon.payload[3] = 0x86;
    beacon.payload[4] = 0x01;
    beacon.payload[5] = 0x00;
    beacon.payload[6] = 0x10;
    beacon.payload[7] = 0x27;
    beacon.payload[8] = 0xE8;
    beacon.payload[9] = 0x03;
    beacon.payload[10] = ADQ_MAC_MAX_NODES;
    beacon.payload_len = ADQ_BEACON_PAYLOAD_SIZE;

    if (node_apply_beacon_frame(&ctx, 1000000u, &beacon) != 0) {
        printf("node beacon apply failed\n");
        return 1;
    }

    ctx.state = NODE_STREAMING;

    if (!node_can_tx_now(&ctx, 1005000u)) {
        printf("node not in slot\n");
        return 1;
    }

    node_prepare_data_frame(&ctx, 1005000u, &frame);

    if (adq_encode_frame(&frame, encoded, sizeof(encoded), &encoded_len) != 0) {
        printf("encode failed\n");
        return 1;
    }

    printf("node link=%s range=%um bytes=%zu seq=%u\n",
           adq_rf_stack_name(ctx.link_params.stack),
           ctx.link_params.target_range_m,
           encoded_len,
           frame.seq);
    return 0;
}
