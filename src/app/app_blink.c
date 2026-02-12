#include <stdio.h>

#include "hal_delay.h"
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

// App 主程式
void app_blink_run(void)
{
    const LedDevice *led = hal_led_get_default();

    if (led && led->init)
    {
        led->init();
    }

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