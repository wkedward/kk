#include <gtest/gtest.h>
#include "services/FeedbackService.h"

// FeedbackService 测试
class FeedbackServiceTest : public ::testing::Test {
protected:
    xasst::services::FeedbackService feedbackService;
};

TEST_F(FeedbackServiceTest, CanCreateInstance) {
    SUCCEED();
}
