#include "FeedbackService.h"
#include <drogon/drogon.h>

namespace xasst {
namespace services {

using namespace drogon;

FeedbackService::FeedbackService() = default;

std::shared_ptr<models::Feedback> FeedbackService::submitFeedback(
    int64_t userId,
    const std::string& issueType,
    const std::string& description,
    const std::string& screenshotPath,
    const std::string& videoPath)
{
    auto feedback = std::make_shared<models::Feedback>();
    feedback->user_id = userId;
    feedback->issue_type = issueType;
    feedback->description = description;
    feedback->screenshot_path = screenshotPath;
    feedback->video_path = videoPath;
    feedback->status = "pending";

    auto dbClient = app().getDbClient();
    dbClient->execSqlSync(
        "INSERT INTO feedback (user_id, issue_type, description, screenshot_path, video_path, status, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, datetime('now'), datetime('now'))",
        userId, issueType, description, screenshotPath, videoPath, "pending");
    
    auto rowResult = dbClient->execSqlSync("SELECT last_insert_rowid()");
    if(!rowResult.empty()) {
        feedback->id = rowResult[0][0].as<long long>();
    }

    return feedback;
}

std::vector<std::shared_ptr<models::Feedback>> FeedbackService::getUserFeedbacks(int64_t userId)
{
    std::vector<std::shared_ptr<models::Feedback>> feedbacks;
    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync(
        "SELECT * FROM feedback WHERE user_id=? ORDER BY created_at DESC",
        userId);
    
    for(const auto& row : result) {
        auto feedback = std::make_shared<models::Feedback>();
        feedback->id = row["id"].as<long long>();
        feedback->user_id = row["user_id"].as<long long>();
        feedback->issue_type = row["issue_type"].as<std::string>();
        feedback->description = row["description"].as<std::string>();
        feedback->screenshot_path = row["screenshot_path"].as<std::string>();
        feedback->video_path = row["video_path"].as<std::string>();
        feedback->status = row["status"].as<std::string>();
        feedback->created_at = row["created_at"].as<std::string>();
        feedback->updated_at = row["updated_at"].as<std::string>();
        feedbacks.push_back(feedback);
    }

    return feedbacks;
}

std::vector<std::shared_ptr<models::Feedback>> FeedbackService::getAllFeedbacks()
{
    std::vector<std::shared_ptr<models::Feedback>> feedbacks;
    auto dbClient = app().getDbClient();
    auto result = dbClient->execSqlSync(
        "SELECT * FROM feedback ORDER BY created_at DESC");
    
    for(const auto& row : result) {
        auto feedback = std::make_shared<models::Feedback>();
        feedback->id = row["id"].as<long long>();
        feedback->user_id = row["user_id"].as<long long>();
        feedback->issue_type = row["issue_type"].as<std::string>();
        feedback->description = row["description"].as<std::string>();
        feedback->screenshot_path = row["screenshot_path"].as<std::string>();
        feedback->video_path = row["video_path"].as<std::string>();
        feedback->status = row["status"].as<std::string>();
        feedback->created_at = row["created_at"].as<std::string>();
        feedback->updated_at = row["updated_at"].as<std::string>();
        feedbacks.push_back(feedback);
    }

    return feedbacks;
}

bool FeedbackService::updateFeedbackStatus(int64_t feedbackId, const std::string& status)
{
    auto dbClient = app().getDbClient();
    dbClient->execSqlSync(
        "UPDATE feedback SET status=?, updated_at=datetime('now') WHERE id=?",
        status, feedbackId);
    return true;
}

std::shared_ptr<models::Survey> FeedbackService::createSurvey(
    const std::string& title,
    const std::string& description,
    const std::string& questionsJson,
    int64_t createdBy)
{
    auto survey = std::make_shared<models::Survey>();
    survey->title = title;
    survey->description = description;
    survey->questions_json = questionsJson;
    survey->created_by = createdBy;

    auto dbClient = app().getDbClient();
    dbClient->execSqlSync(
        "INSERT INTO survey (title, description, questions, created_by, created_at, updated_at) VALUES (?, ?, ?, ?, datetime('now'), datetime('now'))",
        title, description, questionsJson, createdBy);
    
    auto rowResult = dbClient->execSqlSync("SELECT last_insert_rowid()");
    if(!rowResult.empty()) {
        survey->id = rowResult[0][0].as<long long>();
    }

    return survey;
}

bool FeedbackService::submitSurveyResponse(
    int64_t surveyId,
    int64_t userId,
    const std::vector<SurveyAnswer>& answers)
{
    auto dbClient = app().getDbClient();
    
    Json::Value answersJson(Json::arrayValue);
    for(const auto& answer : answers) {
        Json::Value ansJson;
        ansJson["question_id"] = answer.questionId;
        ansJson["answer"] = answer.answer;
        answersJson.append(ansJson);
    }
    
    Json::FastWriter writer;
    std::string answersStr = writer.write(answersJson);

    dbClient->execSqlSync(
        "INSERT INTO survey_response (survey_id, user_id, answers, submitted_at) VALUES (?, ?, ?, datetime('now'))",
        surveyId, userId, answersStr);
    
    return true;
}

}
}
