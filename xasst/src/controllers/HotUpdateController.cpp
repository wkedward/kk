#include "HotUpdateController.h"
#include "services/HotUpdateService.h"
#include <drogon/drogon.h>
#include <fstream>

namespace xasst {
namespace controllers {

using namespace drogon;

void HotUpdateController::registerApp(const HttpRequestPtr& req,
                                std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto json = req->getJsonObject();
    if(!json || !json->isMember("app_id")) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Missing app_id";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::string appId = (*json)["app_id"].asString();
    services::HotUpdateService hotUpdateService;
    bool success = hotUpdateService.registerApp(appId, 1);

    Json::Value result;
    result["success"] = success;
    result["message"] = success ? "App registered successfully" : "Failed to register app";
    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void HotUpdateController::uploadLibrary(const HttpRequestPtr& req,
                                std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto& config = app().getCustomConfig();
    auto uploadPath = config.get("app", Json::nullValue)
                              .get("upload_path", "./data/uploads/")
                              .asString();

    MultiPartParser fileUpload;
    if(fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "File upload failed";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    auto& file = fileUpload.getFiles()[0];
    std::string tmpFilePath = uploadPath + file.getFileName();
    file.saveAs(tmpFilePath);

    auto json = req->getJsonObject();
    if(!json || !json->isMember("app_id") || !json->isMember("lib_name") || !json->isMember("version")) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Missing required parameters";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::string appId = (*json)["app_id"].asString();
    std::string libName = (*json)["lib_name"].asString();
    std::string version = (*json)["version"].asString();

    services::HotUpdateService hotUpdateService;
    auto library = hotUpdateService.uploadLibrary(appId, libName, tmpFilePath, version, 1);

    Json::Value result;
    result["success"] = true;
    result["library_id"] = library->id;
    result["md5"] = library->md5;
    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void HotUpdateController::checkUpdates(const HttpRequestPtr& req,
                                std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto json = req->getJsonObject();
    if(!json || !json->isMember("app_id") || !json->isMember("libraries")) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Missing required parameters";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::string appId = (*json)["app_id"].asString();
    std::vector<std::pair<std::string, std::string>> localLibs;
    
    auto& libsJson = (*json)["libraries"];
    for(const auto& lib : libsJson) {
        localLibs.emplace_back(
            lib["name"].asString(),
            lib["md5"].asString()
        );
    }

    services::HotUpdateService hotUpdateService;
    auto updates = hotUpdateService.checkForUpdates(appId, localLibs);

    Json::Value result;
    result["success"] = true;
    Json::Value updatesJson(Json::arrayValue);
    
    for(const auto& update : updates) {
        Json::Value updateJson;
        updateJson["lib_name"] = update.libName;
        updateJson["version"] = update.version;
        updateJson["md5"] = update.md5;
        updateJson["download_url"] = "/hotupdate/download/" + appId + "/" + update.libName;
        updatesJson.append(updateJson);
    }
    result["updates"] = updatesJson;

    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void HotUpdateController::downloadLibrary(const HttpRequestPtr&,
                                    std::function<void (const HttpResponsePtr&)>&& callback,
                                    const std::string& appId,
                                    const std::string& libName)
{
    services::HotUpdateService hotUpdateService;
    auto library = hotUpdateService.getLatestLibrary(appId, libName);

    if(!library) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Library not found";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k404NotFound);
        callback(resp);
        return;
    }

    auto resp = HttpResponse::newFileResponse(
        library->lib_path,
        library->lib_name + "_" + library->version + ".so",
        CT_APPLICATION_OCTET_STREAM
    );
    callback(resp);
}

}
}
