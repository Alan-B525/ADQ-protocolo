#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "adq_link.h"
#include "adq_rf_hal.h"

static void test_loopback_send_receive(void) {
    adq_link_params_t params;
    adq_rf_hal_t hal;
    uint8_t tx[4] = {0xA5u, 0x01u, 0x07u, 0x00u};
    uint8_t rx[4] = {0};
    size_t rx_len = 0;
    int8_t rssi = 0;

    adq_link_params_default_100m(&params);
    assert(adq_rf_hal_init(&hal, &params) == 0);

    adq_rf_hal_set_loopback(&hal, 1);
    assert(adq_rf_hal_send(&hal, tx, sizeof(tx)) == 0);
    assert(adq_rf_hal_receive(&hal, rx, sizeof(rx), &rx_len) == 0);
    assert(rx_len == sizeof(tx));
    assert(rx[0] == tx[0]);
    assert(rx[1] == tx[1]);
    assert(rx[2] == tx[2]);
    assert(rx[3] == tx[3]);

    assert(adq_rf_hal_last_rssi(&hal, &rssi) == 0);
    assert(rssi > -127);
}

static void test_reject_invalid_params(void) {
    adq_link_params_t params;
    adq_rf_hal_t hal;

    adq_link_params_default_100m(&params);
    params.max_retries = 0;
    assert(adq_rf_hal_init(&hal, &params) != 0);
}

int main(void) {
    test_loopback_send_receive();
    test_reject_invalid_params();
    return 0;
}