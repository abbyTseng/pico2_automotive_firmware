#include <stdio.h>

#include "hal/hal_dma.h"
#include "hal/hal_init.h"
#include "pico/stdlib.h"

// 準備一個超級長的字串 (模擬大量數據傳輸)
const char *long_message =
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
    "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
    "nisi ut aliquip ex ea commodo consequat.\r\n";

int main(void)
{
    // 1. 硬體初始化
    hal_init_system();  // 這裡面應該已經有 stdio_init_all()

    // 等待 USB 連線 (你剛剛修好的版本)
    printf("Waiting for USB...\n");
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }
    printf("=== Day 6 Option A: UART DMA Demo ===\n");
    sleep_ms(1000);

    // 2. 啟動 UART DMA 傳輸
    // 我們要傳送這個長字串，但【不使用 printf】
    uint32_t len = 0;
    while (long_message[len] != 0) len++;  // 算長度

    printf("Commanding DMA to send message...\n");
    hal_dma_uart_send((const uint8_t *)long_message, len);

    // 3. 證明 CPU 是自由的
    // 當 DMA 正在背景瘋狂吐字串時，CPU 在這邊做自己的事
    printf("CPU is FREE! I can do math now: ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", i);
        sleep_ms(10);  // 假裝在忙
    }
    printf("\nDone!\n");

    while (1)
    {
        sleep_ms(1000);
    }
    return 0;
}