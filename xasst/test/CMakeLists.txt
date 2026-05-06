# Test executable
add_executable(xasst_test
    test_main.cpp
    file_service_test.cpp
    hot_update_service_test.cpp
    feedback_service_test.cpp
    ../src/utils/MD5Util.cpp
    ../src/utils/TokenUtil.cpp
)

target_link_libraries(xasst_test
    GTest::GTest
    GTest::Main
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Include directories
target_include_directories(xasst_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(xasst_test)
