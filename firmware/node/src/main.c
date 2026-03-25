#include <stdio.h>

#include "adq_codec.h"
#include "node_app.h"

int main(void) {
    node_ctx_t ctx;
    adq_frame_t frame;
    uint8_t encoded[300];
    size_t encoded_len = 0;

    node_init(&ctx, 0x0101);
    node_on_beacon(&ctx, 1000000u, 1000100u);
    ctx.state = NODE_STREAMING;

    node_prepare_data_frame(&ctx, 1005000u, &frame);

    if (adq_encode_frame(&frame, encoded, sizeof(encoded), &encoded_len) != 0) {
        printf("encode failed\n");
        return 1;
    }

    printf("node produced frame bytes=%zu seq=%u\n", encoded_len, frame.seq);
    return 0;
}
