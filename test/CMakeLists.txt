cmake_minimum_required(VERSION 3.14)

project(pico2_tests C)

# ==============================================================================
# 1. 環境準備 (Unity 測試框架)
# ==============================================================================
include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)

enable_testing()

# 設定標頭檔搜尋路徑 (讓測試程式找不到 .h 檔時去哪裡找)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/app
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/hal
)

# ==============================================================================
# 2. 既有的測試: test_blink (測試 App 層邏輯)
# ==============================================================================
# 如果你之前的 mock 檔還在，這個測試應該要能跑
# (備註：如果你 app_blink.c 加入了多核心功能，這裡可能缺 hal_multicore 的 mock，
#  如果報錯可以暫時註解掉下面這一段，或是補上 mock_hal_multicore.c)
add_executable(test_blink
    test_blink.c
    ../src/app/app_blink.c
    mock_hal_led.c 
    mock_hal_delay.c
    # 如果 app_blink.c 用到了 multicore，這裡未來要補 mock_hal_multicore.c
)
target_link_libraries(test_blink unity)
add_test(NAME test_blink COMMAND test_blink)

# ==============================================================================
# 3. 新增的測試: test_ringbuffer (測試 Common 資料結構)
# ==============================================================================
add_executable(test_ringbuffer
    test_ringbuffer.c                  # 測試腳本 (我們等下要寫)
    ../src/common/common_ringbuffer.c  # 被測試的實作 (我們等下要寫)
)

target_link_libraries(test_ringbuffer unity)

# 加入 CTest 清單
add_test(NAME test_ringbuffer COMMAND test_ringbuffer)