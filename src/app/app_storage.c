#include "app_storage.h"

#include <stdint.h>
#include <stdio.h>

#include "common/common_status.h"
#include "hal/hal_storage.h"

#define BOOT_COUNT_FILE "sys_boot.bin"

common_status_t app_storage_log_boot(void)
{
    uint32_t count = 0;

    // 1. 嘗試讀取現有的開機次數
    // (void) 轉型是因為若檔案不存在，讀取失敗是正常的，我們直接用 count = 0 開始
    (void)hal_storage_read_file(BOOT_COUNT_FILE, (uint8_t *)&count, sizeof(count));

    count++;
    printf("[App Storage] Current Boot Count: %u\n", (unsigned int)count);

    // 2. 寫回新的計數
    return hal_storage_write_file(BOOT_COUNT_FILE, (uint8_t *)&count, sizeof(count));
}