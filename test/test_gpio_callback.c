#include "unity.h"

// =========================================================
// 💡 白箱測試技巧
// 直接 Include 原始碼 (.c)，這樣我們就能存取 static 變數
// =========================================================
#include "../src/hal/hal_gpio.c"

// 模擬 App 層的 Callback 函式 (Mock)
volatile int callback_triggered_count = 0;

void mock_app_handler(uint32_t gpio, uint32_t events)
{
    (void)gpio;
    (void)events;
    callback_triggered_count++;
}

// Unity Setup (每個測試前重置狀態)
void setUp(void)
{
    _app_callback = NULL;  // 重置 static 變數
    callback_triggered_count = 0;
}

// Unity Teardown
void tearDown(void) {}

// --- 測試案例 ---

// 測試 1: 檢查 set_callback 是否正確儲存函式指標
void test_hal_gpio_set_callback_should_store_function_pointer(void)
{
    // Act
    hal_gpio_set_callback(mock_app_handler);

    // Assert: 檢查 static 變數是否等於我們傳進去的函式
    TEST_ASSERT_EQUAL_PTR(mock_app_handler, _app_callback);
}

// 測試 2: 模擬硬體中斷觸發 ISR，檢查是否轉發給 App
void test_internal_isr_should_trigger_app_callback(void)
{
    // Arrange
    hal_gpio_set_callback(mock_app_handler);

    // Act: 直接呼叫 static ISR (模擬硬體行為)
    _internal_gpio_isr(22, 0x04);  // 0x04 = GPIO_IRQ_EDGE_FALL

    // Assert
    TEST_ASSERT_EQUAL_INT(1, callback_triggered_count);
}

// 測試 3: 如果沒註冊 Callback，ISR 不應該當機或亂叫
void test_internal_isr_should_do_nothing_if_no_callback(void)
{
    // Arrange
    _app_callback = NULL;

    // Act
    _internal_gpio_isr(22, 0x04);

    // Assert
    TEST_ASSERT_EQUAL_INT(0, callback_triggered_count);
}

// 程式入口
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_hal_gpio_set_callback_should_store_function_pointer);
    RUN_TEST(test_internal_isr_should_trigger_app_callback);
    RUN_TEST(test_internal_isr_should_do_nothing_if_no_callback);
    return UNITY_END();
}