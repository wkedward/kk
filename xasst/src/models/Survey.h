#pragma once

#include <string>

namespace xasst {
namespace models {

struct Survey
{
    int64_t id = 0;
    std::string title;
    std::string description;
    std::string questions_json;
    int64_t created_by = 0;
    std::string created_at;
    std::string updated_at;
};

}
}
