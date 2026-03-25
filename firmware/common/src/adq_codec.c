#include "adq_codec.h"

#include <string.h>

#include "adq_crc16.h"

#define ADQ_HEADER_SIZE 18u
#define ADQ_CRC_SIZE 2u

static void write_le16(uint8_t* p, uint16_t v) {
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
}

static void write_le64(uint8_t* p, uint64_t v) {
    for (uint8_t i = 0; i < 8; ++i) {
        p[i] = (uint8_t)((v >> (8u * i)) & 0xFFu);
    }
}

static uint16_t read_le16(const uint8_t* p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint64_t read_le64(const uint8_t* p) {
    uint64_t v = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        v |= (uint64_t)p[i] << (8u * i);
    }
    return v;
}

int adq_encode_frame(const adq_frame_t* frame, uint8_t* out, size_t out_cap, size_t* out_len) {
    const size_t total = ADQ_HEADER_SIZE + frame->payload_len + ADQ_CRC_SIZE;
    uint16_t crc;

    if (!frame || !out || !out_len) {
        return -1;
    }

    if (frame->payload_len > ADQ_MAX_PAYLOAD || out_cap < total) {
        return -2;
    }

    out[0] = frame->start;
    out[1] = frame->proto_version;
    out[2] = frame->msg_type;
    out[3] = frame->flags;
    write_le16(&out[4], frame->node_id);
    write_le16(&out[6], frame->seq);
    write_le64(&out[8], frame->timestamp_us);
    write_le16(&out[16], frame->payload_len);

    if (frame->payload_len > 0) {
        memcpy(&out[18], frame->payload, frame->payload_len);
    }

    crc = adq_crc16_ccitt(out, ADQ_HEADER_SIZE + frame->payload_len);
    write_le16(&out[ADQ_HEADER_SIZE + frame->payload_len], crc);

    *out_len = total;
    return 0;
}

int adq_decode_frame(const uint8_t* in, size_t in_len, adq_frame_t* out) {
    uint16_t crc_expected;
    uint16_t crc_actual;
    uint16_t payload_len;

    if (!in || !out || in_len < ADQ_HEADER_SIZE + ADQ_CRC_SIZE) {
        return -1;
    }

    payload_len = read_le16(&in[16]);
    if (payload_len > ADQ_MAX_PAYLOAD) {
        return -2;
    }

    if (in_len != ADQ_HEADER_SIZE + payload_len + ADQ_CRC_SIZE) {
        return -3;
    }

    crc_expected = read_le16(&in[ADQ_HEADER_SIZE + payload_len]);
    crc_actual = adq_crc16_ccitt(in, ADQ_HEADER_SIZE + payload_len);
    if (crc_expected != crc_actual) {
        return -4;
    }

    out->start = in[0];
    out->proto_version = in[1];
    out->msg_type = in[2];
    out->flags = in[3];
    out->node_id = read_le16(&in[4]);
    out->seq = read_le16(&in[6]);
    out->timestamp_us = read_le64(&in[8]);
    out->payload_len = payload_len;

    if (payload_len > 0) {
        memcpy(out->payload, &in[18], payload_len);
    }

    out->crc16 = crc_expected;
    return 0;
}
