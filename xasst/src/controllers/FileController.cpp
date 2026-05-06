#include "FileController.h"
#include "services/FileService.h"
#include <drogon/drogon.h>
#include <fstream>
#include <sstream>

namespace xasst {
namespace controllers {

using namespace drogon;

void FileController::showUploadPage(const HttpRequestPtr& req,
                                  std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto resp = HttpResponse::newHttpViewResponse("UploadPage");
    callback(resp);
}

void FileController::handleUpload(const HttpRequestPtr& req,
                             std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto& config = app().getCustomConfig();
    auto uploadPath = config.get("app", Json::Value::nullMember)
                              .get("upload_path", "./data/uploads/")
                              .asString();

    MultiPartParser fileUpload;
    if(fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("File upload failed");
        callback(resp);
        return;
    }

    auto& file = fileUpload.getFiles()[0];
    std::string tmpFilePath = uploadPath + file.getFileName();
    file.saveAs(tmpFilePath);

    services::FileService fileService;
    auto uploadedFile = fileService.uploadFile(file.getFileName(), tmpFilePath, 1);

    Json::Value result;
    result["success"] = true;
    result["file_id"] = uploadedFile->getId();
    result["download_token"] = uploadedFile->getDownloadToken();
    result["download_link"] = fileService.generateDownloadLink(uploadedFile->getDownloadToken());

    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void FileController::downloadFile(const HttpRequestPtr& req,
                            std::function<void (const HttpResponsePtr&)>&& callback,
                            const std::string& token)
{
    services::FileService fileService;
    auto file = fileService.getFileByToken(token);

    if(!file) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k404NotFound);
        resp->setBody("File not found");
        callback(resp);
        return;
    }

    std::string rangeHeader = req->getHeader("Range");
    auto resp = HttpResponse::newFileResponse(
        file->getFilePath(),
        file->getFileName(),
        CT_APPLICATION_OCTET_STREAM,
        true
    );

    fileService.incrementDownloadCount(file->getId());
    callback(resp);
}

void FileController::listFiles(const HttpRequestPtr& req,
                           std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync("SELECT * FROM file ORDER BY created_at DESC");

    Json::Value filesJson(Json::arrayValue);
    for(const auto& row : result) {
        Json::Value fileJson;
        fileJson["id"] = row["id"].as<long>();
        fileJson["file_name"] = row["file_name"].as<std::string>();
        fileJson["download_token"] = row["download_token"].as<std::string>();
        fileJson["download_count"] = row["download_count"].as<long>();
        fileJson["created_at"] = row["created_at"].as<std::string>();
        filesJson.append(fileJson);
    }

    Json::Value resultJson;
    resultJson["files"] = filesJson;
    auto resp = HttpResponse::newHttpJsonResponse(resultJson);
    callback(resp);
}

}
}
