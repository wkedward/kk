#pragma once

#include <drogon/HttpController.h>

namespace xasst {
namespace controllers {

class FileController : public drogon::HttpController<FileController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(FileController::showUploadPage, "/upload", drogon::Get);
    ADD_METHOD_TO(FileController::handleUpload, "/upload", drogon::Post);
    ADD_METHOD_TO(FileController::downloadFile, "/download/{token}", drogon::Get);
    ADD_METHOD_TO(FileController::listFiles, "/files", drogon::Get);
    METHOD_LIST_END

    void showUploadPage(const drogon::HttpRequestPtr& req,
                       std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void handleUpload(const drogon::HttpRequestPtr& req,
                     std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void downloadFile(const drogon::HttpRequestPtr& req,
                     std::function<void (const drogon::HttpResponsePtr&)>&& callback,
                     const std::string& token);

    void listFiles(const drogon::HttpRequestPtr& req,
                  std::function<void (const drogon::HttpResponsePtr&)>&& callback);
};

}
}
