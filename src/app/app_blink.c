/**
 * @file app_blink.c
 * @brief Blink logic: app tells HAL to toggle LED and delay. No direct SDK.
 */

#include "app_blink.h"

#include "common_status.h"
#include "hal_delay.h"
#include "hal_led.h"

#define BLINK_MS 500

void app_run(void) {
    if (hal_led_init() != COMMON_OK) {
        return;
    }

    for (;;) {
        hal_led_toggle();
        hal_delay_ms(BLINK_MS);
    }
}
