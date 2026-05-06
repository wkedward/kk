#pragma once

#include <string>
#include <fstream>

namespace xasst {
namespace utils {

class MD5Util
{
  public:
    static std::string computeString(const std::string& input);
    static std::string computeFile(const std::string& filePath);
};

}
}
