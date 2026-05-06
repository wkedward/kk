#include "MD5Util.h"
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>
#include <fstream>

namespace xasst {
namespace utils {

std::string MD5Util::computeString(const std::string& input)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);

    std::stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return ss.str();
}

std::string MD5Util::computeFile(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if(!file) {
        return "";
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);

    char buffer[8192];
    while(file.read(buffer, sizeof(buffer))) {
        MD5_Update(&ctx, buffer, file.gcount());
    }
    if(file.gcount() > 0) {
        MD5_Update(&ctx, buffer, file.gcount());
    }

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);

    std::stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return ss.str();
}

}
}
