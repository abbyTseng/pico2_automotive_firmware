# RP2350 (Pico 2) Firmware - Root CMakeLists.txt
# Strict layered architecture: HAL | App | Common

cmake_minimum_required(VERSION 3.13)

# =============================================================================
# [架構師修正] 強制依賴下載路徑
# =============================================================================
# 這行非常重要！它告訴 CMake 把所有下載的依賴 (Picotool, Unity 等)
# 全部丟到 build 目錄下的 _deps 資料夾，絕對不准汙染 src 目錄。
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/_deps")

# -----------------------------------------------------------------------------
# Board and SDK (must set PICO_BOARD before pico_sdk_import.cmake in SDK 2.0.0+)
# -----------------------------------------------------------------------------
set(PICO_BOARD pico2_w CACHE STRING "Board: pico2 or pico2_w for wireless")

# Fix known picotool fetch/path detection issues
set(PICOTOOL_FETCH_FROM_GIT_PATH ON CACHE BOOL "Use git path for picotool fetch" FORCE)

# -----------------------------------------------------------------------------
# Pico SDK: use PICO_SDK_PATH if set, else fetch from git (first configure needs network)
# -----------------------------------------------------------------------------
if(NOT PICO_SDK_PATH AND NOT DEFINED ENV{PICO_SDK_PATH})
    set(PICO_SDK_FETCH_FROM_GIT ON CACHE BOOL "Fetch Pico SDK from git if not found")
endif()

include(pico_sdk_import.cmake)

project(pico2_firmware C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(PICO_BOARD pico2_w CACHE STRING "Board type" FORCE)
# -----------------------------------------------------------------------------
# Strict compiler warnings (MISRA-C preparation)
# -----------------------------------------------------------------------------
add_compile_options(
    -Wall
    -Wextra
    -Werror
)

# -----------------------------------------------------------------------------
# Initialize Pico SDK
# -----------------------------------------------------------------------------
pico_sdk_init()

# -----------------------------------------------------------------------------
# Application
# -----------------------------------------------------------------------------
add_subdirectory(src)
