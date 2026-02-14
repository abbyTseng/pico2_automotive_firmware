#ifndef MOCK_PICO_STDLIB_H
#define MOCK_PICO_STDLIB_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// --------------------------------------------------------------------------
// [關鍵修正] 定義 SDK 專用的非標準型別
// 雖然 MISRA C 不喜歡 uint，但為了 Mock 官方 SDK 的函式原型，我們必須定義它
// --------------------------------------------------------------------------
typedef unsigned int uint;

// 模擬 GPIO 功能常數
#define GPIO_FUNC_I2C 1
#define GPIO_FUNC_SIO 5
#define GPIO_OUT 1
#define GPIO_IN 0

// 中斷觸發定義 (補齊 Day 8 缺少的定義)
#define GPIO_IRQ_EDGE_FALL 0x04
#define GPIO_IRQ_EDGE_RISE 0x08

// 模擬時間延遲
void sleep_us(uint64_t us);
void sleep_ms(uint32_t ms);

#endif