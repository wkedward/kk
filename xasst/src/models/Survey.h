#pragma once

#include <drogon/HttpModel.h>
#include <string>

namespace drogon_model {
namespace xasst {

class Survey : public drogon::HttpModel<Survey>
{
  public:
    HttpModelCtor(Survey);

    PrimaryKey(id, autoIncrement);
    Column(title);
    Column(description);
    Column(questions_json);
    Column(created_by);
    Column(created_at, "now()", true);
    Column(updated_at, "now()", true, true);

    enum Columns
    {
        Id = 0,
        Title,
        Description,
        QuestionsJson,
        CreatedBy,
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

    void setTitle(const std::string& title)
    {
        setColumnValue(Title, title);
    }
    std::string getTitle() const
    {
        return getColumnValue<std::string>(Title);
    }

    void setDescription(const std::string& description)
    {
        setColumnValue(Description, description);
    }
    std::string getDescription() const
    {
        return getColumnValue<std::string>(Description);
    }

    void setQuestionsJson(const std::string& questionsJson)
    {
        setColumnValue(QuestionsJson, questionsJson);
    }
    std::string getQuestionsJson() const
    {
        return getColumnValue<std::string>(QuestionsJson);
    }

    void setCreatedBy(int64_t createdBy)
    {
        setColumnValue(CreatedBy, createdBy);
    }
    int64_t getCreatedBy() const
    {
        return getColumnValue<int64_t>(CreatedBy);
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
