#ifndef ADQ_MAC_SCHEDULER_H
#define ADQ_MAC_SCHEDULER_H

#include <stddef.h>
#include <stdint.h>

#define ADQ_MAC_MAX_NODES 16u
#define ADQ_BEACON_PAYLOAD_VERSION 1u
#define ADQ_BEACON_PAYLOAD_SIZE 11u

typedef struct {
    uint16_t node_id;
    uint8_t slot_index;
    uint8_t active;
    uint64_t last_seen_us;
} adq_node_slot_t;

typedef struct {
    uint32_t beacon_interval_us;
    uint16_t slot_us;
    uint16_t guard_us;
    uint8_t max_nodes;
    adq_node_slot_t nodes[ADQ_MAC_MAX_NODES];
} adq_mac_scheduler_t;

void adq_mac_scheduler_init(adq_mac_scheduler_t* scheduler,
                            uint32_t beacon_interval_us,
                            uint16_t slot_us,
                            uint16_t guard_us,
                            uint8_t max_nodes);
int adq_mac_scheduler_add_or_refresh_node(adq_mac_scheduler_t* scheduler,
                                          uint16_t node_id,
                                          uint64_t now_us,
                                          uint8_t* out_slot_index);
int adq_mac_scheduler_get_slot_window(const adq_mac_scheduler_t* scheduler,
                                      uint16_t node_id,
                                      uint64_t beacon_time_us,
                                      uint64_t* slot_start_us,
                                      uint64_t* slot_end_us);
int adq_mac_scheduler_make_beacon_payload(const adq_mac_scheduler_t* scheduler,
                                          uint16_t node_id,
                                          uint8_t* out_payload,
                                          uint16_t out_cap,
                                          uint16_t* out_len);
int adq_mac_scheduler_parse_beacon_payload(const uint8_t* payload,
                                           uint16_t payload_len,
                                           uint8_t* slot_index,
                                           uint32_t* beacon_interval_us,
                                           uint16_t* slot_us,
                                           uint16_t* guard_us,
                                           uint8_t* max_nodes);

#endif