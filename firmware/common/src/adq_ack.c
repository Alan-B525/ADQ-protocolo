#include "adq_ack.h"

#include <string.h>

static void write_le16(uint8_t* p, uint16_t value) {
    p[0] = (uint8_t)(value & 0xFFu);
    p[1] = (uint8_t)((value >> 8) & 0xFFu);
}

static uint16_t read_le16(const uint8_t* p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

int adq_ack_build_frame(uint16_t node_id,
                        uint16_t seq,
                        uint64_t timestamp_us,
                        int is_ack,
                        adq_nack_reason_t reason,
                        adq_frame_t* out) {
    if (out == 0) {
        return -1;
    }

    if (!is_ack && reason == ADQ_NACK_REASON_NONE) {
        return -2;
    }

    memset(out, 0, sizeof(*out));
    out->start = ADQ_FRAME_START;
    out->proto_version = ADQ_PROTO_VERSION;
    out->msg_type = is_ack ? ADQ_MSG_ACK : ADQ_MSG_NACK;
    out->flags = 0;
    out->node_id = node_id;
    out->seq = seq;
    out->timestamp_us = timestamp_us;
    out->payload_len = ADQ_ACK_PAYLOAD_SIZE;

    write_le16(&out->payload[0], seq);
    out->payload[2] = (uint8_t)(is_ack ? ADQ_NACK_REASON_NONE : reason);
    return 0;
}

int adq_ack_parse_frame(const adq_frame_t* frame,
                        uint16_t* seq,
                        int* is_ack,
                        adq_nack_reason_t* reason) {
    if (frame == 0 || seq == 0 || is_ack == 0 || reason == 0) {
        return -1;
    }

    if (frame->msg_type != ADQ_MSG_ACK && frame->msg_type != ADQ_MSG_NACK) {
        return -2;
    }

    if (frame->payload_len < ADQ_ACK_PAYLOAD_SIZE) {
        return -3;
    }

    *is_ack = frame->msg_type == ADQ_MSG_ACK ? 1 : 0;
    *seq = read_le16(&frame->payload[0]);
    *reason = (adq_nack_reason_t)frame->payload[2];
    return 0;
}

void adq_txn_manager_init(adq_txn_manager_t* manager, uint32_t ack_timeout_us, uint8_t max_retries) {
    if (manager == 0) {
        return;
    }

    memset(manager, 0, sizeof(*manager));
    manager->ack_timeout_us = ack_timeout_us == 0 ? 20000u : ack_timeout_us;
    manager->max_retries = max_retries == 0 ? 1u : max_retries;
    manager->state = ADQ_TXN_IDLE;
}

void adq_txn_manager_reset(adq_txn_manager_t* manager) {
    if (manager == 0) {
        return;
    }

    manager->state = ADQ_TXN_IDLE;
    manager->pending_seq = 0;
    manager->attempts = 0;
    manager->last_tx_us = 0;
}

int adq_txn_manager_on_tx(adq_txn_manager_t* manager, uint16_t seq, uint64_t now_us) {
    if (manager == 0) {
        return -1;
    }

    if (manager->state == ADQ_TXN_WAIT_FEEDBACK) {
        return -2;
    }

    if (manager->state == ADQ_TXN_RETRY_READY) {
        if (seq != manager->pending_seq) {
            return -3;
        }
        manager->attempts++;
    } else {
        manager->pending_seq = seq;
        manager->attempts = 1;
    }

    manager->last_tx_us = now_us;
    manager->state = ADQ_TXN_WAIT_FEEDBACK;
    return 0;
}

int adq_txn_manager_on_feedback(adq_txn_manager_t* manager, const adq_frame_t* feedback_frame) {
    uint16_t feedback_seq = 0;
    int is_ack = 0;
    adq_nack_reason_t reason = ADQ_NACK_REASON_NONE;

    if (manager == 0 || feedback_frame == 0) {
        return -1;
    }

    if (manager->state != ADQ_TXN_WAIT_FEEDBACK) {
        return -2;
    }

    if (adq_ack_parse_frame(feedback_frame, &feedback_seq, &is_ack, &reason) != 0) {
        return -3;
    }

    if (feedback_seq != manager->pending_seq) {
        return -4;
    }

    if (is_ack) {
        manager->state = ADQ_TXN_COMPLETED;
        return 1;
    }

    if (manager->attempts > manager->max_retries) {
        manager->state = ADQ_TXN_FAILED;
        return -5;
    }

    manager->state = ADQ_TXN_RETRY_READY;
    (void)reason;
    return 2;
}

int adq_txn_manager_poll(adq_txn_manager_t* manager, uint64_t now_us, int* retry_ready, int* failed) {
    if (manager == 0 || retry_ready == 0 || failed == 0) {
        return -1;
    }

    *retry_ready = 0;
    *failed = 0;

    if (manager->state != ADQ_TXN_WAIT_FEEDBACK) {
        return 0;
    }

    if (now_us - manager->last_tx_us < manager->ack_timeout_us) {
        return 0;
    }

    if (manager->attempts <= manager->max_retries) {
        manager->state = ADQ_TXN_RETRY_READY;
        *retry_ready = 1;
        return 1;
    }

    manager->state = ADQ_TXN_FAILED;
    *failed = 1;
    return 2;
}