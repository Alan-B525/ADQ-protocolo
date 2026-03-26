#ifndef ADQ_STREAM_PARSER_H
#define ADQ_STREAM_PARSER_H

#include <stddef.h>
#include <stdint.h>

#include "adq_protocol.h"

#define ADQ_STREAM_BUFFER_MAX 300u

typedef enum {
    ADQ_STREAM_SEARCH_START = 0,
    ADQ_STREAM_COLLECT_HEADER,
    ADQ_STREAM_COLLECT_PAYLOAD
} adq_stream_state_t;

typedef struct {
    adq_stream_state_t state;
    uint8_t buffer[ADQ_STREAM_BUFFER_MAX];
    size_t index;
    size_t expected_total;
} adq_stream_parser_t;

void adq_stream_parser_init(adq_stream_parser_t* parser);
void adq_stream_parser_reset(adq_stream_parser_t* parser);
int adq_stream_parser_push_byte(adq_stream_parser_t* parser, uint8_t byte, adq_frame_t* out, int* frame_ready);
int adq_stream_parser_push(adq_stream_parser_t* parser,
                           const uint8_t* data,
                           size_t len,
                           adq_frame_t* out,
                           uint32_t* frames_ready);

#endif