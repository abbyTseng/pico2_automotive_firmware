#include "app_display.h"

#include <stdio.h>

#include "hal_i2c.h"  // App 層呼叫 HAL 層，這是合法的
#include "pico/stdlib.h"

#define OLED_ADDR 0x3C

// OLED 初始化指令
const uint8_t OLED_INIT_CMD[] = {0x00, 0xAE, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 0xAF};

bool app_display_init(void)
{
    // [修正點]：將 I2C 初始化移到這裡
    // App 宣告：「我要用 I2C，請幫我準備好」
    hal_i2c_init(100 * 1000);

    // 接著執行 OLED 自己的初始化
    hal_i2c_status_t status =
        hal_i2c_write_timeout(OLED_ADDR, OLED_INIT_CMD, sizeof(OLED_INIT_CMD), 50000);

    if (status != HAL_I2C_OK)
    {
        printf("[App] OLED Init Failed!\n");
        return false;
    }
    printf("[App] OLED Init Success.\n");
    return true;
}

void app_display_test_once(void)
{
    // ... (維持原樣)
    const uint8_t dummy_cmd[] = {0x00, 0xE3};
    hal_i2c_status_t status = hal_i2c_write_timeout(OLED_ADDR, dummy_cmd, 2, 10000);

    if (status == HAL_I2C_TIMEOUT)
    {
        printf("[App] I2C Timeout! (Recovery triggered)\n");
    }
    else if (status == HAL_I2C_OK)
    {
        printf("[App] I2C OK.\n");
    }
}