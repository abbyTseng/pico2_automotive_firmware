/**
 * @file hal_delay.h
 * @brief Delay abstraction. App uses this instead of SDK sleep.
 */

#ifndef HAL_DELAY_H
#define HAL_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Block for approximately ms milliseconds.
 * @param ms Delay in milliseconds
 */
void hal_delay_ms(unsigned int ms);

#ifdef __cplusplus
}
#endif

#endif /* HAL_DELAY_H */
