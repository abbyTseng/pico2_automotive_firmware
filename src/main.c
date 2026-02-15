#include <stdio.h>

#include "app/app_display.h"
#include "app/app_storage.h"  // 新增：App 存儲邏輯
#include "hal/hal_init.h"
#include "hal/hal_led.h"
#include "hal/hal_storage.h"  // 新增：HAL 存儲介面
#include "pico/stdlib.h"

int main(void)
{
    // 1. 取得 LED 驅動實體
    const LedDevice *led = hal_led_get_default();

    // 2. 系統基礎初始化 (時鐘、GPIO、UART)
    hal_init_system();

    // 【關鍵修改】
    // 這是 PICO SDK 的專用函數。
    // 它會檢查 USB CDC 是否已經被電腦的 Terminal 軟體連上。
    // 如果妳沒開 Serial Monitor，程式就會停在這裡發呆。
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    // 連上後再多等一點點時間，確保字元不會亂碼
    sleep_ms(500);
    printf("\n\n=== [SYSTEM START] ===\n");
    printf("USB Console Connected!\n");

    // 3. 基礎硬體服務初始化
    if (led->init() != COMMON_OK)
    {
        printf("LED Driver Init Failed!\n");
    }

    // --- Day 10 新增：存儲系統啟動 ---
    if (hal_storage_init() == COMMON_OK)
    {
        printf("[Main] Storage HAL Initialized.\n");
        // 呼叫 App 層邏輯：紀錄一次開機計數
        if (app_storage_log_boot() == COMMON_OK)
        {
            printf("[Main] Boot logged successfully.\n");
        }
    }
    else
    {
        printf("[Main] Storage Init FAILED! Check Flash/XIP settings.\n");
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
        // 任務 A: I2C 顯示測試
        app_display_test_once();

        // 任務 B: LED 閃爍
        led->toggle();

        sleep_ms(1000);
    }
    return 0;
}