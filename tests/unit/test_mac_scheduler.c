#include <assert.h>
#include <stdint.h>

#include "adq_mac_scheduler.h"

static void test_slot_assignment_and_window(void) {
    adq_mac_scheduler_t scheduler;
    uint8_t slot0 = 0;
    uint8_t slot1 = 0;
    uint64_t start_us = 0;
    uint64_t end_us = 0;

    adq_mac_scheduler_init(&scheduler, 100000u, 10000u, 1000u, 8);
    assert(adq_mac_scheduler_add_or_refresh_node(&scheduler, 0x0101, 1000u, &slot0) == 0);
    assert(adq_mac_scheduler_add_or_refresh_node(&scheduler, 0x0102, 1000u, &slot1) == 0);
    assert(slot0 == 0u);
    assert(slot1 == 1u);

    assert(adq_mac_scheduler_get_slot_window(&scheduler, 0x0102, 1000000u, &start_us, &end_us) == 0);
    assert(start_us == 1011000u);
    assert(end_us == 1019000u);
}

static void test_beacon_payload_roundtrip(void) {
    adq_mac_scheduler_t scheduler;
    uint8_t slot = 0;
    uint8_t payload[ADQ_BEACON_PAYLOAD_SIZE];
    uint16_t payload_len = 0;
    uint8_t parsed_slot = 0;
    uint32_t parsed_interval = 0;
    uint16_t parsed_slot_us = 0;
    uint16_t parsed_guard_us = 0;
    uint8_t parsed_max_nodes = 0;

    adq_mac_scheduler_init(&scheduler, 100000u, 10000u, 1000u, 8);
    assert(adq_mac_scheduler_add_or_refresh_node(&scheduler, 0x0101, 1000u, &slot) == 0);
    assert(adq_mac_scheduler_make_beacon_payload(&scheduler, 0x0101, payload, sizeof(payload), &payload_len) == 0);
    assert(payload_len == ADQ_BEACON_PAYLOAD_SIZE);

    assert(adq_mac_scheduler_parse_beacon_payload(payload,
                                                  payload_len,
                                                  &parsed_slot,
                                                  &parsed_interval,
                                                  &parsed_slot_us,
                                                  &parsed_guard_us,
                                                  &parsed_max_nodes) == 0);
    assert(parsed_slot == slot);
    assert(parsed_interval == 100000u);
    assert(parsed_slot_us == 10000u);
    assert(parsed_guard_us == 1000u);
    assert(parsed_max_nodes == 8u);
}

int main(void) {
    test_slot_assignment_and_window();
    test_beacon_payload_roundtrip();
    return 0;
}