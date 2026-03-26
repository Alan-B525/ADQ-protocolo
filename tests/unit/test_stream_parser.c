#include <assert.h>
#include <string.h>

#include "adq_codec.h"
#include "adq_stream_parser.h"

static void fill_heartbeat(adq_frame_t* frame) {
    memset(frame, 0, sizeof(*frame));
    frame->start = ADQ_FRAME_START;
    frame->proto_version = ADQ_PROTO_VERSION;
    frame->msg_type = ADQ_MSG_HEARTBEAT;
    frame->flags = 0;
    frame->node_id = 0x0101;
    frame->seq = 3;
    frame->timestamp_us = 111222333u;
    frame->payload_len = 0;
}

static void test_parser_skips_noise_and_decodes_frame(void) {
    adq_stream_parser_t parser;
    adq_frame_t tx;
    adq_frame_t rx;
    uint8_t bytes[300];
    size_t len = 0;
    uint8_t with_noise[304];
    uint32_t frames_ready = 0;

    fill_heartbeat(&tx);
    assert(adq_encode_frame(&tx, bytes, sizeof(bytes), &len) == 0);

    with_noise[0] = 0x00u;
    with_noise[1] = 0xFFu;
    with_noise[2] = 0x11u;
    with_noise[3] = 0x22u;
    memcpy(&with_noise[4], bytes, len);

    adq_stream_parser_init(&parser);
    assert(adq_stream_parser_push(&parser, with_noise, 4 + len, &rx, &frames_ready) == 0);
    assert(frames_ready == 1u);
    assert(rx.msg_type == ADQ_MSG_HEARTBEAT);
    assert(rx.node_id == 0x0101u);
    assert(rx.seq == 3u);
}

int main(void) {
    test_parser_skips_noise_and_decodes_frame();
    return 0;
}