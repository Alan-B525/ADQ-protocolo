#ifndef ADQ_RF_HAL_H
#define ADQ_RF_HAL_H

#include <stddef.h>
#include <stdint.h>

#include "adq_link.h"

#define ADQ_RF_HAL_MAX_FRAME 300u

typedef struct {
    adq_link_params_t params;
    uint8_t loopback_enabled;
    int8_t last_rssi_dbm;
    uint8_t rx_buffer[ADQ_RF_HAL_MAX_FRAME];
    size_t rx_len;
} adq_rf_hal_t;

int adq_rf_hal_init(adq_rf_hal_t* hal, const adq_link_params_t* params);
void adq_rf_hal_set_loopback(adq_rf_hal_t* hal, int enabled);
int adq_rf_hal_send(adq_rf_hal_t* hal, const uint8_t* data, size_t len);
int adq_rf_hal_receive(adq_rf_hal_t* hal, uint8_t* out, size_t out_cap, size_t* out_len);
int adq_rf_hal_last_rssi(const adq_rf_hal_t* hal, int8_t* rssi_dbm);

#endif