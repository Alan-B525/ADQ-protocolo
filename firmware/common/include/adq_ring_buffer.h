#ifndef ADQ_RING_BUFFER_H
#define ADQ_RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t* data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} adq_ring_buffer_t;

void adq_rb_init(adq_ring_buffer_t* rb, uint8_t* backing, size_t capacity);
int adq_rb_push(adq_ring_buffer_t* rb, uint8_t byte);
int adq_rb_pop(adq_ring_buffer_t* rb, uint8_t* out);
size_t adq_rb_size(const adq_ring_buffer_t* rb);
size_t adq_rb_free(const adq_ring_buffer_t* rb);

#endif
