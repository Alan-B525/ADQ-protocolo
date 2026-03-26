#include "base_app.h"

#include <string.h>

#include "adq_codec.h"

void base_init(base_stats_t* stats) {
    adq_link_params_default_100m(&stats->link_params);
    adq_mac_scheduler_init(&stats->scheduler, 100000u, 10000u, 1000u, ADQ_MAC_MAX_NODES);
    adq_stream_parser_init(&stats->stream_parser);
    stats->beacon_seq = 0;
    stats->rx_ok = 0;
    stats->rx_crc_error = 0;
    stats->rx_unknown = 0;
}

int base_set_link_params(base_stats_t* stats, const adq_link_params_t* params) {
    if (stats == 0 || params == 0) {
        return -1;
    }

    if (adq_link_params_validate(params) != 0) {
        return -2;
    }

    stats->link_params = *params;
    return 0;
}

int base_register_node(base_stats_t* stats, uint16_t node_id, uint64_t now_us, uint8_t* out_slot_index) {
    if (stats == 0 || out_slot_index == 0) {
        return -1;
    }

    return adq_mac_scheduler_add_or_refresh_node(&stats->scheduler, node_id, now_us, out_slot_index);
}

int base_build_beacon_for_node(base_stats_t* stats, uint16_t node_id, uint64_t now_us, adq_frame_t* frame) {
    uint8_t payload[ADQ_BEACON_PAYLOAD_SIZE];
    uint16_t payload_len = 0;

    if (stats == 0 || frame == 0) {
        return -1;
    }

    if (adq_mac_scheduler_make_beacon_payload(&stats->scheduler, node_id, payload, sizeof(payload), &payload_len) != 0) {
        return -2;
    }

    memset(frame, 0, sizeof(*frame));
    frame->start = ADQ_FRAME_START;
    frame->proto_version = ADQ_PROTO_VERSION;
    frame->msg_type = ADQ_MSG_BEACON;
    frame->flags = 0;
    frame->node_id = 0;
    frame->seq = stats->beacon_seq++;
    frame->timestamp_us = now_us;
    frame->payload_len = payload_len;
    memcpy(frame->payload, payload, payload_len);
    return 0;
}

int base_build_feedback_for_data(const adq_frame_t* data_frame,
                                 int accept,
                                 adq_nack_reason_t reason,
                                 uint64_t now_us,
                                 adq_frame_t* feedback_frame) {
    if (data_frame == 0 || feedback_frame == 0) {
        return -1;
    }

    if (data_frame->msg_type != ADQ_MSG_DATA_FRAME) {
        return -2;
    }

    if (!accept && reason == ADQ_NACK_REASON_NONE) {
        return -3;
    }

    return adq_ack_build_frame(data_frame->node_id,
                               data_frame->seq,
                               now_us,
                               accept,
                               reason,
                               feedback_frame);
}

int base_process_bytes(base_stats_t* stats, const uint8_t* data, size_t len, adq_frame_t* out) {
    const int rc = adq_decode_frame(data, len, out);
    if (rc == 0) {
        stats->rx_ok++;
        return 0;
    }

    if (rc == -4) {
        stats->rx_crc_error++;
    } else {
        stats->rx_unknown++;
    }

    return rc;
}

int base_process_stream(base_stats_t* stats,
                        const uint8_t* data,
                        size_t len,
                        adq_frame_t* out,
                        uint32_t* frames_decoded) {
    const int rc = adq_stream_parser_push(&stats->stream_parser, data, len, out, frames_decoded);
    if (rc == 0) {
        stats->rx_ok += *frames_decoded;
        return 0;
    }

    if (rc == -4) {
        stats->rx_crc_error++;
    } else {
        stats->rx_unknown++;
    }

    return rc;
}
