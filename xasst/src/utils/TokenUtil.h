#pragma once

#include <string>

namespace xasst {
namespace utils {

class TokenUtil
{
  public:
    static std::string generateToken(int length = 32);
    static std::string generateDownloadToken();
};

}
}
