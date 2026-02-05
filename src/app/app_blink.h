/**
 * @file app_blink.h
 * @brief Application entry: blink LED. Uses HAL only; no SDK.
 */

#ifndef APP_BLINK_H
#define APP_BLINK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Application entry. Initializes LED via HAL and runs blink loop.
 * Called once from main after hal_init().
 */
// 應用層初始化：獲取 HAL 介面
void app_blink_init(void);

// 應用層主邏輯：執行閃爍與延遲
void app_blink_run(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_BLINK_H */
