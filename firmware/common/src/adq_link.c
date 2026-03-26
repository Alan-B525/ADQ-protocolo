#include "adq_link.h"

void adq_link_params_default_100m(adq_link_params_t* params) {
    if (params == 0) {
        return;
    }

    params->stack = ADQ_RF_STACK_ESB_LIKE;
    params->channel = 40;
    params->bitrate_kbps = 1000;
    params->tx_power_dbm = 0;
    params->ack_timeout_ms = 20;
    params->max_retries = 5;
    params->target_range_m = 100;
}

int adq_link_params_validate(const adq_link_params_t* params) {
    if (params == 0) {
        return -1;
    }

    if (params->stack != ADQ_RF_STACK_ESB_LIKE && params->stack != ADQ_RF_STACK_BLE) {
        return -2;
    }

    if (params->channel > 100) {
        return -3;
    }

    if (params->bitrate_kbps == 0 || params->bitrate_kbps > 2000) {
        return -4;
    }

    if (params->tx_power_dbm < -40 || params->tx_power_dbm > 8) {
        return -5;
    }

    if (params->ack_timeout_ms == 0 || params->ack_timeout_ms > 1000) {
        return -6;
    }

    if (params->max_retries == 0 || params->max_retries > 15) {
        return -7;
    }

    if (params->target_range_m == 0 || params->target_range_m > 2000) {
        return -8;
    }

    return 0;
}

const char* adq_rf_stack_name(adq_rf_stack_t stack) {
    if (stack == ADQ_RF_STACK_BLE) {
        return "BLE";
    }
    return "ESB-LIKE";
}