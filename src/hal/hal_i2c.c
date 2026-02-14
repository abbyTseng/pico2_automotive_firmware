#include "hal_i2c.h"

#include "hardware/i2c.h"
#include "pico/stdlib.h"

/* * 靜態配置：在實際專案中，這些通常會定義在 board_config.h
 * 這裡為了 Day 9 示範直接定義
 */
#define I2C_INST i2c0
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5
#define BUS_RECOVERY_CLOCKS 9
#define RECOVERY_DELAY_US 5

/**
 * @brief 強制執行 I2C Bus Recovery (9 Clocks Mechanism)
 * 當 Slave 設備卡死在 SDA = LOW 時，透過 Bit-banging SCL 讓其釋放 Bus。
 */
static void hal_i2c_bus_recovery(void)
{
    // 1. 脫離 I2C 硬體控制，切換回 GPIO 模式
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_SIO);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_SIO);

    // 2. 配置方向：SCL 輸出(控制脈衝), SDA 輸入(監測狀態)
    gpio_set_dir(I2C_SCL_PIN, GPIO_OUT);
    gpio_set_dir(I2C_SDA_PIN, GPIO_IN);

    // 3. 執行最多 9 個脈衝
    for (uint8_t i = 0; i < BUS_RECOVERY_CLOCKS; i++)
    {
        // 檢查 SDA 是否已被釋放 (變回 HIGH)
        if (gpio_get(I2C_SDA_PIN))
        {
            break;
        }

        // 手動切換 SCL (Bit-banging)
        gpio_put(I2C_SCL_PIN, 0);
        sleep_us(RECOVERY_DELAY_US);
        gpio_put(I2C_SCL_PIN, 1);
        sleep_us(RECOVERY_DELAY_US);
    }

    // 4. 發送一個手動 STOP 訊號確保匯流排重置
    gpio_set_dir(I2C_SDA_PIN, GPIO_OUT);
    gpio_put(I2C_SDA_PIN, 0);
    sleep_us(RECOVERY_DELAY_US);
    gpio_put(I2C_SCL_PIN, 1);
    sleep_us(RECOVERY_DELAY_US);
    gpio_put(I2C_SDA_PIN, 1);  // SDA rising while SCL is high = STOP
    sleep_us(RECOVERY_DELAY_US);

    // 5. 恢復 I2C 硬體功能
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    // 重新初始化硬體暫存器，確保狀態乾淨
    i2c_init(I2C_INST, 100 * 1000);  // 預設 100kHz
}

void hal_i2c_init(uint32_t baudrate)
{
    // 初始化硬體實例
    i2c_init(I2C_INST, baudrate);

    // 設定 GPIO Pull-up (汽車電子環境通常需要外部強拉電阻，但內部也要開啟作為保護)
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

hal_i2c_status_t hal_i2c_write_timeout(uint8_t addr, const uint8_t *src, size_t len,
                                       uint32_t timeout_us)
{
    if (src == NULL || len == 0)
    {
        return HAL_I2C_ERROR;
    }

    // 調用 Pico SDK 的帶逾時寫入 API
    // 返回值 > 0 為寫入位元組數，PICO_ERROR_TIMEOUT 為逾時
    int result = i2c_write_timeout_us(I2C_INST, addr, src, len, false, timeout_us);

    if (result == PICO_ERROR_TIMEOUT)
    {
        // 核心邏輯：一旦逾時，立刻執行恢復程序
        hal_i2c_bus_recovery();
        return HAL_I2C_TIMEOUT;
    }
    else if (result < 0)
    {
        return HAL_I2C_ERROR;
    }

    return HAL_I2C_OK;
}