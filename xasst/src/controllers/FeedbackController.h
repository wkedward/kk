#pragma once

#include <drogon/HttpController.h>

namespace xasst {
namespace controllers {

class FeedbackController : public drogon::HttpController<FeedbackController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(FeedbackController::showFeedbackPage, "/feedback", drogon::Get);
    ADD_METHOD_TO(FeedbackController::submitFeedback, "/feedback/submit", drogon::Post);
    ADD_METHOD_TO(FeedbackController::listUserFeedbacks, "/feedback/user", drogon::Get);
    ADD_METHOD_TO(FeedbackController::listAllFeedbacks, "/feedback/all", drogon::Get);
    ADD_METHOD_TO(FeedbackController::updateFeedbackStatus, "/feedback/{id}/status", drogon::Put);
    ADD_METHOD_TO(FeedbackController::showSurveyPage, "/survey/{id}", drogon::Get);
    ADD_METHOD_TO(FeedbackController::createSurvey, "/survey/create", drogon::Post);
    ADD_METHOD_TO(FeedbackController::submitSurvey, "/survey/{id}/submit", drogon::Post);
    METHOD_LIST_END

    void showFeedbackPage(const drogon::HttpRequestPtr& req,
                        std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void submitFeedback(const drogon::HttpRequestPtr& req,
                       std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void listUserFeedbacks(const drogon::HttpRequestPtr& req,
                          std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void listAllFeedbacks(const drogon::HttpRequestPtr& req,
                          std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void updateFeedbackStatus(const drogon::HttpRequestPtr& req,
                            std::function<void (const drogon::HttpResponsePtr&)>&& callback,
                            int64_t id);

    void showSurveyPage(const drogon::HttpRequestPtr& req,
                      std::function<void (const drogon::HttpResponsePtr&)>&& callback,
                      int64_t id);

    void createSurvey(const drogon::HttpRequestPtr& req,
                     std::function<void (const drogon::HttpResponsePtr&)>&& callback);

    void submitSurvey(const drogon::HttpRequestPtr& req,
                     std::function<void (const drogon::HttpResponsePtr&)>&& callback,
                     int64_t id);
};

}
}
