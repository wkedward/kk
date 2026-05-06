#include "TokenUtil.h"
#include <random>
#include <chrono>

namespace xasst {
namespace utils {

std::string TokenUtil::generateToken(int length)
{
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);
    for(int i = 0; i < length; i++) {
        result += charset[dis(gen)];
    }
    return result;
}

std::string TokenUtil::generateDownloadToken()
{
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    std::string seed = std::to_string(millis) + generateToken(16);
    return generateToken(32);
}

}
}
