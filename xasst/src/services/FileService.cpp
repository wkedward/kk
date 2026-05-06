#include "FileService.h"
#include "utils/MD5Util.h"
#include "utils/TokenUtil.h"
#include <drogon/drogon.h>
#include <fstream>
#include <sys/stat.h>

namespace xasst {
namespace services {

FileService::FileService()
{
    auto& config = drogon::app().getCustomConfig();
    fileStoragePath_ = config.get("app", Json::nullValue)
                            .get("file_path", "./data/files/")
                            .asString();
    if(fileStoragePath_.back() != '/') {
        fileStoragePath_ += '/';
    }

    struct stat info;
    if(stat(fileStoragePath_.c_str(), &info) != 0) {
        mkdir(fileStoragePath_.c_str(), 0755);
    }
}

std::shared_ptr<models::File> FileService::uploadFile(
    const std::string& fileName,
    const std::string& tmpFilePath,
    int64_t uploadedBy)
{
    std::string md5 = utils::MD5Util::computeFile(tmpFilePath);
    struct stat fileStat;
    stat(tmpFilePath.c_str(), &fileStat);
    int64_t fileSize = fileStat.st_size;

    std::string token = utils::TokenUtil::generateDownloadToken();
    std::string newFileName = token + "_" + fileName;
    std::string newFilePath = fileStoragePath_ + newFileName;

    std::ifstream src(tmpFilePath, std::ios::binary);
    std::ofstream dst(newFilePath, std::ios::binary);
    dst << src.rdbuf();

    auto file = std::make_shared<models::File>();
    file->file_name = fileName;
    file->file_path = newFilePath;
    file->file_size = fileSize;
    file->md5 = md5;
    file->download_token = token;
    file->uploaded_by = uploadedBy;
    file->download_count = 0;

    auto dbClient = drogon::app().getDbClient();
    auto result = dbClient->execSqlSync(
        "INSERT INTO file (file_name, file_path, file_size, md5, download_token, uploaded_by, download_count, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?, datetime('now'), datetime('now'))",
        fileName, newFilePath, fileSize, md5, token, uploadedBy, 0);
    
    auto rowResult = dbClient->execSqlSync("SELECT last_insert_rowid()");
    if(!rowResult.empty()) {
        file->id = rowResult[0][0].as<long long>();
    }

    return file;
}

std::shared_ptr<models::File> FileService::getFileByToken(const std::string& token)
{
    auto dbClient = drogon::app().getDbClient();
    auto result = dbClient->execSqlSync(
        "SELECT * FROM file WHERE download_token=?",
        token);
    
    if(result.empty()) {
        return nullptr;
    }
    
    auto file = std::make_shared<models::File>();
    file->id = result[0]["id"].as<long long>();
    file->file_name = result[0]["file_name"].as<std::string>();
    file->file_path = result[0]["file_path"].as<std::string>();
    file->file_size = result[0]["file_size"].as<long long>();
    file->md5 = result[0]["md5"].as<std::string>();
    file->download_token = result[0]["download_token"].as<std::string>();
    file->uploaded_by = result[0]["uploaded_by"].as<long long>();
    file->download_count = result[0]["download_count"].as<long long>();
    file->created_at = result[0]["created_at"].as<std::string>();
    file->updated_at = result[0]["updated_at"].as<std::string>();
    
    return file;
}

void FileService::incrementDownloadCount(int64_t fileId)
{
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlSync(
        "UPDATE file SET download_count = download_count + 1 WHERE id=?",
        fileId);
}

std::string FileService::generateDownloadLink(const std::string& token)
{
    return "/download/" + token;
}

}
}
