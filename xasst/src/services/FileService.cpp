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
    fileStoragePath_ = config.get("app", Json::Value::nullMember)
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

std::shared_ptr<drogon_model::xasst::File> FileService::uploadFile(
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

    auto file = std::make_shared<drogon_model::xasst::File>();
    file->setFileName(fileName);
    file->setFilePath(newFilePath);
    file->setFileSize(fileSize);
    file->setMd5(md5);
    file->setDownloadToken(token);
    file->setUploadedBy(uploadedBy);
    file->setDownloadCount(0);
    file->save();

    return file;
}

std::shared_ptr<drogon_model::xasst::File> FileService::getFileByToken(const std::string& token)
{
    auto dbClient = drogon::app().getDbClient();
    auto result = dbClient->execSqlSync(
        "SELECT * FROM file WHERE download_token=?",
        token);
    
    if(result.empty()) {
        return nullptr;
    }
    
    auto file = std::make_shared<drogon_model::xasst::File>();
    file->updateByJson(result[0]);
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
