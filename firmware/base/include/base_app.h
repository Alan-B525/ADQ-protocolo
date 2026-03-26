#ifndef BASE_APP_H
#define BASE_APP_H

#include <stddef.h>
#include <stdint.h>

#include "adq_ack.h"
#include "adq_link.h"
#include "adq_mac_scheduler.h"
#include "adq_protocol.h"
#include "adq_stream_parser.h"

typedef struct {
    adq_link_params_t link_params;
    adq_mac_scheduler_t scheduler;
    adq_stream_parser_t stream_parser;
    uint16_t beacon_seq;
    uint32_t rx_ok;
    uint32_t rx_crc_error;
    uint32_t rx_unknown;
} base_stats_t;

void base_init(base_stats_t* stats);
int base_set_link_params(base_stats_t* stats, const adq_link_params_t* params);
int base_register_node(base_stats_t* stats, uint16_t node_id, uint64_t now_us, uint8_t* out_slot_index);
int base_build_beacon_for_node(base_stats_t* stats, uint16_t node_id, uint64_t now_us, adq_frame_t* frame);
int base_build_feedback_for_data(const adq_frame_t* data_frame,
                                 int accept,
                                 adq_nack_reason_t reason,
                                 uint64_t now_us,
                                 adq_frame_t* feedback_frame);
int base_process_bytes(base_stats_t* stats, const uint8_t* data, size_t len, adq_frame_t* out);
int base_process_stream(base_stats_t* stats,
                        const uint8_t* data,
                        size_t len,
                        adq_frame_t* out,
                        uint32_t* frames_decoded);

#endif
