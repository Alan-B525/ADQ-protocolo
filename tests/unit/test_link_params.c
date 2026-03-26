#include <assert.h>

#include "adq_link.h"

static void test_default_100m_is_valid(void) {
    adq_link_params_t params;
    adq_link_params_default_100m(&params);

    assert(params.stack == ADQ_RF_STACK_ESB_LIKE);
    assert(params.target_range_m == 100);
    assert(adq_link_params_validate(&params) == 0);
}

static void test_invalid_rejects_bad_retries(void) {
    adq_link_params_t params;
    adq_link_params_default_100m(&params);
    params.max_retries = 0;

    assert(adq_link_params_validate(&params) != 0);
}

static void test_invalid_rejects_bad_channel(void) {
    adq_link_params_t params;
    adq_link_params_default_100m(&params);
    params.channel = 120;

    assert(adq_link_params_validate(&params) != 0);
}

int main(void) {
    test_default_100m_is_valid();
    test_invalid_rejects_bad_retries();
    test_invalid_rejects_bad_channel();
    return 0;
}