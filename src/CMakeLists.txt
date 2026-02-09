# Application sources: main + common, hal, app libraries

add_subdirectory(common)
add_subdirectory(hal)
add_subdirectory(app)

# -----------------------------------------------------------------------------
# Main executable (entry point only: init system, call app)
# -----------------------------------------------------------------------------
add_executable(${PROJECT_NAME}_main main.c)

pico_enable_stdio_usb(${PROJECT_NAME}_main 1)
pico_enable_stdio_uart(${PROJECT_NAME}_main 0)

target_include_directories(${PROJECT_NAME}_main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/app
)

target_link_libraries(${PROJECT_NAME}_main
    pico_stdlib
    common
    hal
    app
)

# Create UF2 for Pico 2
pico_add_extra_outputs(${PROJECT_NAME}_main)
