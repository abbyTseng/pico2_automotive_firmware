/**
 * @file hal_led.c
 * @brief LED HAL implementation for Pico 2 W (Wireless).
 * Uses CYW43 architecture instead of standard GPIO.
 */

#include "hal_led.h"

#include "pico/cyw43_arch.h"  // <--- 關鍵改變：引入無線架構庫
#include "pico/stdlib.h"

static bool hal_led_initialized = false;
static bool current_led_state = false;  // 用來記錄 LED 狀態以便 Toggle

common_status_t hal_led_init(void) {
    if (hal_led_initialized) {
        return COMMON_OK;
    }

    // <--- 關鍵改變：初始化無線晶片，而不是 gpio_init
    // 這會啟動板載電源管理與無線模組
    if (cyw43_arch_init()) {
        // 如果初始化失敗 (回傳非 0)，代表硬體有問題
        return COMMON_ERR;
    }

    hal_led_initialized = true;
    return COMMON_OK;
}

void hal_led_toggle(void) {
    if (!hal_led_initialized) {
        return;
    }

    // 切換狀態
    current_led_state = !current_led_state;

    // <--- 關鍵改變：透過無線晶片控制 LED (CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, current_led_state);
}