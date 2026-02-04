/**
 * @file hal_led.h
 * @brief LED abstraction. App uses this; no direct SDK in app.
 */

#ifndef HAL_LED_H
#define HAL_LED_H

#include "common_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize LED GPIO (board LED). Idempotent.
 * @return COMMON_OK or COMMON_ERR
 */
common_status_t hal_led_init(void);

/**
 * Toggle board LED state.
 */
void hal_led_toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_LED_H */
