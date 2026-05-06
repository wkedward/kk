cmake_minimum_required(VERSION 3.14)
project(xasst VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 本地依赖库路径
set(LOCAL_DEPS_DIR ${CMAKE_SOURCE_DIR}/public)
set(LOCAL_INCLUDE_DIR ${LOCAL_DEPS_DIR}/include)
set(LOCAL_LIB_DIR ${LOCAL_DEPS_DIR}/lib)

# 添加本地库路径到 CMake
link_directories(${LOCAL_LIB_DIR})

# Drogon Framework - 使用 CMake Find Module
# 查找 Drogon 的 CMake 配置文件
set(DROGON_CMAKE_DIR ${LOCAL_DEPS_DIR}/drogon/lib/cmake)
if(EXISTS ${DROGON_CMAKE_DIR})
    list(APPEND CMAKE_MODULE_PATH ${DROGON_CMAKE_DIR})
    find_package(Drogon REQUIRED)
else()
    message(FATAL_ERROR "Drogon not found in ${DROGON_CMAKE_DIR}")
endif()

# brpc - 使用 CMake Find Module
set(BRPC_CMAKE_DIR ${LOCAL_DEPS_DIR}/brpc)
if(EXISTS ${BRPC_CMAKE_DIR}/CMakeLists.txt)
    add_subdirectory(${BRPC_CMAKE_DIR} ${CMAKE_BINARY_DIR}/brpc_build EXCLUDE_FROM_ALL)
else()
    message(STATUS "brpc not found, skipping...")
endif()

# OpenSSL - 优先使用本地版本
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

# 其他系统库
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)

# 添加 include 路径
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/config
    ${LOCAL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${DROGON_INCLUDE_DIR}
)

# Source files
aux_source_directory(${CMAKE_SOURCE_DIR}/src/controllers CTL_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/services SERVICE_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/utils UTIL_SRC)

# View files
file(GLOB_RECURSE VIEW_SRC ${CMAKE_SOURCE_DIR}/views/*.csp)

# 创建 data 目录
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data/files)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data/libraries)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data/uploads)

# 复制配置文件到构建目录
configure_file(
    ${CMAKE_SOURCE_DIR}/config/config.json
    ${CMAKE_BINARY_DIR}/config.json
    COPYONLY
)

# Main executable
drogon_add_executable(xasst
    ${CTL_SRC}
    ${SERVICE_SRC}
    ${UTIL_SRC}
    ${VIEW_SRC}
    src/main.cpp
)

# 链接库
target_link_libraries(xasst
    Drogon::Drogon
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    zlib
    pthread
    dl
)

# 添加 brpc 依赖（如果存在）
if(TARGET brpc)
    target_link_libraries(xasst brpc)
endif()

# Testing
enable_testing()
find_package(GTest QUIET)
if(GTest_FOUND)
    add_subdirectory(test)
else()
    message(STATUS "GTest not found, tests will not be built")
endif()
