#include <stdio.h>

#include "app/app_display.h"
#include "hal_init.h"
#include "hal_led.h"  // 引入 LED 介面
#include "pico/stdlib.h"

int main(void)
{
    // 1. 取得 LED 驅動實體
    const LedDevice *led = hal_led_get_default();

    // 2. 系統初始化
    hal_init_system();

    // 3. LED 初始化 (這會呼叫你寫的 pico_w_led_init)
    if (led->init() != COMMON_OK)
    {
        printf("LED Driver Init Failed!\n");
    }

    sleep_ms(3000);
    printf("=== Pico 2W System Started ===\n");

    // 4. OLED 初始化
    if (!app_display_init())
    {
        printf("[Main] Display Init Failed\n");
    }

    // 5. Super Loop
    while (1)
    {
        // 任務 A: I2C 顯示測試 (記得做 SDA 短路實驗!)
        app_display_test_once();

        // 任務 B: LED 閃爍 (透過介面呼叫)
        led->toggle();

        sleep_ms(1000);
    }
    return 0;
}