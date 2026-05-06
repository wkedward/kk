#pragma once

#include <drogon/HttpModel.h>
#include <string>

namespace drogon_model {
namespace xasst {

class Feedback : public drogon::HttpModel<Feedback>
{
  public:
    HttpModelCtor(Feedback);

    PrimaryKey(id, autoIncrement);
    Column(user_id);
    Column(issue_type);
    Column(description);
    Column(screenshot_path);
    Column(video_path);
    Column(status, "pending");
    Column(created_at, "now()", true);
    Column(updated_at, "now()", true, true);

    enum Columns
    {
        Id = 0,
        UserId,
        IssueType,
        Description,
        ScreenshotPath,
        VideoPath,
        Status,
        CreatedAt,
        UpdatedAt,
        ColumnCount
    };

    void setId(int64_t id)
    {
        setColumnValue(Id, id);
    }
    int64_t getId() const
    {
        return getColumnValue<int64_t>(Id);
    }

    void setUserId(int64_t userId)
    {
        setColumnValue(UserId, userId);
    }
    int64_t getUserId() const
    {
        return getColumnValue<int64_t>(UserId);
    }

    void setIssueType(const std::string& issueType)
    {
        setColumnValue(IssueType, issueType);
    }
    std::string getIssueType() const
    {
        return getColumnValue<std::string>(IssueType);
    }

    void setDescription(const std::string& description)
    {
        setColumnValue(Description, description);
    }
    std::string getDescription() const
    {
        return getColumnValue<std::string>(Description);
    }

    void setScreenshotPath(const std::string& screenshotPath)
    {
        setColumnValue(ScreenshotPath, screenshotPath);
    }
    std::string getScreenshotPath() const
    {
        return getColumnValue<std::string>(ScreenshotPath);
    }

    void setVideoPath(const std::string& videoPath)
    {
        setColumnValue(VideoPath, videoPath);
    }
    std::string getVideoPath() const
    {
        return getColumnValue<std::string>(VideoPath);
    }

    void setStatus(const std::string& status)
    {
        setColumnValue(Status, status);
    }
    std::string getStatus() const
    {
        return getColumnValue<std::string>(Status);
    }

    void setCreatedAt(const trantor::Date& createdAt)
    {
        setColumnValue(CreatedAt, createdAt);
    }
    trantor::Date getCreatedAt() const
    {
        return getColumnValue<trantor::Date>(CreatedAt);
    }

    void setUpdatedAt(const trantor::Date& updatedAt)
    {
        setColumnValue(UpdatedAt, updatedAt);
    }
    trantor::Date getUpdatedAt() const
    {
        return getColumnValue<trantor::Date>(UpdatedAt);
    }
};

}
}
