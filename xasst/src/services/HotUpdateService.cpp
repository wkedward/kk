#include "HotUpdateService.h"
#include "utils/MD5Util.h"
#include <drogon/drogon.h>
#include <fstream>
#include <sys/stat.h>

namespace xasst {
namespace services {

using namespace drogon;

HotUpdateService::HotUpdateService()
{
    auto& config = app().getCustomConfig();
    libraryStoragePath_ = config.get("app", Json::nullValue)
                                 .get("library_path", "./data/libraries/")
                                 .asString();
    if(libraryStoragePath_.back() != '/') {
        libraryStoragePath_ += '/';
    }

    struct stat info;
    if(stat(libraryStoragePath_.c_str(), &info) != 0) {
        mkdir(libraryStoragePath_.c_str(), 0755);
    }
}

bool HotUpdateService::registerApp(const std::string& appId, int64_t userId)
{
    try {
        auto dbClient = app().getDbClient();
        auto result = dbClient->execSqlSync(
            "SELECT id FROM library WHERE app_id=? LIMIT 1",
            appId);
        
        if(result.empty()) {
            dbClient->execSqlSync(
                "INSERT INTO library (app_id, lib_name, version, created_at, updated_at) VALUES (?, 'dummy', '0.0.0', datetime('now'), datetime('now'))",
                appId);
        }
        return true;
    } catch(const std::exception& e) {
        LOG_ERROR << "Failed to register app: " << e.what();
        return false;
    }
}

std::shared_ptr<models::Library> HotUpdateService::uploadLibrary(
    const std::string& appId,
    const std::string& libName,
    const std::string& tmpLibPath,
    const std::string& version,
    int64_t uploadedBy)
{
    std::string md5 = utils::MD5Util::computeFile(tmpLibPath);
    std::string newFileName = appId + "_" + libName + "_" + version + ".so";
    std::string newLibPath = libraryStoragePath_ + newFileName;

    std::ifstream src(tmpLibPath, std::ios::binary);
    std::ofstream dst(newLibPath, std::ios::binary);
    dst << src.rdbuf();

    auto library = std::make_shared<models::Library>();
    library->app_id = appId;
    library->lib_name = libName;
    library->lib_path = newLibPath;
    library->version = version;
    library->md5 = md5;
    library->uploaded_by = uploadedBy;

    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync(
        "INSERT INTO library (app_id, lib_name, lib_path, version, md5, uploaded_by, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, datetime('now'), datetime('now'))",
        appId, libName, newLibPath, version, md5, uploadedBy);
    
    auto rowResult = dbClient->execSqlSync("SELECT last_insert_rowid()");
    if(!rowResult.empty()) {
        library->id = rowResult[0][0].as<long long>();
    }

    return library;
}

std::vector<LibraryUpdateInfo> HotUpdateService::checkForUpdates(
    const std::string& appId,
    const std::vector<std::pair<std::string, std::string>>& localLibs)
{
    std::vector<LibraryUpdateInfo> updates;
    auto dbClient = app().getDbClient();

    for(const auto& localLib : localLibs) {
        auto result = dbClient->execSqlSync(
            "SELECT * FROM library WHERE app_id=? AND lib_name=? ORDER BY created_at DESC LIMIT 1",
            appId, localLib.first);
        
        if(!result.empty()) {
            std::string serverMd5 = result[0]["md5"].as<std::string>();
            std::string localMd5 = localLib.second;

            if(serverMd5 != localMd5) {
                LibraryUpdateInfo info;
                info.libName = result[0]["lib_name"].as<std::string>();
                info.version = result[0]["version"].as<std::string>();
                info.md5 = serverMd5;
                info.libPath = result[0]["lib_path"].as<std::string>();
                updates.push_back(info);
            }
        }
    }

    return updates;
}

std::shared_ptr<models::Library> HotUpdateService::getLatestLibrary(
    const std::string& appId,
    const std::string& libName)
{
    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync(
        "SELECT * FROM library WHERE app_id=? AND lib_name=? ORDER BY created_at DESC LIMIT 1",
        appId, libName);
    
    if(result.empty()) {
        return nullptr;
    }

    auto library = std::make_shared<models::Library>();
    library->id = result[0]["id"].as<long long>();
    library->app_id = result[0]["app_id"].as<std::string>();
    library->lib_name = result[0]["lib_name"].as<std::string>();
    library->lib_path = result[0]["lib_path"].as<std::string>();
    library->version = result[0]["version"].as<std::string>();
    library->md5 = result[0]["md5"].as<std::string>();
    library->uploaded_by = result[0]["uploaded_by"].as<long long>();
    library->created_at = result[0]["created_at"].as<std::string>();
    library->updated_at = result[0]["updated_at"].as<std::string>();
    
    return library;
}

}
}
