#ifndef ADQ_CODEC_H
#define ADQ_CODEC_H

#include <stddef.h>
#include <stdint.h>

#include "adq_protocol.h"

int adq_encode_frame(const adq_frame_t* frame, uint8_t* out, size_t out_cap, size_t* out_len);
int adq_decode_frame(const uint8_t* in, size_t in_len, adq_frame_t* out);

#endif
