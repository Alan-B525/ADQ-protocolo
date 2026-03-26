#include "adq_rf_hal.h"

#include <string.h>

int adq_rf_hal_init(adq_rf_hal_t* hal, const adq_link_params_t* params) {
    if (hal == 0 || params == 0) {
        return -1;
    }

    if (adq_link_params_validate(params) != 0) {
        return -2;
    }

    memset(hal, 0, sizeof(*hal));
    hal->params = *params;
    hal->last_rssi_dbm = -127;
    return 0;
}

void adq_rf_hal_set_loopback(adq_rf_hal_t* hal, int enabled) {
    if (hal == 0) {
        return;
    }

    hal->loopback_enabled = enabled ? 1u : 0u;
}

int adq_rf_hal_send(adq_rf_hal_t* hal, const uint8_t* data, size_t len) {
    if (hal == 0 || data == 0 || len == 0) {
        return -1;
    }

    if (len > ADQ_RF_HAL_MAX_FRAME) {
        return -2;
    }

    hal->last_rssi_dbm = -55;

    if (hal->loopback_enabled) {
        memcpy(hal->rx_buffer, data, len);
        hal->rx_len = len;
    }

    return 0;
}

int adq_rf_hal_receive(adq_rf_hal_t* hal, uint8_t* out, size_t out_cap, size_t* out_len) {
    if (hal == 0 || out == 0 || out_len == 0) {
        return -1;
    }

    if (hal->rx_len == 0) {
        return -2;
    }

    if (out_cap < hal->rx_len) {
        return -3;
    }

    memcpy(out, hal->rx_buffer, hal->rx_len);
    *out_len = hal->rx_len;
    hal->rx_len = 0;
    return 0;
}

int adq_rf_hal_last_rssi(const adq_rf_hal_t* hal, int8_t* rssi_dbm) {
    if (hal == 0 || rssi_dbm == 0) {
        return -1;
    }

    *rssi_dbm = hal->last_rssi_dbm;
    return 0;
}