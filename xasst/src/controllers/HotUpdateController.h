#pragma once

#include <drogon/HttpController.h>

namespace xasst {
namespace controllers {

class HotUpdateController : public drogon::HttpController<HotUpdateController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(HotUpdateController::registerApp, "/hotupdate/register", drogon::Post);
    ADD_METHOD_TO(HotUpdateController::uploadLibrary, "/hotupdate/upload", drogon::Post);
    ADD_METHOD_TO(HotUpdateController::checkUpdates, "/hotupdate/check", drogon::Post);
    ADD_METHOD_TO(HotUpdateController::downloadLibrary, "/hotupdate/download/{appId}/{libName}", drogon::Get);
    METHOD_LIST_END

    void registerApp(const drogon::HttpRequestPtr& req,
                    std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void uploadLibrary(const drogon::HttpRequestPtr& req,
                      std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void checkUpdates(const drogon::HttpRequestPtr& req,
                     std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void downloadLibrary(const drogon::HttpRequestPtr& req,
                        std::function<void (const drogon::HttpResponsePtr&)>&& callback,
                        const std::string& appId,
                        const std::string& libName);
};

}
}
