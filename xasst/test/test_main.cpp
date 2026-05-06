#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "utils/MD5Util.h"
#include "utils/TokenUtil.h"

// MD5Utils 测试
TEST(MD5UtilTest, ComputeString) {
    std::string result = xasst::utils::MD5Util::computeString("Hello World");
    // "Hello World" 的标准 MD5 值
    EXPECT_EQ(result.size(), 32);
    EXPECT_NE(result, "");
}

// TokenUtils 测试
TEST(TokenUtilTest, GenerateToken) {
    std::string token = xasst::utils::TokenUtil::generateToken(32);
    EXPECT_EQ(token.size(), 32);
}

TEST(TokenUtilTest, GenerateDownloadToken) {
    std::string token = xasst::utils::TokenUtil::generateDownloadToken();
    EXPECT_EQ(token.size(), 32);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
