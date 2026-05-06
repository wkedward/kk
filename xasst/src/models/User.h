#pragma once

#include <drogon/HttpModel.h>
#include <string>

namespace drogon_model {
namespace xasst {

class User : public drogon::HttpModel<User>
{
  public:
    HttpModelCtor(User);

    PrimaryKey(id, autoIncrement);
    Column(name);
    Column(email);
    Column(password_hash);
    Column(created_at, "now()", true);
    Column(updated_at, "now()", true, true);

    enum Columns
    {
        Id = 0,
        Name,
        Email,
        PasswordHash,
        CreatedAt,
        UpdatedAt,
        ColumnCount
    };

    void setId(int64_t id)
    {
        setColumnValue(Id, id);
    }
    int64_t getId() const
    {
        return getColumnValue<int64_t>(Id);
    }

    void setName(const std::string& name)
    {
        setColumnValue(Name, name);
    }
    std::string getName() const
    {
        return getColumnValue<std::string>(Name);
    }

    void setEmail(const std::string& email)
    {
        setColumnValue(Email, email);
    }
    std::string getEmail() const
    {
        return getColumnValue<std::string>(Email);
    }

    void setPasswordHash(const std::string& passwordHash)
    {
        setColumnValue(PasswordHash, passwordHash);
    }
    std::string getPasswordHash() const
    {
        return getColumnValue<std::string>(PasswordHash);
    }

    void setCreatedAt(const trantor::Date& createdAt)
    {
        setColumnValue(CreatedAt, createdAt);
    }
    trantor::Date getCreatedAt() const
    {
        return getColumnValue<trantor::Date>(CreatedAt);
    }

    void setUpdatedAt(const trantor::Date& updatedAt)
    {
        setColumnValue(UpdatedAt, updatedAt);
    }
    trantor::Date getUpdatedAt() const
    {
        return getColumnValue<trantor::Date>(UpdatedAt);
    }
};

}
}
