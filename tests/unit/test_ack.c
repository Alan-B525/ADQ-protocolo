#include <assert.h>

#include "adq_ack.h"

static void test_ack_roundtrip(void) {
    adq_frame_t frame;
    uint16_t seq = 0;
    int is_ack = 0;
    adq_nack_reason_t reason = ADQ_NACK_REASON_BUSY;

    assert(adq_ack_build_frame(0x0101, 42, 1000000u, 1, ADQ_NACK_REASON_NONE, &frame) == 0);
    assert(frame.msg_type == ADQ_MSG_ACK);
    assert(frame.payload_len == ADQ_ACK_PAYLOAD_SIZE);
    assert(adq_ack_parse_frame(&frame, &seq, &is_ack, &reason) == 0);
    assert(is_ack == 1);
    assert(seq == 42u);
    assert(reason == ADQ_NACK_REASON_NONE);
}

static void test_nack_roundtrip(void) {
    adq_frame_t frame;
    uint16_t seq = 0;
    int is_ack = 1;
    adq_nack_reason_t reason = ADQ_NACK_REASON_NONE;

    assert(adq_ack_build_frame(0x0101, 77, 1000001u, 0, ADQ_NACK_REASON_SEQ_GAP, &frame) == 0);
    assert(frame.msg_type == ADQ_MSG_NACK);
    assert(adq_ack_parse_frame(&frame, &seq, &is_ack, &reason) == 0);
    assert(is_ack == 0);
    assert(seq == 77u);
    assert(reason == ADQ_NACK_REASON_SEQ_GAP);
}

static void test_txn_timeout_and_ack(void) {
    adq_txn_manager_t manager;
    adq_frame_t ack;
    int should_retry = 0;
    int failed = 0;

    adq_txn_manager_init(&manager, 1000u, 2u);
    assert(adq_txn_manager_on_tx(&manager, 10u, 10000u) == 0);

    assert(adq_txn_manager_poll(&manager, 10500u, &should_retry, &failed) == 0);
    assert(should_retry == 0);
    assert(failed == 0);

    assert(adq_txn_manager_poll(&manager, 11000u, &should_retry, &failed) == 1);
    assert(should_retry == 1);
    assert(failed == 0);
    assert(manager.state == ADQ_TXN_RETRY_READY);

    assert(adq_txn_manager_on_tx(&manager, 10u, 11000u) == 0);
    assert(adq_ack_build_frame(0x0101, 10u, 11020u, 1, ADQ_NACK_REASON_NONE, &ack) == 0);
    assert(adq_txn_manager_on_feedback(&manager, &ack) == 1);
    assert(manager.state == ADQ_TXN_COMPLETED);
}

static void test_txn_fails_after_retry_budget(void) {
    adq_txn_manager_t manager;
    int should_retry = 0;
    int failed = 0;

    adq_txn_manager_init(&manager, 1000u, 1u);
    assert(adq_txn_manager_on_tx(&manager, 5u, 1000u) == 0);
    assert(adq_txn_manager_poll(&manager, 2000u, &should_retry, &failed) == 1);
    assert(should_retry == 1);
    assert(failed == 0);

    assert(adq_txn_manager_on_tx(&manager, 5u, 2000u) == 0);
    assert(adq_txn_manager_poll(&manager, 3000u, &should_retry, &failed) == 2);
    assert(should_retry == 0);
    assert(failed == 1);
    assert(manager.state == ADQ_TXN_FAILED);
}

int main(void) {
    test_ack_roundtrip();
    test_nack_roundtrip();
    test_txn_timeout_and_ack();
    test_txn_fails_after_retry_budget();
    return 0;
}