#include "FeedbackController.h"
#include "services/FeedbackService.h"
#include <drogon/drogon.h>
#include <fstream>

namespace xasst {
namespace controllers {

using namespace drogon;

void FeedbackController::showFeedbackPage(const HttpRequestPtr& req,
                                    std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto resp = HttpResponse::newHttpViewResponse("FeedbackPage");
    callback(resp);
}

void FeedbackController::submitFeedback(const HttpRequestPtr& req,
                              std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto& config = app().getCustomConfig();
    auto uploadPath = config.get("app", Json::Value::nullMember)
                              .get("upload_path", "./data/uploads/")
                              .asString();

    std::string issueType;
    std::string description;
    std::string screenshotPath;
    std::string videoPath;

    MultiPartParser fileUpload;
    if(fileUpload.parse(req) == 0) {
        for(const auto& file : fileUpload.getFiles()) {
            std::string tmpFilePath = uploadPath + file.getFileName();
            file.saveAs(tmpFilePath);
            if(file.getFileName().find("screenshot") != std::string::npos) {
                screenshotPath = tmpFilePath;
            } else if(file.getFileName().find("video") != std::string::npos) {
                videoPath = tmpFilePath;
            }
        }
        
        issueType = fileUpload.getParameter("issue_type");
        description = fileUpload.getParameter("description");
    } else {
        auto json = req->getJsonObject();
        if(json && json->isMember("issue_type") && json->isMember("description")) {
            issueType = (*json)["issue_type"].asString();
            description = (*json)["description"].asString();
        }
    }

    if(issueType.empty() || description.empty()) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Missing required parameters";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    services::FeedbackService feedbackService;
    auto feedback = feedbackService.submitFeedback(1, issueType, description, screenshotPath, videoPath);

    Json::Value result;
    result["success"] = true;
    result["feedback_id"] = feedback->getId();
    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void FeedbackController::listUserFeedbacks(const HttpRequestPtr& req,
                                std::function<void (const HttpResponsePtr&)>&& callback)
{
    services::FeedbackService feedbackService;
    auto feedbacks = feedbackService.getUserFeedbacks(1);

    Json::Value result;
    result["success"] = true;
    Json::Value feedbacksJson(Json::arrayValue);
    
    for(const auto& feedback : feedbacks) {
        Json::Value feedbackJson;
        feedbackJson["id"] = feedback->getId();
        feedbackJson["issue_type"] = feedback->getIssueType();
        feedbackJson["description"] = feedback->getDescription();
        feedbackJson["status"] = feedback->getStatus();
        feedbackJson["created_at"] = feedback->getCreatedAt().toDbString();
        feedbacksJson.append(feedbackJson);
    }
    result["feedbacks"] = feedbacksJson;

    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void FeedbackController::listAllFeedbacks(const HttpRequestPtr& req,
                                std::function<void (const HttpResponsePtr&)>&& callback)
{
    services::FeedbackService feedbackService;
    auto feedbacks = feedbackService.getAllFeedbacks();

    Json::Value result;
    result["success"] = true;
    Json::Value feedbacksJson(Json::arrayValue);
    
    for(const auto& feedback : feedbacks) {
        Json::Value feedbackJson;
        feedbackJson["id"] = feedback->getId();
        feedbackJson["user_id"] = feedback->getUserId();
        feedbackJson["issue_type"] = feedback->getIssueType();
        feedbackJson["description"] = feedback->getDescription();
        feedbackJson["status"] = feedback->getStatus();
        feedbackJson["created_at"] = feedback->getCreatedAt().toDbString();
        feedbacksJson.append(feedbackJson);
    }
    result["feedbacks"] = feedbacksJson;

    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void FeedbackController::updateFeedbackStatus(const HttpRequestPtr& req,
                                    std::function<void (const HttpResponsePtr&)>&& callback,
                                    int64_t id)
{
    auto json = req->getJsonObject();
    if(!json || !json->isMember("status")) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Missing status parameter";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::string status = (*json)["status"].asString();

    services::FeedbackService feedbackService;
    bool success = feedbackService.updateFeedbackStatus(id, status);

    Json::Value result;
    result["success"] = success;
    result["message"] = success ? "Status updated successfully" : "Failed to update status";
    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void FeedbackController::showSurveyPage(const HttpRequestPtr& req,
                                std::function<void (const HttpResponsePtr&)>&& callback,
                                int64_t id)
{
    auto resp = HttpResponse::newHttpViewResponse("SurveyPage");
    callback(resp);
}

void FeedbackController::createSurvey(const HttpRequestPtr& req,
                            std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto json = req->getJsonObject();
    if(!json || !json->isMember("title") || !json->isMember("description") || !json->isMember("questions")) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Missing required parameters";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::string title = (*json)["title"].asString();
    std::string description = (*json)["description"].asString();
    
    Json::FastWriter writer;
    std::string questionsJson = writer.write((*json)["questions"]);

    services::FeedbackService feedbackService;
    auto survey = feedbackService.createSurvey(title, description, questionsJson, 1);

    Json::Value result;
    result["success"] = true;
    result["survey_id"] = survey->getId();
    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void FeedbackController::submitSurvey(const HttpRequestPtr& req,
                            std::function<void (const HttpResponsePtr&)>&& callback,
                            int64_t id)
{
    auto json = req->getJsonObject();
    if(!json || !json->isMember("answers")) {
        Json::Value result;
        result["success"] = false;
        result["message"] = "Missing answers parameter";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::vector<services::SurveyAnswer> answers;
    auto& answersJson = (*json)["answers"];
    for(const auto& answer : answersJson) {
        services::SurveyAnswer sa;
        sa.questionId = answer["question_id"].asString();
        sa.answer = answer["answer"].asString();
        answers.push_back(sa);
    }

    services::FeedbackService feedbackService;
    bool success = feedbackService.submitSurveyResponse(id, 1, answers);

    Json::Value result;
    result["success"] = success;
    result["message"] = success ? "Survey submitted successfully" : "Failed to submit survey";
    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

}
}
