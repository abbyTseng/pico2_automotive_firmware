# App: Business logic. Hardware agnostic. No direct SDK calls.

add_library(app STATIC
    app_blink.c
    app_display.c
    app_storage.c
)

target_include_directories(app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# App depends on HAL (and common) only; never on pico_stdlib or hardware_*
target_link_libraries(app PUBLIC
    hal
    common
)
