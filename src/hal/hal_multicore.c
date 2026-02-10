#include "hal_multicore.h"

#include "pico/multicore.h"

void hal_multicore_launch(core1_task_t task)
{
    // 呼叫 SDK 啟動核心 1
    multicore_launch_core1(task);
}

void hal_multicore_fifo_push(uint32_t data)
{
    // 阻塞直到 FIFO 有空間
    multicore_fifo_push_blocking(data);
}

bool hal_multicore_fifo_pop(uint32_t *output_data)
{
    if (output_data == NULL) return false;

    // 阻塞直到 FIFO 有資料
    *output_data = multicore_fifo_pop_blocking();
    return true;
}

bool hal_multicore_fifo_has_data(void) { return multicore_fifo_rvalid(); }