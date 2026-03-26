#ifndef ADQ_FRAGMENT_H
#define ADQ_FRAGMENT_H

#include <stddef.h>
#include <stdint.h>

#include "adq_protocol.h"

#define ADQ_RF_FRAGMENT_OVERHEAD 3u
#define ADQ_RF_FRAGMENT_MAX_PAYLOAD 64u
#define ADQ_RF_FRAGMENT_MAX_COUNT 16u
#define ADQ_RF_REASSEMBLY_MAX_BYTES 300u

typedef struct {
    uint8_t bytes[ADQ_RF_FRAGMENT_MAX_PAYLOAD];
    size_t len;
} adq_rf_fragment_t;

typedef struct {
    uint8_t count;
    adq_rf_fragment_t fragments[ADQ_RF_FRAGMENT_MAX_COUNT];
} adq_fragment_list_t;

typedef struct {
    uint8_t in_progress;
    uint8_t expected_count;
    uint8_t received_count;
    size_t total_len;
    uint8_t data[ADQ_RF_REASSEMBLY_MAX_BYTES];
} adq_reassembly_ctx_t;

int adq_fragment_frame(const adq_frame_t* frame, size_t rf_payload_max, adq_fragment_list_t* out_list);
void adq_reassembly_reset(adq_reassembly_ctx_t* ctx);
int adq_reassembly_push(adq_reassembly_ctx_t* ctx,
                        const uint8_t* fragment,
                        size_t fragment_len,
                        uint8_t* out_frame,
                        size_t out_cap,
                        size_t* out_frame_len,
                        int* completed);

#endif