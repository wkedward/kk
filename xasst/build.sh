#!/bin/bash

# XASST 项目构建脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "========================================="
echo "XASST 项目构建"
echo "========================================="

# 检查依赖是否已安装
if [ ! -d "$SCRIPT_DIR/public/drogon" ]; then
    echo "错误: 依赖库未安装"
    echo "请先运行: ./setup_dependencies.sh"
    exit 1
fi

# 创建构建目录
cd "$SCRIPT_DIR"
if [ ! -d "build" ]; then
    mkdir build
fi
cd build

# CMake 配置
echo "运行 CMake 配置..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$SCRIPT_DIR/public" \
    -DCMAKE_INSTALL_PREFIX="$SCRIPT_DIR"

# 编译
echo "编译项目..."
make -j$(nproc)

echo ""
echo "========================================="
echo "构建完成！"
echo "========================================="
echo ""
echo "可执行文件: $SCRIPT_DIR/build/xasst"
echo ""
echo "运行项目:"
echo "  ./build/xasst"
echo ""
