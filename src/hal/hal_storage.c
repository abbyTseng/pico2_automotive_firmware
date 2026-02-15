#include "hal_storage.h"

#include <string.h>

#include "hardware/flash.h"
#include "hardware/sync.h"
#include "lfs.h"
#include "pico/stdlib.h"

/* Flash 佈局配置 */
#define FS_FLASH_OFFSET (2 * 1024 * 1024)
#define FS_SIZE (1 * 1024 * 1024)

static struct lfs lfs_inst;
static struct lfs_config cfg;

/* --- 內部轉換函數：將 LFS Error 轉為 common_status_t --- */
static common_status_t _lfs_err_to_status(int lfs_err)
{
    if (lfs_err >= 0)
    {
        return COMMON_OK;
    }
    return COMMON_ERR;
}

/* --- 底層轉接函數 (Internal Callback) --- */
static int hal_lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer,
                        lfs_size_t size)
{
    uint32_t flash_addr = FS_FLASH_OFFSET + (block * c->block_size) + off;
    memcpy(buffer, (const uint8_t *)(XIP_BASE + flash_addr), size);
    return LFS_ERR_OK;
}

static int hal_lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                        const void *buffer, lfs_size_t size)
{
    uint32_t flash_addr = FS_FLASH_OFFSET + (block * c->block_size) + off;
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(flash_addr, (const uint8_t *)buffer, size);
    restore_interrupts(ints);
    return LFS_ERR_OK;
}

static int hal_lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    uint32_t flash_addr = FS_FLASH_OFFSET + (block * c->block_size);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(flash_addr, c->block_size);
    restore_interrupts(ints);
    return LFS_ERR_OK;
}

static int hal_lfs_sync(const struct lfs_config *c)
{
    (void)c;  // 解決 unused parameter 警告
    return LFS_ERR_OK;
}

/* --- 公開 API 實作 --- */

common_status_t hal_storage_init(void)
{
    memset(&cfg, 0, sizeof(cfg));
    cfg.read = hal_lfs_read;
    cfg.prog = hal_lfs_prog;
    cfg.erase = hal_lfs_erase;
    cfg.sync = hal_lfs_sync;

    cfg.read_size = 16;
    cfg.prog_size = FLASH_PAGE_SIZE;
    cfg.block_size = FLASH_SECTOR_SIZE;
    cfg.block_count = FS_SIZE / FLASH_SECTOR_SIZE;
    cfg.cache_size = FLASH_PAGE_SIZE;
    cfg.lookahead_size = 32;
    cfg.block_cycles = 500;

    int err = lfs_mount(&lfs_inst, &cfg);
    if (err < 0)
    {
        lfs_format(&lfs_inst, &cfg);
        err = lfs_mount(&lfs_inst, &cfg);
    }
    return _lfs_err_to_status(err);
}

common_status_t hal_storage_write_file(const char *path, const uint8_t *data, size_t size)
{
    lfs_file_t file;
    int err = lfs_file_open(&lfs_inst, &file, path, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
    if (err < 0) return COMMON_ERR;

    lfs_ssize_t written = lfs_file_write(&lfs_inst, &file, data, size);
    int close_err = lfs_file_close(&lfs_inst, &file);

    if (written < 0 || (size_t)written != size || close_err < 0)
    {
        return COMMON_ERR;
    }
    return COMMON_OK;
}

common_status_t hal_storage_read_file(const char *path, uint8_t *buffer, size_t size)
{
    lfs_file_t file;
    int err = lfs_file_open(&lfs_inst, &file, path, LFS_O_RDONLY);
    if (err < 0) return COMMON_ERR;

    lfs_ssize_t read_len = lfs_file_read(&lfs_inst, &file, buffer, size);
    lfs_file_close(&lfs_inst, &file);

    if (read_len < 0 || (size_t)read_len != size)
    {
        return COMMON_ERR;
    }
    return COMMON_OK;
}