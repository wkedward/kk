#pragma once

#include <string>

namespace xasst {
namespace models {

struct User
{
    int64_t id = 0;
    std::string name;
    std::string email;
    std::string password_hash;
    std::string created_at;
    std::string updated_at;
};

}
}
