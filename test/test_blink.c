#include "app_blink.h"
#include "unity.h"

// 引入我們剛剛寫的 Mock 輔助函數 (需要宣告一下)
extern int get_init_call_count(void);
extern int get_toggle_call_count(void);
extern void reset_mock(void);

// 每個測試開始前會跑這個
void setUp(void) { reset_mock(); }

// 每個測試結束後會跑這個
void tearDown(void) {}

// --- 測試案例 1: 測試初始化邏輯 ---
void test_app_blink_init_should_call_hal_init(void) {
    // Action
    app_blink_init();

    // Assert (驗證)
    TEST_ASSERT_EQUAL_INT(1, get_init_call_count());  // 應該要呼叫一次 init
}

// --- 測試案例 2: 測試 Run 邏輯 ---
void test_app_blink_run_should_toggle_led(void) {
    // Arrange
    app_blink_init();  // 先初始化拿到指標

    // Action
    app_blink_run();

    // Assert
    TEST_ASSERT_EQUAL_INT(1, get_toggle_call_count());  // 應該要呼叫一次 toggle
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_app_blink_init_should_call_hal_init);
    RUN_TEST(test_app_blink_run_should_toggle_led);
    return UNITY_END();
}