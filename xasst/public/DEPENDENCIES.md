# 依赖库管理说明

本项目使用本地依赖库，所有依赖的头文件和编译后的动态库都放在 `public` 目录下。

## 目录结构

```
public/
├── include/              # 通用头文件目录
│   └── ...               # 其他第三方库头文件
├── drogon/              # Drogon 框架
│   ├── include/          # Drogon 头文件
│   └── lib/              # Drogon 库文件
│       ├── cmake/        # CMake 模块
│       └── libdrogon.a   # 静态库
├── brpc/                # brpc RPC 框架
│   ├── include/          # brpc 头文件
│   ├── src/              # brpc 源代码
│   └── lib/              # brpc 库文件
├── openssl/             # OpenSSL 加密库
│   ├── include/          # OpenSSL 头文件
│   └── lib/              # OpenSSL 库文件
│       ├── libssl.a
│       └── libcrypto.a
├── sqlite3/             # SQLite3 数据库
│   ├── include/          # SQLite3 头文件
│   │   └── sqlite3.h
│   └── lib/              # SQLite3 库文件
│       └── libsqlite3.a
└── lib/                 # 其他编译好的动态库
    ├── libpthread.so
    ├── libz.so
    └── ...
```

## 依赖库准备步骤

### 1. Drogon 框架

```bash
# 克隆 Drogon 源码
git clone https://github.com/an-tao/drogon.git
cd drogon

# 安装依赖
# CentOS/RHEL:
sudo yum install cmake gcc-c++ glib2-devel jsoncpp-devel openssl-devel uuid-devel

# Ubuntu/Debian:
sudo apt-get install cmake build-essential libglib2.0-dev libjsoncpp-dev libssl-dev libuuid1

# 编译安装到 public 目录
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/xasst/public/drogon
make -j$(nproc)
make install
```

### 2. brpc

```bash
# 安装 brpc 依赖
# CentOS/RHEL:
sudo yum install protobuf-compiler protobuf-devel leveldb-devel snappy-devel gflags-devel

# Ubuntu/Debian:
sudo apt-get install libprotobuf-dev protobuf-compiler libleveldb-dev libsnappy-dev libgflags-dev

# 克隆 brpc 源码
git clone https://github.com/apache/brpc.git
cd brpc

# 编译安装到 public 目录
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/xasst/public/brpc \
         -DWITH_MUDPUP=OFF -DWITH_UTIL=ON
make -j$(nproc)
make install
```

### 3. OpenSSL

```bash
# 克隆 OpenSSL 源码
git clone https://github.com/openssl/openssl.git
cd openssl

# 编译安装到 public 目录
./Configure --prefix=/path/to/xasst/public/openssl
make -j$(nproc)
make install
```

### 4. SQLite3

```bash
# 下载 SQLite3 源码
wget https://www.sqlite.org/2024/sqlite-autoconf-3450200.tar.gz
tar xzf sqlite-autoconf-3450200.tar.gz
cd sqlite-autoconf-3450200

# 编译安装到 public 目录
./configure --prefix=/path/to/xasst/public/sqlite3 --enable-static
make -j$(nproc)
make install
```

### 5. 其他依赖

```bash
# Ubuntu/Debian
sudo apt-get install libgtest-dev libgmock-dev zlib1g-dev

# CentOS/RHEL
sudo yum install gtest-devel gmock-devel zlib-devel
```

## 一键构建脚本

创建 `build_deps.sh` 脚本：

```bash
#!/bin/bash

set -e

DEPS_DIR=$(cd "$(dirname "$0")" && pwd)/public

# 创建目录结构
mkdir -p $DEPS_DIR/{drogon,brpc,openssl,sqlite3,include,lib}

# 安装系统依赖 (Ubuntu)
sudo apt-get update
sudo apt-get install -y \
    cmake build-essential \
    libglib2.0-dev libjsoncpp-dev libssl-dev libuuid1 \
    protobuf-compiler libprotobuf-dev libleveldb-dev libsnappy-dev libgflags-dev \
    libgtest-dev zlib1g-dev

# 构建 Drogon
cd $DEPS_DIR/..
git clone --recursive https://github.com/an-tao/drogon.git
cd drogon
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$DEPS_DIR/drogon
make -j$(nproc)
make install

# 构建 brpc
cd $DEPS_DIR/..
git clone https://github.com/apache/brpc.git
cd brpc
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$DEPS_DIR/brpc \
         -DWITH_MUDPUP=OFF -DWITH_UTIL=ON
make -j$(nproc)
make install

# 构建 OpenSSL
cd $DEPS_DIR/..
git clone https://github.com/openssl/openssl.git
cd openssl
./Configure --prefix=$DEPS_DIR/openssl
make -j$(nproc)
make install

# 构建 SQLite3
cd $DEPS_DIR/..
wget https://www.sqlite.org/2024/sqlite-autoconf-3450200.tar.gz
tar xzf sqlite-autoconf-3450200.tar.gz
cd sqlite-autoconf-3450200
./configure --prefix=$DEPS_DIR/sqlite3 --enable-static
make -j$(nproc)
make install

echo "所有依赖库已成功构建到 $DEPS_DIR"
```

## 使用说明

1. **首次设置**: 运行上面的构建脚本，或手动按照步骤编译各个依赖库
2. **编译项目**: 
   ```bash
   mkdir build && cd build
   cmake ..
   make -j$(nproc)
   ```
3. **运行**:
   ```bash
   ./xasst
   ```

## 注意事项

- 确保所有依赖库编译为静态库或动态库放在对应的 `lib` 目录
- 头文件放在对应的 `include` 目录
- CMake 会自动在 `public` 目录查找依赖
- 如果使用动态库，确保运行时能找到这些库文件
