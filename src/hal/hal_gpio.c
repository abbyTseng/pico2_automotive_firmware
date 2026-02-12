#include "hal_gpio.h"

#include "hardware/gpio.h"
#include "pico/stdlib.h"

// 私有變數：儲存 App 傳進來的函式指標
static hal_gpio_callback_t _app_callback = NULL;

// 這是 Pico SDK 真正的 ISR，我們把它轉發給 App
static void _internal_gpio_isr(uint gpio, uint32_t events)
{
    if (_app_callback != NULL)
    {
        _app_callback((uint32_t)gpio, (uint32_t)events);
    }
}

void hal_gpio_init_input(uint32_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);

    // 設定硬體中斷 (這裡假設是 Falling Edge 觸發)
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &_internal_gpio_isr);
}

void hal_gpio_set_callback(hal_gpio_callback_t cb) { _app_callback = cb; }