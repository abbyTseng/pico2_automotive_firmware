// src/common/common_ringbuffer.c
#include "common_ringbuffer.h"

void common_ringbuffer_init(ring_buffer_t *rb, uint8_t *buffer, size_t size)
{
    rb->buffer = buffer;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
}

bool common_ringbuffer_push(ring_buffer_t *rb, uint8_t data)
{
    // 計算下一個寫入位置
    size_t next_head = (rb->head + 1) % rb->size;

    // 如果下一個位置是 tail，代表滿了 (保留一個空位原則)
    if (next_head == rb->tail)
    {
        return false;
    }

    rb->buffer[rb->head] = data;
    rb->head = next_head;
    return true;
}

bool common_ringbuffer_pop(ring_buffer_t *rb, uint8_t *data)
{
    // 如果頭尾相等，代表空的
    if (rb->head == rb->tail)
    {
        return false;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    return true;
}