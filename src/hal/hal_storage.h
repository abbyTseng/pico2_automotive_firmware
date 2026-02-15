#ifndef HAL_STORAGE_H
#define HAL_STORAGE_H

#include <stddef.h>
#include <stdint.h>

#include "common/common_status.h"  // 確保拼字正確

// 統一使用 common_status_t
common_status_t hal_storage_init(void);
common_status_t hal_storage_write_file(const char *path, const uint8_t *data, size_t size);
common_status_t hal_storage_read_file(const char *path, uint8_t *buffer, size_t size);

#endif