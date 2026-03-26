#ifndef ADQ_LINK_H
#define ADQ_LINK_H

#include <stdint.h>

typedef enum {
    ADQ_RF_STACK_ESB_LIKE = 0,
    ADQ_RF_STACK_BLE = 1
} adq_rf_stack_t;

typedef struct {
    adq_rf_stack_t stack;
    uint8_t channel;
    uint32_t bitrate_kbps;
    int8_t tx_power_dbm;
    uint16_t ack_timeout_ms;
    uint8_t max_retries;
    uint16_t target_range_m;
} adq_link_params_t;

void adq_link_params_default_100m(adq_link_params_t* params);
int adq_link_params_validate(const adq_link_params_t* params);
const char* adq_rf_stack_name(adq_rf_stack_t stack);

#endif