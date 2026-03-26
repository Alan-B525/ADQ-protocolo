#include "adq_mac_scheduler.h"

#include <string.h>

static void write_le16(uint8_t* p, uint16_t v) {
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
}

static void write_le32(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
    p[2] = (uint8_t)((v >> 16) & 0xFFu);
    p[3] = (uint8_t)((v >> 24) & 0xFFu);
}

static uint16_t read_le16(const uint8_t* p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t read_le32(const uint8_t* p) {
    return (uint32_t)p[0]
         | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

static int find_node_slot(const adq_mac_scheduler_t* scheduler, uint16_t node_id) {
    uint8_t i = 0;
    for (i = 0; i < scheduler->max_nodes; ++i) {
        if (scheduler->nodes[i].active && scheduler->nodes[i].node_id == node_id) {
            return i;
        }
    }

    return -1;
}

static int find_free_slot(const adq_mac_scheduler_t* scheduler) {
    uint8_t i = 0;
    for (i = 0; i < scheduler->max_nodes; ++i) {
        if (!scheduler->nodes[i].active) {
            return i;
        }
    }

    return -1;
}

void adq_mac_scheduler_init(adq_mac_scheduler_t* scheduler,
                            uint32_t beacon_interval_us,
                            uint16_t slot_us,
                            uint16_t guard_us,
                            uint8_t max_nodes) {
    uint8_t i = 0;

    if (scheduler == 0) {
        return;
    }

    memset(scheduler, 0, sizeof(*scheduler));
    scheduler->beacon_interval_us = beacon_interval_us == 0 ? 100000u : beacon_interval_us;
    scheduler->slot_us = slot_us == 0 ? 10000u : slot_us;
    scheduler->guard_us = guard_us;
    scheduler->max_nodes = max_nodes == 0 ? 1u : max_nodes;
    if (scheduler->max_nodes > ADQ_MAC_MAX_NODES) {
        scheduler->max_nodes = ADQ_MAC_MAX_NODES;
    }

    for (i = 0; i < scheduler->max_nodes; ++i) {
        scheduler->nodes[i].slot_index = i;
    }
}

int adq_mac_scheduler_add_or_refresh_node(adq_mac_scheduler_t* scheduler,
                                          uint16_t node_id,
                                          uint64_t now_us,
                                          uint8_t* out_slot_index) {
    int index = 0;

    if (scheduler == 0 || out_slot_index == 0 || node_id == 0) {
        return -1;
    }

    index = find_node_slot(scheduler, node_id);
    if (index >= 0) {
        scheduler->nodes[index].last_seen_us = now_us;
        *out_slot_index = scheduler->nodes[index].slot_index;
        return 0;
    }

    index = find_free_slot(scheduler);
    if (index < 0) {
        return -2;
    }

    scheduler->nodes[index].active = 1u;
    scheduler->nodes[index].node_id = node_id;
    scheduler->nodes[index].last_seen_us = now_us;
    *out_slot_index = scheduler->nodes[index].slot_index;
    return 0;
}

int adq_mac_scheduler_get_slot_window(const adq_mac_scheduler_t* scheduler,
                                      uint16_t node_id,
                                      uint64_t beacon_time_us,
                                      uint64_t* slot_start_us,
                                      uint64_t* slot_end_us) {
    int index = 0;
    uint64_t slot_base = 0;

    if (scheduler == 0 || slot_start_us == 0 || slot_end_us == 0) {
        return -1;
    }

    index = find_node_slot(scheduler, node_id);
    if (index < 0) {
        return -2;
    }

    slot_base = beacon_time_us + ((uint64_t)scheduler->nodes[index].slot_index * scheduler->slot_us);
    *slot_start_us = slot_base + scheduler->guard_us;

    if (scheduler->slot_us <= (uint16_t)(scheduler->guard_us * 2u)) {
        *slot_end_us = *slot_start_us;
    } else {
        *slot_end_us = slot_base + scheduler->slot_us - scheduler->guard_us;
    }

    return 0;
}

int adq_mac_scheduler_make_beacon_payload(const adq_mac_scheduler_t* scheduler,
                                          uint16_t node_id,
                                          uint8_t* out_payload,
                                          uint16_t out_cap,
                                          uint16_t* out_len) {
    int index = 0;

    if (scheduler == 0 || out_payload == 0 || out_len == 0) {
        return -1;
    }

    if (out_cap < ADQ_BEACON_PAYLOAD_SIZE) {
        return -2;
    }

    index = find_node_slot(scheduler, node_id);
    if (index < 0) {
        return -3;
    }

    out_payload[0] = ADQ_BEACON_PAYLOAD_VERSION;
    out_payload[1] = scheduler->nodes[index].slot_index;
    write_le32(&out_payload[2], scheduler->beacon_interval_us);
    write_le16(&out_payload[6], scheduler->slot_us);
    write_le16(&out_payload[8], scheduler->guard_us);
    out_payload[10] = scheduler->max_nodes;
    *out_len = ADQ_BEACON_PAYLOAD_SIZE;
    return 0;
}

int adq_mac_scheduler_parse_beacon_payload(const uint8_t* payload,
                                           uint16_t payload_len,
                                           uint8_t* slot_index,
                                           uint32_t* beacon_interval_us,
                                           uint16_t* slot_us,
                                           uint16_t* guard_us,
                                           uint8_t* max_nodes) {
    if (payload == 0 || slot_index == 0 || beacon_interval_us == 0 || slot_us == 0 || guard_us == 0 || max_nodes == 0) {
        return -1;
    }

    if (payload_len != ADQ_BEACON_PAYLOAD_SIZE) {
        return -2;
    }

    if (payload[0] != ADQ_BEACON_PAYLOAD_VERSION) {
        return -3;
    }

    *slot_index = payload[1];
    *beacon_interval_us = read_le32(&payload[2]);
    *slot_us = read_le16(&payload[6]);
    *guard_us = read_le16(&payload[8]);
    *max_nodes = payload[10];
    return 0;
}