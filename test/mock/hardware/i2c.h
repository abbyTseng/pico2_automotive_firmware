#ifndef MOCK_HARDWARE_I2C_H
#define MOCK_HARDWARE_I2C_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// 定義 i2c_inst_t (空結構即可，只求編譯通過)
typedef struct i2c_inst i2c_inst_t;
// 定義 i2c0 指標 (轉型 0 即可)
#define i2c0 ((i2c_inst_t *)0)

// 錯誤碼 (必須對應 Pico SDK)
#define PICO_ERROR_TIMEOUT -1
#define PICO_ERROR_GENERIC -2

// I2C API
uint i2c_init(i2c_inst_t *i2c, uint baudrate);
int i2c_write_timeout_us(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop,
                         uint timeout_us);

#endif