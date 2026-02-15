# Context: Pico 2 Automotive Firmware Project

## 1. Project Overview
* **Project Name:** pico2_automotive_firmware
* **Target Hardware:** Raspberry Pi Pico 2 W (RP2350 + CYW43) + SSD1306 OLED
* **Language:** C (Standard C99/C11)
* **Build System:** CMake + Docker (Standardized Build Env)
* **Testing:** Unity Framework (Unit Test) + Saleae/Oscilloscope (Physical Test)
* **Current Phase:** Phase 3 - RTOS Kernel & SMP (Starting Day 11)

## 2. Architecture & Design Patterns (Layered Architecture)
We have refactored the system into a strict 3-layer architecture to ensure decoupling and testability.

### A. Main Layer (`src/main.c`)
* **Role:** System Scheduler & Configurator.
* **Responsibilities:**
    * Initializes System HAL (`hal_init_system`).
    * Initializes Drivers (`hal_led`, `hal_i2c`, `hal_storage`).
    * Injects dependencies into App Layer.
    * Runs the Super Loop.
* **Constraint:** **NO** direct hardware manipulation logic allowed here.

### B. App Layer (`src/app/`)
* **Role:** Business Logic (e.g., `app_display`, `app_storage`).
* **Responsibilities:**
    * Implements feature logic (e.g., OLED UI state machine, Boot counting).
    * Calls HAL interfaces using `common_status_t`.
* **Constraint:** Hardware-agnostic. Should run on any MCU if HAL is provided.

### C. HAL Layer (`src/hal/`)
* **Role:** Hardware Abstraction.
* **Key Modules:**
    * **`hal_i2c`:** Implements "9-Clock Recovery" and physical timing timeouts.
    * **`hal_led`:** Abstracts Pico 2W's CYW43 wireless LED control behind a generic interface.
    * **`hal_gpio`:** Handles interrupts and callback registration.
    * **`hal_storage`:** (New) Abstraction for LittleFS on Flash with XIP protection.

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
chinghuitseng@Mac pico2_automotive_firmware % tree -I "build*|.git|.vscode"
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
├── external
│   └── littlefs           # [Added Day 10]
├── src
│   ├── CMakeLists.txt
│   ├── app
│   │   ├── CMakeLists.txt
│   │   ├── app_blink.c
│   │   ├── app_blink.h
│   │   ├── app_display.c
│   │   ├── app_display.h
│   │   ├── app_storage.c  # [Added Day 10]
│   │   └── app_storage.h  # [Added Day 10]
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
│   │   ├── hal_multicore.h
│   │   ├── hal_storage.c  # [Added Day 10]
│   │   └── hal_storage.h  # [Added Day 10]
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

## 6. Development Workflow Rules
* **New Feature:** Create Source (`src/`) -> Create Test (`test/`) -> Update `test/CMakeLists.txt`.
* **Mocking Rule:**
    * Testing App? Link `mock_hal_xxx.c`.
    * Testing HAL? Include `test/mock` headers.
* **Static Testing Rule:**
    * Testing static functions? Use `#include "source.c"`.

---

## 7. Next Steps (Phase 3: RTOS & SMP)
**Goal:** Introduce FreeRTOS Kernel and Symmetric Multi-Processing (SMP) to handle concurrency.

**Tasks (Day 11):**
1.  **Integrate FreeRTOS:** Add FreeRTOS-SMP kernel to the project.
2.  **Core Activation:** Enable Core 1.
3.  **Task Separation:**
    * **Core 0 Task:** Critical Logic & Storage (LittleFS).
    * **Core 1 Task:** UI Refresh (OLED/LED) to prevent blocking.
4.  **Thread Safety:** Implement Spinlocks/Mutexes for shared resources (`hal_i2c`, `hal_gpio`).

---

## 8. 🛠 Current Technical Debt (技術債與重構追蹤)

### Resolved (Phase 2 Finished)
* ✅ **[Fixed] Flash Collision:** Implemented `save_and_disable_interrupts()` in `hal_storage.c` to prevent XIP crashes during write operations.
* ✅ **[Fixed] I2C Timing:** Increased timeout to 50ms to account for physical transmission time of 129 bytes at 100kHz.
* ✅ **[Fixed] OLED Artifacts:** Corrected SSD1306 initialization (Multiplex Ratio & Page Addressing Mode 0x02) for 128x32 display.
* ✅ **[Fixed] Enum Conversion:** Enforced explicit casting between `hal_i2c_status_t` and `common_status_t`.

### Pending (Phase 3 Focus)
* ⚠️ **[High] Thread Safety:** `hal_i2c` and `hal_gpio` are currently **NOT** thread-safe. Concurrent access from Core 0 and Core 1 will cause race conditions. (Target: Day 11)
* ⚠️ **[Low] MISRA Compliance:** `common_ringbuffer.c` still contains some pointer arithmetic that needs review against MISRA C:2012 Rule 18.4.

---

## 9. Key Metrics (Portfolio Proof - Day 10)
* **Reliability:** System verified to survive power-loss events during filesystem writes (LittleFS Copy-on-Write).
* **Persistence:** Boot Count tracking verified in logs (Count increments across reboots: 17 -> 18).
* **Display Stability:** Artifact-free rendering on SSD1306 (128x32) confirmed via visual verification.