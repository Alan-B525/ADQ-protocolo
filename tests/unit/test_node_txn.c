#include <assert.h>

#include "adq_ack.h"
#include "adq_link.h"
#include "node_app.h"

static void test_node_txn_ack_success(void) {
    node_ctx_t node;
    adq_frame_t data;
    adq_frame_t ack;
    int should_retry = 0;
    int failed = 0;

    node_init(&node, 0x0101);
    node_on_beacon(&node, 1000000u, 1000000u);
    node_prepare_data_frame(&node, 1001000u, &data);

    assert(node_mark_tx_attempt(&node, data.seq, 1001000u) == 0);
    assert(node.retry_count == 0u);

    assert(node_poll_retry(&node, 1015000u, &should_retry, &failed) == 0);
    assert(should_retry == 0);
    assert(failed == 0);

    assert(adq_ack_build_frame(node.node_id,
                               data.seq,
                               1015000u,
                               1,
                               ADQ_NACK_REASON_NONE,
                               &ack) == 0);
    assert(node_handle_feedback(&node, &ack) == 1);
    assert(node.retry_count == 0u);
}

static void test_node_txn_enters_recovery_on_fail(void) {
    node_ctx_t node;
    adq_link_params_t params;
    adq_frame_t data;
    int should_retry = 0;
    int failed = 0;

    node_init(&node, 0x0101);
    adq_link_params_default_100m(&params);
    params.max_retries = 1u;
    params.ack_timeout_ms = 1u;
    assert(node_configure_link(&node, &params) == 0);
    node_on_beacon(&node, 1000000u, 1000000u);

    node_prepare_data_frame(&node, 1001000u, &data);
    assert(node_mark_tx_attempt(&node, data.seq, 1001000u) == 0);
    assert(node_poll_retry(&node, 1003000u, &should_retry, &failed) == 1);
    assert(should_retry == 1);
    assert(failed == 0);

    assert(node_mark_tx_attempt(&node, data.seq, 1003000u) == 0);
    assert(node_poll_retry(&node, 1005000u, &should_retry, &failed) == 2);
    assert(should_retry == 0);
    assert(failed == 1);
    assert(node.state == NODE_RECOVERY);
}

int main(void) {
    test_node_txn_ack_success();
    test_node_txn_enters_recovery_on_fail();
    return 0;
}