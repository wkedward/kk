#pragma once

#include <string>
#include <vector>
#include <memory>
#include "models/Library.h"

namespace xasst {
namespace services {

struct LibraryUpdateInfo
{
    std::string libName;
    std::string version;
    std::string md5;
    std::string libPath;
};

class HotUpdateService
{
  public:
    HotUpdateService();
    ~HotUpdateService() = default;

    bool registerApp(const std::string& appId, int64_t userId);

    std::shared_ptr<drogon_model::xasst::Library> uploadLibrary(
        const std::string& appId,
        const std::string& libName,
        const std::string& libPath,
        const std::string& version,
        int64_t uploadedBy);

    std::vector<LibraryUpdateInfo> checkForUpdates(
        const std::string& appId,
        const std::vector<std::pair<std::string, std::string>>& localLibs);

    std::shared_ptr<drogon_model::xasst::Library> getLatestLibrary(
        const std::string& appId,
        const std::string& libName);

  private:
    std::string libraryStoragePath_;
};

}
}
