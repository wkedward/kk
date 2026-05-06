#pragma once

#include <string>

namespace xasst {
namespace models {

struct File
{
    int64_t id = 0;
    std::string file_name;
    std::string file_path;
    int64_t file_size = 0;
    std::string md5;
    std::string download_token;
    int64_t uploaded_by = 0;
    int64_t download_count = 0;
    std::string created_at;
    std::string updated_at;
};

}
}
