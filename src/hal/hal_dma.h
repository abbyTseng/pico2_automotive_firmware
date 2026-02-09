#ifndef HAL_DMA_H
#define HAL_DMA_H

#include <stdbool.h>
#include <stdint.h>

// 初始化 DMA (目前留空，預留擴充用)
void hal_dma_init(void);

// [新增] 專門用來透過 UART 發送資料的 DMA 函式
// data: 要發送的字串/陣列
// len: 長度
void hal_dma_uart_send(const uint8_t *data, uint32_t len);

// 檢查搬運是否還在忙碌中
bool hal_dma_is_busy(int channel);

#endif  // HAL_DMA_H