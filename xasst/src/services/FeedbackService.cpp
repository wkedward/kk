#include "FeedbackService.h"
#include <drogon/drogon.h>
#include <json/json.h>

namespace xasst {
namespace services {

using namespace drogon;

FeedbackService::FeedbackService()
{
}

std::shared_ptr<drogon_model::xasst::Feedback> FeedbackService::submitFeedback(
    int64_t userId,
    const std::string& issueType,
    const std::string& description,
    const std::string& screenshotPath,
    const std::string& videoPath)
{
    auto feedback = std::make_shared<drogon_model::xasst::Feedback>();
    feedback->setUserId(userId);
    feedback->setIssueType(issueType);
    feedback->setDescription(description);
    feedback->setScreenshotPath(screenshotPath);
    feedback->setVideoPath(videoPath);
    feedback->setStatus("pending");
    feedback->save();

    return feedback;
}

bool FeedbackService::updateFeedbackStatus(int64_t feedbackId, const std::string& status)
{
    try {
        auto dbClient = app().getDbClient();
        dbClient->execSqlSync(
            "UPDATE feedback SET status=?, updated_at=datetime('now') WHERE id=?",
            status, feedbackId);
        return true;
    } catch(const std::exception& e) {
        LOG_ERROR << "Failed to update feedback status: " << e.what();
        return false;
    }
}

std::vector<std::shared_ptr<drogon_model::xasst::Feedback>> FeedbackService::getUserFeedbacks(int64_t userId)
{
    std::vector<std::shared_ptr<drogon_model::xasst::Feedback>> feedbacks;
    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync(
        "SELECT * FROM feedback WHERE user_id=? ORDER BY created_at DESC",
        userId);

    for(const auto& row : result) {
        auto feedback = std::make_shared<drogon_model::xasst::Feedback>();
        feedback->updateByJson(row);
        feedbacks.push_back(feedback);
    }

    return feedbacks;
}

std::vector<std::shared_ptr<drogon_model::xasst::Feedback>> FeedbackService::getAllFeedbacks()
{
    std::vector<std::shared_ptr<drogon_model::xasst::Feedback>> feedbacks;
    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync("SELECT * FROM feedback ORDER BY created_at DESC");

    for(const auto& row : result) {
        auto feedback = std::make_shared<drogon_model::xasst::Feedback>();
        feedback->updateByJson(row);
        feedbacks.push_back(feedback);
    }

    return feedbacks;
}

std::shared_ptr<drogon_model::xasst::Survey> FeedbackService::createSurvey(
    const std::string& title,
    const std::string& description,
    const std::string& questionsJson,
    int64_t createdBy)
{
    auto survey = std::make_shared<drogon_model::xasst::Survey>();
    survey->setTitle(title);
    survey->setDescription(description);
    survey->setQuestionsJson(questionsJson);
    survey->setCreatedBy(createdBy);
    survey->save();

    return survey;
}

std::shared_ptr<drogon_model::xasst::Survey> FeedbackService::getSurvey(int64_t surveyId)
{
    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync(
        "SELECT * FROM survey WHERE id=?",
        surveyId);

    if(result.empty()) {
        return nullptr;
    }

    auto survey = std::make_shared<drogon_model::xasst::Survey>();
    survey->updateByJson(result[0]);
    return survey;
}

bool FeedbackService::submitSurveyResponse(int64_t surveyId, int64_t userId, const std::vector<SurveyAnswer>& answers)
{
    try {
        auto dbClient = app().getDbClient();
        
        Json::Value answersJson(Json::objectValue);
        for(const auto& answer : answers) {
            answersJson[answer.questionId] = answer.answer;
        }

        Json::FastWriter writer;
        std::string answersStr = writer.write(answersJson);

        dbClient->execSqlSync(
            "INSERT INTO survey_response (survey_id, user_id, answers, created_at) VALUES (?, ?, ?, datetime('now'))",
            surveyId, userId, answersStr);

        return true;
    } catch(const std::exception& e) {
        LOG_ERROR << "Failed to submit survey response: " << e.what();
        return false;
    }
}

}
}
