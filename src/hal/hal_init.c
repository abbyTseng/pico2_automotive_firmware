/**
 * @file hal_init.c
 * @brief HAL init implementation. Only place that calls SDK stdio_init.
 */

#include "hal_init.h"

#include "pico/stdlib.h"

void hal_init_system(void) {
    stdio_init_all();
}
