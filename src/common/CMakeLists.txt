# Common: shared types and utilities. No SDK, no app logic.

add_library(common INTERFACE)

target_include_directories(common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(common INTERFACE
    common_types.h
    common_status.h
)
