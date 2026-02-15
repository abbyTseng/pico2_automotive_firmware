#!/bin/bash
# /workspace/scripts/run_tests.sh
set -e

cd "$(dirname "$0")/.."

echo "🚀 [Native] Starting Unit Tests (Host Mode)..."

# 清除舊快取確保開關生效
rm -rf build_test_host

echo "⚙️  Configuring Host Tests..."
docker compose run --rm builder cmake -B build_test_host -S test

echo "🔨 Building Host Tests..."
docker compose run --rm builder cmake --build build_test_host

echo "🧪 Running Tests..."
# 加入 --fail-on-no-test 確保沒找到測試時會報錯
docker compose run --rm builder ctest --test-dir build_test_host --output-on-failure --fail-on-no-test

echo "✅ Real Tests Passed!"