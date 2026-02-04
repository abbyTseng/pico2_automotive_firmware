/**
 * @file main.c
 * @brief Entry point: initializes system and calls application. No app logic here.
 */

#include "hal/hal_init.h"
#include "app/app_blink.h"

int main(void)
{
    hal_init();
    app_run();
    /* never reached in blink example */
    return 0;
}
