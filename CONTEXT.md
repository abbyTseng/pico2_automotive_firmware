沒問題，Abby。你是對的，我剛剛把 Day 10 的目標搞混了。

根據你提供的 30 天計畫，**Day 10 的重點是儲存與可靠性 (LittleFS)**，而不是 RTOS。

這是修正後的 `CONTEXT.md`，請直接覆蓋原本的檔案。這份文件準確反映了我們目前的進度（完成 Day 9 I2C Recovery）以及正確的下一步（Day 10 LittleFS）。

```markdown
# Context: Pico 2 Automotive Firmware Project

## 1. Project Overview
* **Project Name:** pico2_automotive_firmware
* **Target Hardware:** Raspberry Pi Pico 2 W (RP2350 + CYW43)
* **Language:** C (Standard C99/C11)
* **Build System:** CMake + Docker (Standardized Build Env)
* **Testing:** Unity Framework (Unit Test) + Saleae/Oscilloscope (Physical Test)
* **Current Phase:** Phase 2 - HAL Robustness & Architecture Refactoring (Day 9 Completed)

## 2. Architecture & Design Patterns (Layered Architecture)
We have refactored the system into a strict 3-layer architecture to ensure decoupling and testability.

### A. Main Layer (`src/main.c`)
* **Role:** System Scheduler & Configurator.
* **Responsibilities:**
    * Initializes System HAL (`hal_init_system`).
    * Initializes Drivers (`hal_led`, `hal_i2c`).
    * Injects dependencies into App Layer.
    * Runs the Super Loop.
* **Constraint:** **NO** direct hardware manipulation logic allowed here.

### B. App Layer (`src/app/`)
* **Role:** Business Logic (e.g., `app_display`, `app_blink`).
* **Responsibilities:**
    * Implements feature logic (e.g., OLED UI state machine).
    * Calls HAL interfaces.
* **Constraint:** Hardware-agnostic. Should run on any MCU if HAL is provided.

### C. HAL Layer (`src/hal/`)
* **Role:** Hardware Abstraction.
* **Key Modules:**
    * **`hal_i2c`:** Implements "9-Clock Recovery" (Bit-banging) for stuck bus scenarios.
    * **`hal_led`:** Abstracts Pico 2W's CYW43 wireless LED control behind a generic interface.
    * **`hal_gpio`:** Handles interrupts and callback registration.

---

## 3. CI/CD Architecture (Dual-Track)

### A. Local Guardrail (The "Hook")
* **Trigger:** `git commit`
* **Tool:** `pre-commit` framework.
* **Actions:**
    1.  **Static Analysis:** `cppcheck` (configured to suppress missing system headers).
    2.  **Unit Tests:** `scripts/run_tests.sh` (Runs CMake/CTest inside Docker).

### B. Cloud Factory (GitHub Actions)
* **Trigger:** `git push`
* **Actions:** Lint, Test, Build (`.uf2`), and Artifact Upload.

---

## 4. Testing Strategy (Unity Framework)

### A. Mocking Strategy
* **Level 1: Testing App Logic**
    * **Goal:** Verify App logic without hardware.
    * **Method:** Link against `mock_hal_xxx.c`.
    * **CMake:** Add mock source files to `add_executable`.

* **Level 2: Testing HAL Logic (The "Mock SDK" Approach)**
    * **Goal:** Verify HAL interacts correctly with Pico SDK functions (e.g., `gpio_init`).
    * **Method:** Use `test/mock/` headers to simulate SDK behavior in Docker (x86).
    * **Files:** `test/mock/pico/stdlib.h`, `test/mock/hardware/gpio.h`.

### B. White-box Testing (Crucial for Internal Logic)
To test `static` functions (ISRs, internal state machines) or `static` variables:
1.  **Technique:** Directly `#include "../src/hal/hal_xxx.c"` inside the test file.
2.  **Rule:** **DO NOT** add the source file to `add_executable` in CMake to avoid multiple definitions.
3.  **MISRA C Compromise:** To mock standard SDK types like `uint`, we use `typedef unsigned int uint;` in our Mock headers (`mock/pico/stdlib.h`) to satisfy the compiler on non-embedded platforms.

---

## 5. Key Directory Structure
```text
pchinghuitseng@Mac pico2_automotive_firmware % tree -I "build*|.git|.vscode"
.
├── 30天計畫.md
├── CMakeLists.txt
├── CMakePresets.json
├── CONTEXT.md
├── Dockerfile
├── docker-compose.yml
├── pico_sdk_import.cmake
├── scripts
│   └── run_tests.sh
├── src
│   ├── CMakeLists.txt
│   ├── app
│   │   ├── CMakeLists.txt
│   │   ├── app_blink.c
│   │   ├── app_blink.h
│   │   ├── app_display.c
│   │   └── app_display.h
│   ├── common
│   │   ├── CMakeLists.txt
│   │   ├── common_ringbuffer.c
│   │   ├── common_ringbuffer.h
│   │   ├── common_status.h
│   │   └── common_types.h
│   ├── hal
│   │   ├── CMakeLists.txt
│   │   ├── hal_delay.c
│   │   ├── hal_delay.h
│   │   ├── hal_dma.c
│   │   ├── hal_dma.h
│   │   ├── hal_gpio.c
│   │   ├── hal_gpio.h
│   │   ├── hal_i2c.c
│   │   ├── hal_i2c.h
│   │   ├── hal_init.c
│   │   ├── hal_init.h
│   │   ├── hal_led.c
│   │   ├── hal_led.h
│   │   ├── hal_multicore.c
│   │   └── hal_multicore.h
│   └── main.c
└── test
    ├── CMakeLists.txt
    ├── mock
    │   ├── hardware
    │   │   ├── gpio.h
    │   │   └── i2c.h
    │   └── pico
    │       └── stdlib.h
    ├── mock_hal_delay.c
    ├── mock_hal_gpio.c
    ├── mock_hal_led.c
    ├── mock_hal_multicore.c
    ├── test_app_display.c
    ├── test_blink.c
    ├── test_gpio_callback.c
    ├── test_hal_i2c.c
    └── test_ringbuffer.c

10 directories, 48 files


## 6. Development Workflow Rules

1. **New Feature:** Create Source (`src/`) -> Create Test (`test/`) -> Update `test/CMakeLists.txt`.
2. **Mocking Rule:**
* Testing **App**? Link `mock_hal_xxx.c`.
* Testing **HAL**? Include `test/mock` headers.


3. **Static Testing Rule:**
* Testing `static` functions? Use `#include "source.c"`.



## 7. Next Steps (Day 10)

* **Goal:** Storage & Reliability (LittleFS).
* **Tasks:**
1. **Integrate LittleFS:** Add the library to the project structure.
2. **Mount Filesystem:** Configure LittleFS on the internal Flash of RP2350.
3. **Power-Loss Resilience Test:**
* Create a "Boot Counter" file.
* Simulate power cuts during write operations.
* Verify filesystem integrity (no corruption) upon reboot.





```

```