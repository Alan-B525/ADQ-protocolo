#include "ADQProtocol.h"

bool ADQProtocol::encode(const adq_frame_t& frame, uint8_t* out, size_t outCap, size_t& outLen) {
    return adq_encode_frame(&frame, out, outCap, &outLen) == 0;
}

bool ADQProtocol::decode(const uint8_t* in, size_t inLen, adq_frame_t& out) {
    return adq_decode_frame(in, inLen, &out) == 0;
}
