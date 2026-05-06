#!/bin/bash

# XASST 依赖库下载和组织脚本
# 将所有依赖库的头文件和库文件组织到 public 目录

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEPS_DIR="$SCRIPT_DIR/public"

echo "========================================="
echo "XASST 依赖库安装脚本"
echo "========================================="

# 创建目录结构
echo "创建目录结构..."
mkdir -p "$DEPS_DIR"/{include,drogon,brpc,openssl,sqlite3,lib}
mkdir -p "$DEPS_DIR/drogon"/{include,lib/cmake}
mkdir -p "$DEPS_DIR/brpc"/{include,src,lib}
mkdir -p "$DEPS_DIR/openssl"/{include,lib}
mkdir -p "$DEPS_DIR/sqlite3"/{include,lib}

# 检查系统类型
if [ -f /etc/redhat-release ]; then
    SYSTEM_TYPE="centos"
elif [ -f /etc/debian_version ]; then
    SYSTEM_TYPE="debian"
else
    SYSTEM_TYPE="unknown"
fi

echo "检测到系统类型: $SYSTEM_TYPE"

# 安装系统依赖
echo "安装系统依赖..."

if [ "$SYSTEM_TYPE" = "centos" ]; then
    sudo yum update -y
    sudo yum install -y \
        cmake \
        gcc-c++ \
        glib2-devel \
        jsoncpp-devel \
        openssl-devel \
        uuid-devel \
        protobuf-compiler \
        protobuf-devel \
        leveldb-devel \
        snappy-devel \
        gflags-devel \
        zlib-devel \
        gtest-devel \
        git \
        wget
elif [ "$SYSTEM_TYPE" = "debian" ]; then
    sudo apt-get update
    sudo apt-get install -y \
        cmake \
        build-essential \
        libglib2.0-dev \
        libjsoncpp-dev \
        libssl-dev \
        uuid-dev \
        protobuf-compiler \
        libprotobuf-dev \
        libleveldb-dev \
        libsnappy-dev \
        libgflags-dev \
        zlib1g-dev \
        libgtest-dev \
        git \
        wget
else
    echo "未知系统类型，请手动安装依赖"
    exit 1
fi

echo "系统依赖安装完成"
echo ""

# 下载并构建 Drogon
echo "正在下载并构建 Drogon..."
if [ ! -d "$SCRIPT_DIR/_drogon_build" ]; then
    cd "$SCRIPT_DIR"
    git clone --recursive https://github.com/an-tao/drogon.git _drogon_src
    cd _drogon_src
    mkdir -p build
    cd build
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="$DEPS_DIR/drogon" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_TESTING=OFF
    make -j$(nproc)
    make install
    cd "$SCRIPT_DIR"
    rm -rf _drogon_src
fi
echo "Drogon 构建完成"
echo ""

# 下载并构建 brpc
echo "正在下载并构建 brpc..."
if [ ! -d "$SCRIPT_DIR/_brpc_build" ]; then
    cd "$SCRIPT_DIR"
    git clone https://github.com/apache/brpc.git _brpc_src
    cd _brpc_src
    mkdir -p build
    cd build
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="$DEPS_DIR/brpc" \
        -DCMAKE_BUILD_TYPE=Release \
        -DWITH_MUDPUP=OFF \
        -DWITH_UTIL=ON \
        -DBUILD_TESTING=OFF
    make -j$(nproc)
    make install
    cd "$SCRIPT_DIR"
    rm -rf _brpc_src
fi
echo "brpc 构建完成"
echo ""

# 下载并构建 OpenSSL
echo "正在下载并构建 OpenSSL..."
if [ ! -d "$SCRIPT_DIR/_openssl_build" ]; then
    cd "$SCRIPT_DIR"
    if [ ! -f "openssl.tar.gz" ]; then
        wget https://www.openssl.org/source/openssl-3.2.1.tar.gz
    fi
    tar -xzf openssl-3.2.1.tar.gz
    cd openssl-3.2.1
    ./Configure --prefix="$DEPS_DIR/openssl" --openssldir="$DEPS_DIR/openssl/ssl" linux-x86_64
    make -j$(nproc)
    make install_sw
    cd "$SCRIPT_DIR"
    rm -rf openssl-3.2.1 openssl.tar.gz
fi
echo "OpenSSL 构建完成"
echo ""

# 下载并构建 SQLite3
echo "正在下载并构建 SQLite3..."
if [ ! -d "$SCRIPT_DIR/_sqlite_build" ]; then
    cd "$SCRIPT_DIR"
    wget https://www.sqlite.org/2024/sqlite-autoconf-3450200.tar.gz
    tar -xzf sqlite-autoconf-3450200.tar.gz
    cd sqlite-autoconf-3450200
    ./configure --prefix="$DEPS_DIR/sqlite3" --enable-static
    make -j$(nproc)
    make install
    cd "$SCRIPT_DIR"
    rm -rf sqlite-autoconf-3450200 sqlite-autoconf-3450200.tar.gz
fi
echo "SQLite3 构建完成"
echo ""

# 复制 GTest (如果系统库中没有)
echo "配置 GTest..."
if [ ! -f "$DEPS_DIR/lib/libgtest.a" ]; then
    if [ "$SYSTEM_TYPE" = "centos" ]; then
        sudo cp -r /usr/lib64/libgtest* "$DEPS_DIR/lib/" 2>/dev/null || true
        sudo cp -r /usr/include/gtest "$DEPS_DIR/include/" 2>/dev/null || true
    elif [ "$SYSTEM_TYPE" = "debian" ]; then
        sudo cp -r /usr/lib/*/libgtest* "$DEPS_DIR/lib/" 2>/dev/null || true
        sudo cp -r /usr/include/gtest "$DEPS_DIR/include/" 2>/dev/null || true
    fi
fi
echo "GTest 配置完成"
echo ""

# 清理临时目录
echo "清理临时文件..."
cd "$SCRIPT_DIR"
rm -rf _drogon_build _brpc_build _openssl_build _sqlite_build 2>/dev/null || true

echo "========================================="
echo "依赖库安装完成！"
echo "========================================="
echo ""
echo "所有依赖库已安装到: $DEPS_DIR"
echo ""
echo "现在可以编译项目:"
echo "  cd $SCRIPT_DIR"
echo "  mkdir build && cd build"
echo "  cmake .."
echo "  make"
echo ""
echo "或者运行一键构建:"
echo "  ./build.sh"
