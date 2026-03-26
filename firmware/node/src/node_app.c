#include "node_app.h"

#include <string.h>

void node_init(node_ctx_t* ctx, uint16_t node_id) {
    ctx->node_id = node_id;
    ctx->seq = 0;
    ctx->retry_count = 0;
    ctx->assigned_slot = 0;
    ctx->slot_valid = 0;
    ctx->state = NODE_BOOT;
    ctx->beacon_interval_us = 0;
    ctx->slot_us = 0;
    ctx->guard_us = 0;
    ctx->last_beacon_local_us = 0;
    ctx->last_beacon_network_us = 0;
    adq_link_params_default_100m(&ctx->link_params);
    adq_txn_manager_init(&ctx->txn,
                         (uint32_t)ctx->link_params.ack_timeout_ms * 1000u,
                         ctx->link_params.max_retries);
    adq_time_sync_init(&ctx->time_sync);
}

int node_configure_link(node_ctx_t* ctx, const adq_link_params_t* params) {
    if (ctx == 0 || params == 0) {
        return -1;
    }

    if (adq_link_params_validate(params) != 0) {
        return -2;
    }

    ctx->link_params = *params;
    adq_txn_manager_init(&ctx->txn,
                         (uint32_t)ctx->link_params.ack_timeout_ms * 1000u,
                         ctx->link_params.max_retries);
    ctx->retry_count = 0;
    return 0;
}

void node_on_beacon(node_ctx_t* ctx, uint64_t local_us, uint64_t beacon_us) {
    adq_time_sync_update(&ctx->time_sync, local_us, beacon_us);
    ctx->last_beacon_local_us = local_us;
    ctx->last_beacon_network_us = beacon_us;
    if (ctx->state < NODE_SYNCED) {
        ctx->state = NODE_SYNCED;
    }
}

int node_apply_beacon_frame(node_ctx_t* ctx, uint64_t local_us, const adq_frame_t* beacon_frame) {
    uint8_t slot_index = 0;
    uint32_t beacon_interval_us = 0;
    uint16_t slot_us = 0;
    uint16_t guard_us = 0;
    uint8_t max_nodes = 0;

    if (ctx == 0 || beacon_frame == 0) {
        return -1;
    }

    if (beacon_frame->msg_type != ADQ_MSG_BEACON) {
        return -2;
    }

    if (adq_mac_scheduler_parse_beacon_payload(beacon_frame->payload,
                                               beacon_frame->payload_len,
                                               &slot_index,
                                               &beacon_interval_us,
                                               &slot_us,
                                               &guard_us,
                                               &max_nodes) != 0) {
        return -3;
    }

    node_on_beacon(ctx, local_us, beacon_frame->timestamp_us);
    ctx->assigned_slot = slot_index;
    ctx->slot_valid = 1;
    ctx->beacon_interval_us = beacon_interval_us;
    ctx->slot_us = slot_us;
    ctx->guard_us = guard_us;
    (void)max_nodes;
    return 0;
}

int node_can_tx_now(const node_ctx_t* ctx, uint64_t local_us) {
    uint64_t network_now = 0;
    uint64_t delta = 0;
    uint64_t slot_start = 0;
    uint64_t slot_end = 0;

    if (ctx == 0 || !ctx->slot_valid || ctx->beacon_interval_us == 0 || ctx->slot_us == 0) {
        return 0;
    }

    network_now = adq_time_sync_to_network(&ctx->time_sync, local_us);
    if (network_now < ctx->last_beacon_network_us) {
        return 0;
    }

    delta = (network_now - ctx->last_beacon_network_us) % ctx->beacon_interval_us;
    slot_start = ((uint64_t)ctx->assigned_slot * ctx->slot_us) + ctx->guard_us;
    slot_end = ((uint64_t)(ctx->assigned_slot + 1u) * ctx->slot_us);
    if (slot_end > ctx->guard_us) {
        slot_end -= ctx->guard_us;
    }

    return (delta >= slot_start && delta <= slot_end) ? 1 : 0;
}

int node_mark_tx_attempt(node_ctx_t* ctx, uint16_t seq, uint64_t local_us) {
    uint64_t network_now = 0;
    int rc = 0;

    if (ctx == 0) {
        return -1;
    }

    network_now = adq_time_sync_to_network(&ctx->time_sync, local_us);
    rc = adq_txn_manager_on_tx(&ctx->txn, seq, network_now);
    if (rc == 0 && ctx->txn.attempts > 0) {
        ctx->retry_count = (uint8_t)(ctx->txn.attempts - 1u);
    }
    return rc;
}

int node_handle_feedback(node_ctx_t* ctx, const adq_frame_t* feedback_frame) {
    int rc = 0;

    if (ctx == 0 || feedback_frame == 0) {
        return -1;
    }

    rc = adq_txn_manager_on_feedback(&ctx->txn, feedback_frame);
    if (ctx->txn.state == ADQ_TXN_COMPLETED) {
        ctx->retry_count = 0;
    } else if (ctx->txn.state == ADQ_TXN_RETRY_READY && ctx->txn.attempts > 0) {
        ctx->retry_count = (uint8_t)(ctx->txn.attempts - 1u);
    }

    return rc;
}

int node_poll_retry(node_ctx_t* ctx, uint64_t local_us, int* should_retry, int* failed) {
    uint64_t network_now = 0;
    int rc = 0;

    if (ctx == 0 || should_retry == 0 || failed == 0) {
        return -1;
    }

    network_now = adq_time_sync_to_network(&ctx->time_sync, local_us);
    rc = adq_txn_manager_poll(&ctx->txn, network_now, should_retry, failed);
    if (*should_retry && ctx->txn.attempts > 0) {
        ctx->retry_count = (uint8_t)(ctx->txn.attempts - 1u);
    }
    if (*failed) {
        ctx->state = NODE_RECOVERY;
    }

    return rc;
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
    frame->flags = (uint8_t)(ctx->retry_count & 0x0Fu);
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
