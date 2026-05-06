#pragma once

#include <string>
#include <memory>
#include "models/File.h"

namespace xasst {
namespace services {

class FileService
{
  public:
    FileService();
    ~FileService() = default;

    std::shared_ptr<models::File> uploadFile(
        const std::string& fileName,
        const std::string& filePath,
        int64_t uploadedBy);

    std::shared_ptr<models::File> getFileByToken(const std::string& token);

    void incrementDownloadCount(int64_t fileId);

    std::string generateDownloadLink(const std::string& token);

private:
    std::string fileStoragePath_;
};

}
}
