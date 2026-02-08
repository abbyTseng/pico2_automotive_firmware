cmake_minimum_required(VERSION 3.14)

project(pico2_tests C)

# 1. 自動下載 Unity 測試框架
include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)

# 2. 設定包含路徑 (讓我們能測試 src/app 的程式碼)
# 注意：我們要測試的是「純邏輯」，所以只包含 app 和 common，不包含 hal (因為 host 沒有硬體)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/hal  # 雖然包含，但我們會用 Mock (替身)
)

# 3. 定義第一個測試執行檔
add_executable(test_blink
    test_blink.c
    ../src/app/app_blink.c
    # 這裡我們需要一個 "假的" HAL，不然 app_blink.c 連結會失敗
    mock_hal_led.c 
    mock_hal_delay.c
)

# 4. 連結 Unity
target_link_libraries(test_blink unity)

# 5. 加入 CTest 支援
enable_testing()
add_test(NAME test_blink COMMAND test_blink)