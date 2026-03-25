#include "node_app.h"

#include <string.h>

void node_init(node_ctx_t* ctx, uint16_t node_id) {
    ctx->node_id = node_id;
    ctx->seq = 0;
    ctx->state = NODE_BOOT;
    adq_time_sync_init(&ctx->time_sync);
}

void node_on_beacon(node_ctx_t* ctx, uint64_t local_us, uint64_t beacon_us) {
    adq_time_sync_update(&ctx->time_sync, local_us, beacon_us);
    if (ctx->state < NODE_SYNCED) {
        ctx->state = NODE_SYNCED;
    }
}

void node_prepare_data_frame(node_ctx_t* ctx, uint64_t local_us, adq_frame_t* frame) {
    const uint16_t sample_rate_hz = 100;
    const uint16_t channel_mask = 0x0001;
    const uint16_t sample_count = 1;
    const uint16_t battery_mv = 3700;
    const int8_t rssi_dbm = -75;
    const uint8_t status_bits = 0;
    const int16_t sample_value = 123;

    memset(frame, 0, sizeof(*frame));
    frame->start = ADQ_FRAME_START;
    frame->proto_version = ADQ_PROTO_VERSION;
    frame->msg_type = ADQ_MSG_DATA_FRAME;
    frame->flags = 0;
    frame->node_id = ctx->node_id;
    frame->seq = ctx->seq++;
    frame->timestamp_us = adq_time_sync_to_network(&ctx->time_sync, local_us);

    frame->payload[0] = (uint8_t)(sample_rate_hz & 0xFFu);
    frame->payload[1] = (uint8_t)((sample_rate_hz >> 8) & 0xFFu);
    frame->payload[2] = (uint8_t)(channel_mask & 0xFFu);
    frame->payload[3] = (uint8_t)((channel_mask >> 8) & 0xFFu);
    frame->payload[4] = (uint8_t)(sample_count & 0xFFu);
    frame->payload[5] = (uint8_t)((sample_count >> 8) & 0xFFu);
    frame->payload[6] = (uint8_t)(battery_mv & 0xFFu);
    frame->payload[7] = (uint8_t)((battery_mv >> 8) & 0xFFu);
    frame->payload[8] = (uint8_t)rssi_dbm;
    frame->payload[9] = status_bits;
    frame->payload[10] = (uint8_t)(sample_value & 0xFFu);
    frame->payload[11] = (uint8_t)((sample_value >> 8) & 0xFFu);
    frame->payload_len = 12;
}
