/**
 * @file main.c
 * @brief Entry point: initializes system and calls application. No app logic here.
 */

#include "app/app_blink.h"
#include "hal/hal_init.h"

int main(void) {
// 1. 系統硬體初始化
    hal_init_system();

    // 2. 應用層初始化
    app_blink_init();

    // 3. 主迴圈
    while (1) {
        app_blink_run();
    }
    return 0;
}
