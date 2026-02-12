#ifndef HARDWARE_GPIO_H
#define HARDWARE_GPIO_H

#include "pico/stdlib.h"

// 定義一些你在 hal_gpio.c 用到的常數
#define GPIO_IN 0
#define GPIO_OUT 1
#define GPIO_IRQ_EDGE_FALL 0x4
#define GPIO_IRQ_EDGE_RISE 0x8

// 定義 Callback 型別 (必須跟 SDK 一樣)
typedef void (*gpio_irq_callback_t)(uint gpio, uint32_t events);

// 宣告假的函式 (Mock Functions)，讓 Linker 找得到人
// 這裡用 static inline 或 weak 都可以，CI 只要能編譯過就好
static inline void gpio_init(uint gpio) {}
static inline void gpio_set_dir(uint gpio, bool out) {}
static inline void gpio_pull_up(uint gpio) {}
static inline void gpio_set_irq_enabled_with_callback(uint gpio, uint32_t events, bool enabled,
                                                      gpio_irq_callback_t callback)
{
}

#endif