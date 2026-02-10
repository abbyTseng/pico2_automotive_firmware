#ifndef HAL_MULTICORE_H
#define HAL_MULTICORE_H

#include <stdbool.h>
#include <stdint.h>

// 定義核心 1 要執行的任務函式指標
typedef void (*core1_task_t)(void);

/**
 * @brief 啟動核心 1 並執行指定任務
 * @param task 核心 1 的入口函式
 */
void hal_multicore_launch(core1_task_t task);

/**
 * @brief 推送數據到 FIFO (給另一個核心) - 阻塞模式
 * @param data 要傳送的 32-bit 數據
 */
void hal_multicore_fifo_push(uint32_t data);

/**
 * @brief 從 FIFO 讀取數據 (來自另一個核心) - 阻塞模式
 * @param output_data 讀到的數據指標
 * @return true 成功, false 失敗
 */
bool hal_multicore_fifo_pop(uint32_t *output_data);

/**
 * @brief 檢查 FIFO 是否有資料
 */
bool hal_multicore_fifo_has_data(void);

#endif  // HAL_MULTICORE_H