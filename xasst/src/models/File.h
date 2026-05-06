#pragma once

#include <drogon/HttpModel.h>
#include <string>

namespace drogon_model {
namespace xasst {

class File : public drogon::HttpModel<File>
{
  public:
    HttpModelCtor(File);

    PrimaryKey(id, autoIncrement);
    Column(file_name);
    Column(file_path);
    Column(file_size);
    Column(md5);
    Column(download_token);
    Column(uploaded_by);
    Column(download_count, "0");
    Column(created_at, "now()", true);
    Column(updated_at, "now()", true, true);

    enum Columns
    {
        Id = 0,
        FileName,
        FilePath,
        FileSize,
        Md5,
        DownloadToken,
        UploadedBy,
        DownloadCount,
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

    void setFileName(const std::string& fileName)
    {
        setColumnValue(FileName, fileName);
    }
    std::string getFileName() const
    {
        return getColumnValue<std::string>(FileName);
    }

    void setFilePath(const std::string& filePath)
    {
        setColumnValue(FilePath, filePath);
    }
    std::string getFilePath() const
    {
        return getColumnValue<std::string>(FilePath);
    }

    void setFileSize(int64_t fileSize)
    {
        setColumnValue(FileSize, fileSize);
    }
    int64_t getFileSize() const
    {
        return getColumnValue<int64_t>(FileSize);
    }

    void setMd5(const std::string& md5)
    {
        setColumnValue(Md5, md5);
    }
    std::string getMd5() const
    {
        return getColumnValue<std::string>(Md5);
    }

    void setDownloadToken(const std::string& downloadToken)
    {
        setColumnValue(DownloadToken, downloadToken);
    }
    std::string getDownloadToken() const
    {
        return getColumnValue<std::string>(DownloadToken);
    }

    void setUploadedBy(int64_t uploadedBy)
    {
        setColumnValue(UploadedBy, uploadedBy);
    }
    int64_t getUploadedBy() const
    {
        return getColumnValue<int64_t>(UploadedBy);
    }

    void setDownloadCount(int64_t downloadCount)
    {
        setColumnValue(DownloadCount, downloadCount);
    }
    int64_t getDownloadCount() const
    {
        return getColumnValue<int64_t>(DownloadCount);
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
