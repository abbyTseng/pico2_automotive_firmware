#!/bin/bash
set -e

echo "🐳 [Pre-Commit] Starting Unit Tests in Docker..."

# 1. 檢查 Docker 是否活著
if ! docker info > /dev/null 2>&1; then
    echo "❌ Error: Docker is not running!"
    exit 1
fi

# 2. 啟動 Docker 進行測試
# 注意：這裡將當前目錄 $(pwd) 掛載到容器內的 /workspace
docker run --rm -v "$(pwd):/workspace" -w /workspace pico2_builder:latest /bin/bash -c "
    echo '⚙️  Configuring CMake...' && \
    cmake -S test -B build_test_docker > /dev/null && \
    echo '🔨 Building Tests...' && \
    cmake --build build_test_docker > /dev/null && \
    echo '🧪 Running CTest...' && \
    cd build_test_docker && \
    ctest --output-on-failure
"

# 3. 檢查 Docker 的回傳值
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    echo "✅ All Tests Passed!"
else
    echo "❌ Tests Failed!"
    exit 1
fi

# 使用 Docker 映像檔來編譯並執行測試
# 注意：我們掛載當前目錄到 /workspace
docker run --rm -v "$(pwd):/workspace" -w /workspace pico2_builder:latest /bin/bash -c "
    echo '⚙️  Configuring Tests...' && \
    cmake -S test -B build_test_docker > /dev/null && \
    echo '🔨 Building Tests...' && \
    cmake --build build_test_docker > /dev/null && \
    echo '🧪 Running Tests...' && \
    cd build_test_docker && \
    ctest --output-on-failure
"

EXIT_CODE=$?

if [ $EXIT_CODE -eq 0 ]; then
    echo "✅ Docker Unit Tests Passed!"
else
    echo "❌ Docker Unit Tests Failed!"
    exit 1
fi