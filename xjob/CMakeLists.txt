cmake_minimum_required(VERSION 3.14)
project(xasst VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 本地依赖库路径
set(LOCAL_DEPS_DIR ${CMAKE_SOURCE_DIR}/public)

# 设置 Drogon 路径
set(Drogon_DIR ${LOCAL_DEPS_DIR}/drogon/lib/cmake/Drogon)

# 设置 CMAKE_PREFIX_PATH 以查找本地库
set(CMAKE_PREFIX_PATH 
    ${LOCAL_DEPS_DIR}/jsoncpp 
    ${LOCAL_DEPS_DIR}/drogon 
    ${CMAKE_PREFIX_PATH}
)

# 添加本地 lib 路径
link_directories(
    ${LOCAL_DEPS_DIR}/drogon/lib
    ${LOCAL_DEPS_DIR}/jsoncpp/lib
)

# 查找依赖
find_package(Drogon REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# 添加 include 路径
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/views
    ${CMAKE_SOURCE_DIR}/config
    ${LOCAL_DEPS_DIR}/drogon/include
    ${OPENSSL_INCLUDE_DIR}
    ${LOCAL_DEPS_DIR}/jsoncpp/include
)

# Source files
aux_source_directory(${CMAKE_SOURCE_DIR}/src/controllers CTL_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/services SERVICE_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/utils UTIL_SRC)

# View files (pre-generated)
set(VIEW_SRC 
    ${CMAKE_SOURCE_DIR}/src/views/UploadPage.cc
    ${CMAKE_SOURCE_DIR}/src/views/FeedbackPage.cc
    ${CMAKE_SOURCE_DIR}/src/views/SurveyPage.cc
)

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
add_executable(xasst
    ${CTL_SRC}
    ${SERVICE_SRC}
    ${UTIL_SRC}
    ${VIEW_SRC}
    src/main.cpp
)

# 链接库
target_link_libraries(xasst
    drogon
    trantor
    jsoncpp
    ${OPENSSL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    pthread
    dl
    uuid
    sqlite3
)

# Testing
enable_testing()
find_package(GTest QUIET)
if(GTest_FOUND)
    add_subdirectory(test)
else()
    message(STATUS "GTest not found, tests will not be built")
endif()
