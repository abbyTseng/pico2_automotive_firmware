# HAL: Hardware Abstraction Layer. Wraps RP2350 SDK only. No app logic.

add_library(hal STATIC
    hal_init.c
    hal_led.c
    hal_delay.c
    hal_dma.c
    hal_multicore.c
    hal_gpio.c
)

target_include_directories(hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(hal PUBLIC
    pico_stdlib
    pico_cyw43_arch_none
    hardware_gpio
    hardware_dma
    pico_multicore
    common
)

