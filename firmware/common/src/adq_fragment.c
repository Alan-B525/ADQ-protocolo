#include "adq_fragment.h"

#include <string.h>

#include "adq_codec.h"

int adq_fragment_frame(const adq_frame_t* frame, size_t rf_payload_max, adq_fragment_list_t* out_list) {
    uint8_t encoded[ADQ_RF_REASSEMBLY_MAX_BYTES];
    size_t encoded_len = 0;
    size_t chunk_payload = 0;
    size_t offset = 0;
    uint8_t count = 0;
    uint8_t index = 0;

    if (frame == 0 || out_list == 0) {
        return -1;
    }

    if (rf_payload_max <= ADQ_RF_FRAGMENT_OVERHEAD || rf_payload_max > ADQ_RF_FRAGMENT_MAX_PAYLOAD) {
        return -2;
    }

    if (adq_encode_frame(frame, encoded, sizeof(encoded), &encoded_len) != 0) {
        return -3;
    }

    chunk_payload = rf_payload_max - ADQ_RF_FRAGMENT_OVERHEAD;
    count = (uint8_t)((encoded_len + chunk_payload - 1u) / chunk_payload);
    if (count == 0 || count > ADQ_RF_FRAGMENT_MAX_COUNT) {
        return -4;
    }

    memset(out_list, 0, sizeof(*out_list));
    out_list->count = count;

    for (index = 0; index < count; ++index) {
        adq_rf_fragment_t* frag = &out_list->fragments[index];
        size_t remain = encoded_len - offset;
        size_t payload_len = remain < chunk_payload ? remain : chunk_payload;

        frag->bytes[0] = index;
        frag->bytes[1] = count;
        frag->bytes[2] = 0;
        if (index == 0) {
            frag->bytes[2] |= 0x01u;
        }
        if ((uint8_t)(index + 1u) == count) {
            frag->bytes[2] |= 0x02u;
        }

        memcpy(&frag->bytes[3], &encoded[offset], payload_len);
        frag->len = payload_len + ADQ_RF_FRAGMENT_OVERHEAD;
        offset += payload_len;
    }

    return 0;
}

void adq_reassembly_reset(adq_reassembly_ctx_t* ctx) {
    if (ctx == 0) {
        return;
    }

    memset(ctx, 0, sizeof(*ctx));
}

int adq_reassembly_push(adq_reassembly_ctx_t* ctx,
                        const uint8_t* fragment,
                        size_t fragment_len,
                        uint8_t* out_frame,
                        size_t out_cap,
                        size_t* out_frame_len,
                        int* completed) {
    uint8_t frag_index = 0;
    uint8_t frag_count = 0;
    size_t payload_len = 0;

    if (ctx == 0 || fragment == 0 || out_frame == 0 || out_frame_len == 0 || completed == 0) {
        return -1;
    }

    if (fragment_len <= ADQ_RF_FRAGMENT_OVERHEAD) {
        return -2;
    }

    frag_index = fragment[0];
    frag_count = fragment[1];
    payload_len = fragment_len - ADQ_RF_FRAGMENT_OVERHEAD;
    *completed = 0;

    if (frag_count == 0 || frag_count > ADQ_RF_FRAGMENT_MAX_COUNT) {
        return -3;
    }

    if (!ctx->in_progress) {
        if ((fragment[2] & 0x01u) == 0u || frag_index != 0u) {
            return -4;
        }
        ctx->in_progress = 1;
        ctx->expected_count = frag_count;
        ctx->received_count = 0;
        ctx->total_len = 0;
    }

    if (frag_count != ctx->expected_count) {
        return -5;
    }

    if (frag_index != ctx->received_count) {
        return -6;
    }

    if (ctx->total_len + payload_len > sizeof(ctx->data)) {
        return -7;
    }

    memcpy(&ctx->data[ctx->total_len], &fragment[3], payload_len);
    ctx->total_len += payload_len;
    ctx->received_count++;

    if (ctx->received_count == ctx->expected_count) {
        if (out_cap < ctx->total_len) {
            return -8;
        }

        memcpy(out_frame, ctx->data, ctx->total_len);
        *out_frame_len = ctx->total_len;
        *completed = 1;
        adq_reassembly_reset(ctx);
    }

    return 0;
}