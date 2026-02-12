// src/common/common_ringbuffer.h
#ifndef COMMON_RINGBUFFER_H
#define COMMON_RINGBUFFER_H

#include <stdbool.h>
#include <stddef.h>  // for size_t
#include <stdint.h>

typedef struct
{
    uint8_t *buffer;
    size_t size;  // 總容量
    size_t head;  // 寫入點
    size_t tail;  // 讀取點
} ring_buffer_t;

// 初始化
void common_ringbuffer_init(ring_buffer_t *rb, uint8_t *buffer, size_t size);

// 推入 (回傳 true 代表成功，false 代表滿了)
bool common_ringbuffer_push(ring_buffer_t *rb, uint8_t data);

// 彈出 (回傳 true 代表成功，false 代表空的)
bool common_ringbuffer_pop(ring_buffer_t *rb, uint8_t *data);

#endif