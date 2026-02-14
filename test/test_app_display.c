#include "app_display.c"
#include "mock_hal_i2c.h"  // 需要建立 Mock HAL
#include "unity.h"

void test_oled_init_should_retry_on_timeout(void)
{
    // 1. 模擬第一次寫入回傳 TIMEOUT
    hal_i2c_write_timeout_ExpectAndReturn(0x3C, NULL, 9, 50000, HAL_I2C_TIMEOUT);
    hal_i2c_write_timeout_IgnoreArg_src();  // 忽略內容比對

    // 2. 模擬第二次（Recovery 後）寫入回傳 OK
    hal_i2c_write_timeout_ExpectAndReturn(0x3C, NULL, 9, 50000, HAL_I2C_OK);
    hal_i2c_write_timeout_IgnoreArg_src();

    bool result = init_oled();

    TEST_ASSERT_TRUE(result);
}