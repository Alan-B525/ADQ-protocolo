#include <assert.h>
#include <string.h>

#include "adq_mac_scheduler.h"
#include "node_app.h"

static void test_node_slot_gate_from_beacon(void) {
    adq_mac_scheduler_t scheduler;
    node_ctx_t node;
    adq_frame_t beacon;
    uint8_t slot = 0;
    uint8_t payload[ADQ_BEACON_PAYLOAD_SIZE];
    uint16_t payload_len = 0;

    adq_mac_scheduler_init(&scheduler, 100000u, 10000u, 1000u, 8);
    assert(adq_mac_scheduler_add_or_refresh_node(&scheduler, 0x0101, 1000u, &slot) == 0);
    assert(adq_mac_scheduler_make_beacon_payload(&scheduler, 0x0101, payload, sizeof(payload), &payload_len) == 0);

    memset(&beacon, 0, sizeof(beacon));
    beacon.start = ADQ_FRAME_START;
    beacon.proto_version = ADQ_PROTO_VERSION;
    beacon.msg_type = ADQ_MSG_BEACON;
    beacon.timestamp_us = 1000000u;
    beacon.payload_len = payload_len;
    memcpy(beacon.payload, payload, payload_len);

    node_init(&node, 0x0101);
    assert(node_apply_beacon_frame(&node, 1000000u, &beacon) == 0);
    assert(node_can_tx_now(&node, 1005000u) == 1);
    assert(node_can_tx_now(&node, 1000500u) == 0);
    assert(node_can_tx_now(&node, 1011000u) == 0);
}

int main(void) {
    test_node_slot_gate_from_beacon();
    return 0;
}