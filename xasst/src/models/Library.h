#pragma once

#include <drogon/HttpModel.h>
#include <string>

namespace drogon_model {
namespace xasst {

class Library : public drogon::HttpModel<Library>
{
  public:
    HttpModelCtor(Library);

    PrimaryKey(id, autoIncrement);
    Column(app_id);
    Column(lib_name);
    Column(lib_path);
    Column(version);
    Column(md5);
    Column(uploaded_by);
    Column(created_at, "now()", true);
    Column(updated_at, "now()", true, true);

    enum Columns
    {
        Id = 0,
        AppId,
        LibName,
        LibPath,
        Version,
        Md5,
        UploadedBy,
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

    void setAppId(const std::string& appId)
    {
        setColumnValue(AppId, appId);
    }
    std::string getAppId() const
    {
        return getColumnValue<std::string>(AppId);
    }

    void setLibName(const std::string& libName)
    {
        setColumnValue(LibName, libName);
    }
    std::string getLibName() const
    {
        return getColumnValue<std::string>(LibName);
    }

    void setLibPath(const std::string& libPath)
    {
        setColumnValue(LibPath, libPath);
    }
    std::string getLibPath() const
    {
        return getColumnValue<std::string>(LibPath);
    }

    void setVersion(const std::string& version)
    {
        setColumnValue(Version, version);
    }
    std::string getVersion() const
    {
        return getColumnValue<std::string>(Version);
    }

    void setMd5(const std::string& md5)
    {
        setColumnValue(Md5, md5);
    }
    std::string getMd5() const
    {
        return getColumnValue<std::string>(Md5);
    }

    void setUploadedBy(int64_t uploadedBy)
    {
        setColumnValue(UploadedBy, uploadedBy);
    }
    int64_t getUploadedBy() const
    {
        return getColumnValue<int64_t>(UploadedBy);
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
