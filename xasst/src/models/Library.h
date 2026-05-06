#pragma once

#include <string>

namespace xasst {
namespace models {

struct Library
{
    int64_t id = 0;
    std::string app_id;
    std::string lib_name;
    std::string lib_path;
    std::string version;
    std::string md5;
    int64_t uploaded_by = 0;
    std::string created_at;
    std::string updated_at;
};

}
}
