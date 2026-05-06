#pragma once

#include <string>

namespace xasst {
namespace models {

struct Feedback
{
    int64_t id = 0;
    int64_t user_id = 0;
    std::string issue_type;
    std::string description;
    std::string screenshot_path;
    std::string video_path;
    std::string status = "pending";
    std::string created_at;
    std::string updated_at;
};

}
}
