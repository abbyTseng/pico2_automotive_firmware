#include "hal_dma.h"

#include <stddef.h>

#include "hardware/dma.h"
#include "hardware/sync.h"
#include "hardware/uart.h"

// 我們預設使用 UART0 (Pico 的預設 printf 通道)
#define UART_ID uart0
#define UART_DMA_DREQ DREQ_UART0_TX

// 暫存 DMA 通道編號 (-1 代表還沒申請)
static int uart_dma_chan = -1;

void hal_dma_init(void)
{
    // 這裡保留給未來全域初始化使用
}

void hal_dma_uart_send(const uint8_t *data, uint32_t len)
{
    // 1. 如果還沒申請過通道，就向系統要一個沒人用的
    if (uart_dma_chan < 0)
    {
        uart_dma_chan = dma_claim_unused_channel(true);
    }

    // 取得預設設定檔
    dma_channel_config c = dma_channel_get_default_config(uart_dma_chan);

    // A. 傳輸單位：改成 8-bit (1 byte)，因為 UART 是一次傳一個字元
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    // B. 來源地址：要增加 (data++, 把字串一個個讀出來)
    channel_config_set_read_increment(&c, true);
    // C. 目標地址：【不要增加】！
    // 我們要一直塞給同一台「傳真機入口」(UART TX Register)
    channel_config_set_write_increment(&c, false);
    // D. 節奏控制 (Pacing/DREQ)：【最重要的一行】
    // 告訴 DMA：「不要急，等 UART 說 OK 你再塞下一個」
    // 如果沒設這行，DMA 會在一微秒內把 1000 個字塞爆 UART，資料會亂掉
    channel_config_set_dreq(&c, UART_DMA_DREQ);

    // 2. 處理 Cache Coherence (快取一致性)
    // 在 RP2350/Cortex-M33，為了安全起見，我們確保記憶體寫入完成
    // __dmb() 是 "Data Memory Barrier"，像是一個檢查哨
    __dmb();

    dma_channel_configure(uart_dma_chan, &c,
                          &uart0_hw->dr,  // 目標：UART 的資料暫存器 (固定地址)
                          data,           // 來源：你的字串 buffer
                          len,            // 長度
                          true            // Start!
    );
}

bool hal_dma_is_busy(int channel) { return dma_channel_is_busy(channel); }