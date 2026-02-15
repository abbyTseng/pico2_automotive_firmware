#ifndef APP_STORAGE_DEMO_H
#define APP_STORAGE_DEMO_H

#include "common_status.h"

/**
 * @brief 紀錄一次系統啟動
 * 邏輯：讀取 boot_count -> +1 -> 寫回
 */
common_status_t app_storage_log_boot(void);

#endif