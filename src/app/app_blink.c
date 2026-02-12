#include <stdio.h>

#include "hal_delay.h"
#include "hal_gpio.h"
#include "hal_led.h"
#include "hal_multicore.h"

// Core 1 任務
static void core1_worker_task(void)
{
    uint32_t cmd = 0;

    while (1)
    {
        if (hal_multicore_fifo_pop(&cmd))
        {
            // 【關鍵修正】強制轉型為 (unsigned long) 搭配 %lu
            // 這能同時騙過靜態分析器，並滿足 ARM 編譯器
            printf("[Core 1] Received command: %lu\n", (unsigned long)cmd);

            hal_multicore_fifo_push(cmd * 2);
        }
    }
}

// 2. 新增：這是我們的 Callback 函式
// 當按鈕被按下時，HAL 會自動呼叫這裡！
static void on_button_press(uint32_t pin, uint32_t event)
{
    // 告訴編譯器忽略未使用的參數
    (void)event;
    // 注意：這裡是在中斷裡面，盡量不要用 printf (除非是為了 Debug)
    // 這裡示範簡單邏輯：也可以在這裡 push 到 FIFO
    printf("[App Event] Button pressed on pin %lu!\n", (unsigned long)pin);
}

// App 主程式
void app_blink_run(void)
{
    const LedDevice *led = hal_led_get_default();

    if (led && led->init)
    {
        led->init();
    }
    // 3. 新增：初始化 GPIO 並註冊 Callback
    // 假設我們用 GPIO 22 當按鈕 (BOOTSEL 也可以，但它是特殊的，我們先用普通 GPIO)
    hal_gpio_init_input(22);
    hal_gpio_set_callback(on_button_press);

    printf("[Core 0] Launching Core 1...\n");
    hal_multicore_launch(core1_worker_task);

    uint32_t counter = 0;
    uint32_t result = 0;

    while (1)
    {
        if (led && led->toggle)
        {
            led->toggle();
        }

        // 【關鍵修正】強制轉型
        printf("[Core 0] Blinking LED... Counter: %lu\n", (unsigned long)counter);

        hal_multicore_fifo_push(counter);

        if (hal_multicore_fifo_pop(&result))
        {
            // 【關鍵修正】強制轉型
            printf("[Core 0] Core 1 replied: %lu (Double of %lu)\n", (unsigned long)result,
                   (unsigned long)counter);
        }

        counter++;
        hal_delay_ms(1000);
    }
}