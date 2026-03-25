#ifndef ADQ_TIME_SYNC_H
#define ADQ_TIME_SYNC_H

#include <stdint.h>

typedef struct {
    int64_t offset_us;
    int64_t drift_ppm;
    uint8_t locked;
} adq_time_sync_t;

void adq_time_sync_init(adq_time_sync_t* ts);
void adq_time_sync_update(adq_time_sync_t* ts, uint64_t local_us, uint64_t beacon_us);
uint64_t adq_time_sync_to_network(const adq_time_sync_t* ts, uint64_t local_us);

#endif
