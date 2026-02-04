# 1. 使用 Ubuntu 22.04 作為基底系統 (穩定、相容性高)
FROM ubuntu:22.04

# 設定環境變數，避免安裝時跳出詢問視窗 (如時區設定)
ENV DEBIAN_FRONTEND=noninteractive

# 2. 安裝必要的編譯工具
# build-essential: 包含 make 等基礎工具
# cmake: 建置系統經理
# gcc-arm-none-eabi: 我們的主角，ARM 交叉編譯器
# libnewlib-arm-none-eabi: 解決 nosys.specs 問題的關鍵函式庫
# libstdc++-arm-none-eabi-newlib: C++ 支援庫
# python3: Pico SDK 需要用到的小幫手
# git: 用來下載 SDK
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    python3 \
    git \
    && rm -rf /var/lib/apt/lists/*

# 3. 設定工作目錄 (容器內的預設資料夾)
WORKDIR /workspace

# 4. 預設指令 (如果沒有給參數，就顯示 GCC 版本證明安裝成功)
CMD ["arm-none-eabi-gcc", "--version"]