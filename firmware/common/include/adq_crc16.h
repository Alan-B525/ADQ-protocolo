#ifndef ADQ_CRC16_H
#define ADQ_CRC16_H

#include <stddef.h>
#include <stdint.h>

uint16_t adq_crc16_ccitt(const uint8_t* data, size_t len);

#endif
