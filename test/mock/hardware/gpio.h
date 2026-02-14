#ifndef MOCK_HARDWARE_GPIO_H
#define MOCK_HARDWARE_GPIO_H

// 引入我們剛剛修好的 mock stdlib，這樣這裡也就認識 uint 了
#include "../pico/stdlib.h"

// GPIO API (這些函式原型必須跟 Pico SDK 一模一樣，所以參數得用 uint)
void gpio_init(uint gpio);
void gpio_set_function(uint gpio, uint fn);
void gpio_set_dir(uint gpio, bool out);
void gpio_put(uint gpio, bool value);
bool gpio_get(uint gpio);
void gpio_pull_up(uint gpio);

// 中斷 Callback 型別定義
typedef void (*gpio_irq_callback_t)(uint gpio, uint32_t event_mask);
void gpio_set_irq_enabled_with_callback(uint gpio, uint32_t events, bool enabled,
                                        gpio_irq_callback_t callback);

#endif