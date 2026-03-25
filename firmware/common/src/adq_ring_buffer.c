#include "adq_ring_buffer.h"

void adq_rb_init(adq_ring_buffer_t* rb, uint8_t* backing, size_t capacity) {
    rb->data = backing;
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

int adq_rb_push(adq_ring_buffer_t* rb, uint8_t byte) {
    if (rb->count >= rb->capacity) {
        return -1;
    }

    rb->data[rb->head] = byte;
    rb->head = (rb->head + 1u) % rb->capacity;
    rb->count++;
    return 0;
}

int adq_rb_pop(adq_ring_buffer_t* rb, uint8_t* out) {
    if (rb->count == 0) {
        return -1;
    }

    *out = rb->data[rb->tail];
    rb->tail = (rb->tail + 1u) % rb->capacity;
    rb->count--;
    return 0;
}

size_t adq_rb_size(const adq_ring_buffer_t* rb) {
    return rb->count;
}

size_t adq_rb_free(const adq_ring_buffer_t* rb) {
    return rb->capacity - rb->count;
}
