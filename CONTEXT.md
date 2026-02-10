# 專案架構與環境設定 (CONTEXT) - Last Updated: Day 7 (Completed)

## 1. 硬體規格

* **Target**: Raspberry Pi Pico 2 (RP2350)
* **Variant**: Pico 2 W (Wireless) -> CMake 參數 `-DPICO_BOARD=pico2_w`
* **Architecture**: Dual-core ARM Cortex-M33 (AMP Mode)

## 2. 開發環境 (Strict Rules)

* **策略**: 必須使用 Docker 進行編譯，嚴禁依賴 Host 本機 Toolchain。
* **Docker Image**: `pico2_builder` (Tag: latest)
* **Container OS**: Linux (Ubuntu 22.04)
* **關鍵依賴**: Dockerfile 必須包含 `libusb-1.0-0-dev` 與 `pkg-config`。
* **掛載**: 將 Host 當前目錄 `.` 掛載到 Container `/workspace`。

## 3. 編譯指令 (唯一真理)

* **設定檔**: `docker-compose.yml` (位於根目錄)
* **指令**: `docker compose up` (若修改 Dockerfile 則加 `--build`)
* **產出**: `build_docker/pico2_firmware_main.uf2`
* **路徑規則**: Host 端 `build/` (Mac用) 與 `build_docker/` (Docker用) 必須分開。
* **CMake 變數**: 使用 `${PROJECT_NAME}_main` 來參照執行檔。

## 4. 專案結構 (Modular Architecture)

* **`src/common/` (Interface Lib)**:
* 僅包含 `.h` (如 `common_status.h`, `common_types.h`)。
* CMake 屬性: `INTERFACE`。


* **`src/hal/` (Static Lib)**:
* **唯一**允許 include `<hardware/*.h>` 與 `<pico/*.h>` 的地方。
* 透過 `PUBLIC` 屬性將 SDK 路徑與 Common 型別傳遞給 App。
* 實作：`hal_led` (V-Table pattern), `hal_multicore` (FIFO wrapper).


* **`src/app/` (Static Lib)**:
* 純業務邏輯。透過 HAL 介面操作硬體。
* **嚴禁**直接呼叫 SDK 底層 API。


* **`src/main.c`**:
* **極簡入口 (Clean Entry)**。只負責 `hal_init_system()` 並呼叫 App 入口 (`app_blink_run`)。



## 5. 目前進度

* **Day 6**: DMA (Direct Memory Access) 實作完成。
* **成就**: UART Zero-Copy 傳輸，CPU 無須介入資料搬運。


* **Day 7**: 多核心 (Multicore) 與架構重構完成。
* **成就**: 啟用 Core 0 (控制層) 與 Core 1 (運算層) 並行運作。
* **技術**:
* **AMP 架構**: Core 0 管理 LED/Log，Core 1 處理背景任務。
* **FIFO 通訊**: 實作 `hal_multicore_fifo_push/pop` 進行核心間資料傳遞。
* **CMake 重構**: 修復 Library 相依性，使用 `target_link_libraries(... PUBLIC ...)` 解決標頭檔路徑問題。
* **OOC**: 實作 Object-Oriented C (V-Table) 於 `LedDevice`。


* **驗證**: Serial Log 顯示 Core 1 正確接收並回傳運算結果。


* **下一步**: Day 8 車用通訊協定 (CAN Bus / SPI)。

## 6. 架構設計原則 (Architecture Standards) 🛡️

* **CMake 原則**:
* **Target-Centric**: 一切以 Target 為核心，不使用全域 `include_directories`。
* **Propagation**: 庫 (Library) 必須正確設定 `PUBLIC` / `PRIVATE` / `INTERFACE` 以傳遞路徑依賴。


* **Level 1 (HAL)**: 翻譯層。封裝硬體細節，提供 V-Table 或簡化介面。
* **Level 2 (App)**: 決策層。具備可移植性，不依賴特定硬體暫存器。
* **Level 3 (Interface)**: 契約層。`common` 定義跨層級的資料結構與錯誤碼。

