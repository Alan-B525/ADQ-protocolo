#include <assert.h>
#include <string.h>

#include "adq_codec.h"
#include "adq_fragment.h"

static void fill_test_frame(adq_frame_t* frame) {
    size_t i = 0;

    memset(frame, 0, sizeof(*frame));
    frame->start = ADQ_FRAME_START;
    frame->proto_version = ADQ_PROTO_VERSION;
    frame->msg_type = ADQ_MSG_DATA_FRAME;
    frame->flags = 0;
    frame->node_id = 0x0110;
    frame->seq = 21;
    frame->timestamp_us = 987654321u;
    frame->payload_len = 80;
    for (i = 0; i < frame->payload_len; ++i) {
        frame->payload[i] = (uint8_t)(i & 0xFFu);
    }
}

static void test_fragment_and_reassembly_roundtrip(void) {
    adq_frame_t tx;
    adq_frame_t rx;
    adq_fragment_list_t list;
    adq_reassembly_ctx_t reasm;
    uint8_t frame_bytes[300];
    size_t frame_len = 0;
    int completed = 0;
    uint8_t i = 0;

    fill_test_frame(&tx);
    assert(adq_fragment_frame(&tx, 32, &list) == 0);
    assert(list.count > 1);

    adq_reassembly_reset(&reasm);
    for (i = 0; i < list.count; ++i) {
        assert(adq_reassembly_push(&reasm,
                                   list.fragments[i].bytes,
                                   list.fragments[i].len,
                                   frame_bytes,
                                   sizeof(frame_bytes),
                                   &frame_len,
                                   &completed) == 0);
    }

    assert(completed == 1);
    assert(adq_decode_frame(frame_bytes, frame_len, &rx) == 0);
    assert(rx.msg_type == tx.msg_type);
    assert(rx.node_id == tx.node_id);
    assert(rx.seq == tx.seq);
    assert(rx.payload_len == tx.payload_len);
    assert(memcmp(rx.payload, tx.payload, tx.payload_len) == 0);
}

int main(void) {
    test_fragment_and_reassembly_roundtrip();
    return 0;
}