#include "common_types.h"
#include "unity.h"

static bool callback_called = false;
static uint32_t last_val = 0;

void mock_app_handler(uint32_t id, uint32_t val)
{
    callback_called = true;
    last_val = val;
}

void test_callback_should_be_triggered_by_hal(void)
{
    // 1. 註冊
    hal_gpio_register_callback(mock_app_handler);

    // 2. 模擬 HAL 收到硬體中斷 (手動呼叫內部 handler)
    // 在測試環境中，我們直接呼叫被測試的邏輯
    _gpio_internal_handler(2, 0x01);

    // 3. 驗證
    TEST_ASSERT_TRUE(callback_called);
    TEST_ASSERT_EQUAL(0x01, last_val);
}