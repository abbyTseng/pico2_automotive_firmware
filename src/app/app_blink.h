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
void app_run(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_BLINK_H */
