# HAL: Hardware Abstraction Layer. Wraps RP2350 SDK only. No app logic.

add_library(hal STATIC
    hal_init.c
    hal_led.c
    hal_delay.c
)

target_include_directories(hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(hal PUBLIC
    pico_stdlib
    pico_cyw43_arch_none
    hardware_gpio
)

target_link_libraries(hal PRIVATE
    common
)
