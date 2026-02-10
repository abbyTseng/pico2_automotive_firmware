#include <stdio.h>

#include "hal_delay.h"
#include "hal_led.h"
#include "hal_multicore.h"

// 修正 2: 加入 static 關鍵字，因為這個函式只在這個檔案內使用
static void core1_worker_task(void)
{
    uint32_t cmd = 0;

    while (1)
    {
        if (hal_multicore_fifo_pop(&cmd))
        {
            // 修正 1: 將 %lu 改為 %u
            printf("[Core 1] Received command: %u\n", cmd);

            hal_multicore_fifo_push(cmd * 2);
        }
    }
}

void app_blink_run(void)
{
    // 取得 LED 裝置物件
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

        // 修正 1: 將 %lu 改為 %u
        printf("[Core 0] Blinking LED... Counter: %u\n", counter);

        hal_multicore_fifo_push(counter);

        if (hal_multicore_fifo_pop(&result))
        {
            // 修正 1: 將 %lu 改為 %u
            printf("[Core 0] Core 1 replied: %u (Double of %u)\n", result, counter);
        }

        counter++;
        hal_delay_ms(1000);
    }
}