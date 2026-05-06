cmake_minimum_required(VERSION 3.14)
project(xasst VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Find dependencies
find_package(Drogon CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/config)
include_directories(${OPENSSL_INCLUDE_DIR})

# Source files
aux_source_directory(${CMAKE_SOURCE_DIR}/src/controllers CTL_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/services SERVICE_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/utils UTIL_SRC)

# View files
file(GLOB_RECURSE VIEW_SRC ${CMAKE_SOURCE_DIR}/views/*.csp)

# Create data directories
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data/files)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data/libraries)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data/uploads)

# Copy config file
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

target_link_libraries(xasst
    Drogon::Drogon
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Testing
enable_testing()
find_package(GTest QUIET)
if(GTest_FOUND)
    add_subdirectory(test)
else()
    message(STATUS "GTest not found, tests will not be built")
endif()
