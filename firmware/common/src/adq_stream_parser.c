#include "adq_stream_parser.h"

#include "adq_codec.h"

#define ADQ_HEADER_SIZE 18u
#define ADQ_CRC_SIZE 2u

static uint16_t read_le16(const uint8_t* p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

void adq_stream_parser_init(adq_stream_parser_t* parser) {
    if (parser == 0) {
        return;
    }

    parser->state = ADQ_STREAM_SEARCH_START;
    parser->index = 0;
    parser->expected_total = 0;
}

void adq_stream_parser_reset(adq_stream_parser_t* parser) {
    adq_stream_parser_init(parser);
}

int adq_stream_parser_push_byte(adq_stream_parser_t* parser, uint8_t byte, adq_frame_t* out, int* frame_ready) {
    if (parser == 0 || out == 0 || frame_ready == 0) {
        return -1;
    }

    *frame_ready = 0;

    if (parser->state == ADQ_STREAM_SEARCH_START) {
        if (byte == ADQ_FRAME_START) {
            parser->buffer[0] = byte;
            parser->index = 1;
            parser->state = ADQ_STREAM_COLLECT_HEADER;
        }
        return 0;
    }

    if (parser->index >= sizeof(parser->buffer)) {
        adq_stream_parser_reset(parser);
        return -2;
    }

    parser->buffer[parser->index++] = byte;

    if (parser->state == ADQ_STREAM_COLLECT_HEADER && parser->index == ADQ_HEADER_SIZE) {
        uint16_t payload_len = read_le16(&parser->buffer[16]);
        if (payload_len > ADQ_MAX_PAYLOAD) {
            adq_stream_parser_reset(parser);
            return -3;
        }

        parser->expected_total = ADQ_HEADER_SIZE + (size_t)payload_len + ADQ_CRC_SIZE;
        if (parser->expected_total > sizeof(parser->buffer)) {
            adq_stream_parser_reset(parser);
            return -4;
        }

        parser->state = ADQ_STREAM_COLLECT_PAYLOAD;
    }

    if (parser->state == ADQ_STREAM_COLLECT_PAYLOAD && parser->index == parser->expected_total) {
        int rc = adq_decode_frame(parser->buffer, parser->expected_total, out);
        adq_stream_parser_reset(parser);
        if (rc != 0) {
            return rc;
        }

        *frame_ready = 1;
    }

    return 0;
}

int adq_stream_parser_push(adq_stream_parser_t* parser,
                           const uint8_t* data,
                           size_t len,
                           adq_frame_t* out,
                           uint32_t* frames_ready) {
    size_t i = 0;

    if (parser == 0 || data == 0 || out == 0 || frames_ready == 0) {
        return -1;
    }

    *frames_ready = 0;
    for (i = 0; i < len; ++i) {
        int frame_ready = 0;
        int rc = adq_stream_parser_push_byte(parser, data[i], out, &frame_ready);
        if (rc != 0) {
            return rc;
        }
        if (frame_ready) {
            (*frames_ready)++;
        }
    }

    return 0;
}