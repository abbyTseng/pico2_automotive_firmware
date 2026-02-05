/**
 * @file app_blink.c
 * @brief Blink logic: app tells HAL to toggle LED and delay. No direct SDK.
 */

#include "app_blink.h"
#include "hal_led.h"
#include "hal_delay.h"
#include <stddef.h> // for NULL

// 持有 LED 裝置的指標
static const LedDevice* my_led = NULL;

void app_blink_init(void) {
    // 1. 獲取硬體介面
    my_led = hal_led_get_default();

    // 2. 防禦性檢查：指標不為 NULL 且 init 函數指標不為 NULL
    if ((my_led != NULL) && (my_led->init != NULL)) {
        (void)my_led->init(); // MISRA C: 若不檢查回傳值，建議轉型為 void 表示刻意忽略
    }
}

void app_blink_run(void) 

{
    // 3. 防禦性檢查
    if ((my_led != NULL) && (my_led->toggle != NULL)) {
        my_led->toggle();
        
        // 延遲
        hal_delay_ms(500U); // 使用 500U 表示 Unsigned 常數
    }
}