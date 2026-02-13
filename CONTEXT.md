這是一份更新後的 `context.md`。

這份文件現在包含了我們今天完成的所有里程碑：**CI/CD 流水線建立**、**Docker 測試腳本**、以及最關鍵的 **白箱測試策略 (White-box Testing)**。

請將以下內容完全覆蓋你原本的 `context.md`：

```markdown
# Context: Pico 2 Automotive Firmware Project

## 1. Project Overview
* **Project Name:** pico2_automotive_firmware
* **Target Hardware:** Raspberry Pi Pico 2 (RP2350)
* **Language:** C (Standard C99/C11)
* **Build System:** CMake
* **Operating System:** macOS (Development), Ubuntu (CI/CD), Docker (Standardized Build Env)
* **Current Phase:** Phase 2 - HAL Implementation & CI/CD Automation (Day 8/9)

## 2. CI/CD Architecture (Dual-Track)
We use a "Dual-Track" testing strategy to ensure code quality both locally and in the cloud.

### A. Local Guardrail (The "Hook")
* **Trigger:** `git commit`
* **Tool:** `pre-commit` framework.
* **Config:** `.pre-commit-config.yaml`
* **Actions:**
    1.  **Static Analysis:** Runs `cppcheck` locally (suppresses missing headers, checks logic).
    2.  **Unit Tests:** Runs `scripts/run_tests.sh`.
        * Spins up `pico2_builder` Docker container.
        * Mounts current directory to `/workspace`.
        * Runs `cmake` & `ctest` inside Docker.

### B. Cloud Factory (GitHub Actions)
* **Trigger:** `git push`
* **Config:** `.github/workflows/main.yml`
* **Actions:**
    1.  **Lint:** Cppcheck.
    2.  **Test:** Unit Tests (Docker/CMake).
    3.  **Build:** Compiles the actual Firmware (`.uf2`) using Pico SDK.
    4.  **Artifact:** Uploads the `.uf2` file for release.

## 3. Testing Strategy (Unity Framework)

### A. Mocking Strategy
* **Level 1: Testing App Logic (e.g., `test_blink.c`)**
    * **Goal:** Verify App logic without hardware.
    * **Method:** Mock the HAL.
    * **Files:** `test/mock_hal_gpio.c`, `test/mock_hal_led.c`.
    * **CMake:** Add these mock `.c` files to `add_executable`.

* **Level 2: Testing HAL Logic (e.g., `test_gpio_callback.c`)**
    * **Goal:** Verify HAL interacts correctly with SDK (or internal logic).
    * **Method:** Mock the Pico SDK headers.
    * **Files:** `test/mock/hardware/gpio.h`.
    * **CMake:** Use `include_directories(test/mock)`.

### B. White-box Testing (Crucial for ISRs)
To test `static` functions (like ISR handlers) or `static` variables (like callbacks):
1.  **Technique:** Directly `#include "../src/hal/hal_xxx.c"` inside the test file (`test_xxx.c`).
2.  **Rule:** **DO NOT** add the source file (`src/hal/hal_xxx.c`) to `add_executable` in `test/CMakeLists.txt`.
    * *Why?* It causes "Multiple Definition" errors because the code is already included in the test file.
3.  **Example:** `test_gpio_callback.c` includes `src/hal/hal_gpio.c` to access `_internal_gpio_isr`.

## 4. Key Directory Structure
```text
pico2_automotive_firmware/
├── .github/workflows/
│   └── main.yml           # Cloud CI workflow
├── .pre-commit-config.yaml # Local git hook config
├── scripts/
│   └── run_tests.sh       # Script to run tests in Docker (used by pre-commit)
├── src/
│   ├── app/               # Application logic (Blink, etc.)
│   └── hal/               # Hardware Abstraction Layer (GPIO, LED, etc.)
├── test/
│   ├── CMakeLists.txt     # Test build configuration
│   ├── mock/              # Header mocks (fake SDK)
│   │   └── hardware/
│   │       └── gpio.h
│   ├── mock_hal_gpio.c    # Implementation mocks (fake HAL for App)
│   ├── test_blink.c       # Tests for App
│   └── test_gpio_callback.c # Tests for HAL (White-box)
└── Dockerfile             # Build environment definition



## 5. Development Workflow Rules

1. **New Feature:** Create Source (`src/`) -> Create Test (`test/`) -> Update `test/CMakeLists.txt`.
2. **Before Commit:**
* You can run `pre-commit run --all-files` manually to check.
* Or just `git commit`, and the hook will run automatically.


3. **Mocking Rule:**
* If testing **App**, link against `mock_hal_xxx.c`.
* If testing **HAL**, link against `test/mock` headers.


4. **Static Testing Rule:**
* If testing `static` functions, use `#include "source.c"` and remove source from CMake target list.



## 6. Known Issues / Notes

* **Cppcheck:** Is configured to suppress `missingInclude` for SDK headers because they exist inside Docker, not necessarily on the local Mac host.
* **Linker Errors:** If `undefined reference to main` occurs, check if the test file has `int main(void)`. If `undefined reference to static_func`, use the White-box include technique.



### 建議下一步：
儲存這份檔案後，你的 AI (我) 在未來的對話中，就能永遠記得：
1.  我們有 **Docker** 和 **Scripts**。
2.  我們測試 **ISR (中斷)** 時要用 **White-box (`#include .c`)** 的方法。
3.  我們的 **CI/CD** 是怎麼跑的。
