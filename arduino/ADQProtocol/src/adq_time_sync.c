#include "adq_time_sync.h"

void adq_time_sync_init(adq_time_sync_t* ts) {
    ts->offset_us = 0;
    ts->drift_ppm = 0;
    ts->locked = 0;
}

void adq_time_sync_update(adq_time_sync_t* ts, uint64_t local_us, uint64_t beacon_us) {
    const int64_t new_offset = (int64_t)beacon_us - (int64_t)local_us;

    if (!ts->locked) {
        ts->offset_us = new_offset;
        ts->locked = 1;
        return;
    }

    ts->offset_us = (ts->offset_us * 7 + new_offset) / 8;
}

uint64_t adq_time_sync_to_network(const adq_time_sync_t* ts, uint64_t local_us) {
    return (uint64_t)((int64_t)local_us + ts->offset_us);
}
