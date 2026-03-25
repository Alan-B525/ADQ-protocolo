#ifndef NODE_APP_H
#define NODE_APP_H

#include <stdint.h>

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
    node_state_t state;
    adq_time_sync_t time_sync;
} node_ctx_t;

void node_init(node_ctx_t* ctx, uint16_t node_id);
void node_on_beacon(node_ctx_t* ctx, uint64_t local_us, uint64_t beacon_us);
void node_prepare_data_frame(node_ctx_t* ctx, uint64_t local_us, adq_frame_t* frame);

#endif
