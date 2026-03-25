#ifndef BASE_APP_H
#define BASE_APP_H

#include <stddef.h>
#include <stdint.h>

#include "adq_protocol.h"

typedef struct {
    uint32_t rx_ok;
    uint32_t rx_crc_error;
    uint32_t rx_unknown;
} base_stats_t;

void base_init(base_stats_t* stats);
int base_process_bytes(base_stats_t* stats, const uint8_t* data, size_t len, adq_frame_t* out);

#endif
