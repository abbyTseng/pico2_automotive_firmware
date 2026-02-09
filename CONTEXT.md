# 專案架構與環境設定 (CONTEXT) - Last Updated: Day 6 (Completed)

## 1. 硬體規格
* **Target**: Raspberry Pi Pico 2 (RP2350)
* **Variant**: Pico 2 W (Wireless) -> CMake 參數 `-DPICO_BOARD=pico2_w`

## 2. 開發環境 (Strict Rules)
* **策略**: 必須使用 Docker 進行編譯，嚴禁依賴 Host 本機 Toolchain。
* **Docker Image**: `my-pico-builder` (Tag: latest)
* **Container OS**: Linux (Ubuntu 22.04)
* **關鍵依賴**: Dockerfile 必須包含 `libusb-1.0-0-dev` 與 `pkg-config`。
* **掛載**: 將 Host 當前目錄 `.` 掛載到 Container `/workspace`。

## 3. 編譯指令 (唯一真理)
* **設定檔**: `docker-compose.yml` (位於根目錄)
* **指令**: `docker compose up --build` (若改 Dockerfile) 或 `docker compose up`
* **產出**: `build_docker/pico2_firmware_main.uf2`
* **路徑規則**: Host 端 `build/` (Mac用) 與 `build_docker/` (Docker用) 必須分開。
* **CMake 變數**: 使用 `${PROJECT_NAME}_main` 來參照執行檔。

## 4. 專案結構
* `src/hal/`: 硬體抽象層 (HAL)。**唯一**允許 include `<hardware/*.h>` 的地方。
* `src/app/`: 應用邏輯層 (App)。只處理業務，**嚴禁**直接呼叫 SDK。
* `src/main.c`: 系統入口。負責初始化 HAL，啟動 App。
* `.github/workflows/`: CI/CD 自動化。
* `.git/hooks/`: 本機 pre-commit 檢查。

## 5. 目前進度
* **Day 6**: DMA (Direct Memory Access) 實作完成。
    * **成就**: 實作 `hal_dma_transfer` (Mem-to-Mem) 與 `hal_dma_uart_send` (UART Zero-Copy)。
    * **技術**: 導入 Cache Coherence (`__dmb`) 與 DREQ 流量控制。
    * **驗證**: 確認 CPU 非阻塞特性 (Non-blocking verified)。
* **下一步**: Day 7 雙核心 (Multicore) 架構。

## 6. 架構設計原則 (Architecture Standards) 🛡️
* **Level 1 (HAL)**: 翻譯層。負責 `hardware/*` 操作。
* **Level 2 (App)**: 決策層。具備可移植性，透過 HAL 介面操作。
* **Level 3 (Interface)**: 契約層。`hal_*.h` 定義功能。