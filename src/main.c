#include "hal/hal_init.h"

// 宣告外部函式 (這個函式定義在 src/app/app_blink.c)
// 在正式專案中，這行通常會放在 app.h 裡，但現在我們先簡單處理
void app_blink_run(void);

int main(void)
{
    // 1. 硬體底層初始化 (包含 stdio, USB 等)
    hal_init_system();

    // 2. 直接跳進 App 層的無限迴圈
    // 從這裡開始，就是 Day 7 雙核心邏輯的主場
    app_blink_run();

    // 理論上程式不會跑到這裡，因為 app_blink_run 裡面有 while(1)
    return 0;
}