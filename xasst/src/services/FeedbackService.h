#pragma once

#include <string>
#include <vector>
#include <memory>
#include "models/Feedback.h"
#include "models/Survey.h"

namespace xasst {
namespace services {

struct SurveyAnswer
{
    std::string questionId;
    std::string answer;
};

class FeedbackService
{
  public:
    FeedbackService();
    ~FeedbackService() = default;

    std::shared_ptr<drogon_model::xasst::Feedback> submitFeedback(
        int64_t userId,
        const std::string& issueType,
        const std::string& description,
        const std::string& screenshotPath,
        const std::string& videoPath);

    bool updateFeedbackStatus(int64_t feedbackId, const std::string& status);

    std::vector<std::shared_ptr<drogon_model::xasst::Feedback>> getUserFeedbacks(int64_t userId);

    std::vector<std::shared_ptr<drogon_model::xasst::Feedback>> getAllFeedbacks();

    std::shared_ptr<drogon_model::xasst::Survey> createSurvey(
        const std::string& title,
        const std::string& description,
        const std::string& questionsJson,
        int64_t createdBy);

    std::shared_ptr<drogon_model::xasst::Survey> getSurvey(int64_t surveyId);

    bool submitSurveyResponse(int64_t surveyId, int64_t userId, const std::vector<SurveyAnswer>& answers);
};

}
}
