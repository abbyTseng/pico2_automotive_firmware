/**
 * @file main.c
 * @brief Entry point: initializes system and calls application. No app logic here.
 */

#include "app/app_blink.h"
#include "hal/hal_init.h"

int main(void) {
    hal_init();
    app_run();
    /* never reached in blink example */
    return 0;
}
