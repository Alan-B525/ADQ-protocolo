#ifndef NODE_APP_H
#define NODE_APP_H

#include <stdint.h>

#include "adq_ack.h"
#include "adq_link.h"
#include "adq_mac_scheduler.h"
#include "adq_protocol.h"
#include "adq_time_sync.h"

typedef enum {
    NODE_BOOT = 0,
    NODE_DISCOVERY,
    NODE_JOINED,
    NODE_SYNCED,
    NODE_STREAMING,
    NODE_RECOVERY,
    NODE_ERROR
} node_state_t;

typedef struct {
    uint16_t node_id;
    uint16_t seq;
    uint8_t retry_count;
    uint8_t assigned_slot;
    uint8_t slot_valid;
    node_state_t state;
    uint32_t beacon_interval_us;
    uint16_t slot_us;
    uint16_t guard_us;
    uint64_t last_beacon_local_us;
    uint64_t last_beacon_network_us;
    adq_link_params_t link_params;
    adq_txn_manager_t txn;
    adq_time_sync_t time_sync;
} node_ctx_t;

void node_init(node_ctx_t* ctx, uint16_t node_id);
int node_configure_link(node_ctx_t* ctx, const adq_link_params_t* params);
void node_on_beacon(node_ctx_t* ctx, uint64_t local_us, uint64_t beacon_us);
int node_apply_beacon_frame(node_ctx_t* ctx, uint64_t local_us, const adq_frame_t* beacon_frame);
int node_can_tx_now(const node_ctx_t* ctx, uint64_t local_us);
int node_mark_tx_attempt(node_ctx_t* ctx, uint16_t seq, uint64_t local_us);
int node_handle_feedback(node_ctx_t* ctx, const adq_frame_t* feedback_frame);
int node_poll_retry(node_ctx_t* ctx, uint64_t local_us, int* should_retry, int* failed);
void node_prepare_data_frame(node_ctx_t* ctx, uint64_t local_us, adq_frame_t* frame);

#endif
