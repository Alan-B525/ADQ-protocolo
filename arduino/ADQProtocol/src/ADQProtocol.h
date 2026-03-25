#ifndef ADQ_PROTOCOL_ARDUINO_WRAPPER_H
#define ADQ_PROTOCOL_ARDUINO_WRAPPER_H

#include <Arduino.h>

extern "C" {
#include "adq_codec.h"
#include "adq_time_sync.h"
}

class ADQProtocol {
public:
    static bool encode(const adq_frame_t& frame, uint8_t* out, size_t outCap, size_t& outLen);
    static bool decode(const uint8_t* in, size_t inLen, adq_frame_t& out);
};

#endif
