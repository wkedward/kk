#include <drogon/drogon.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace drogon;

void initDatabase()
{
    auto dbClient = app().getDbClient();
    
    // Create tables if they don't exist
    try {
        // User table
        dbClient->execSqlSync(R"(
            CREATE TABLE IF NOT EXISTS user (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                email TEXT NOT NULL UNIQUE,
                password_hash TEXT NOT NULL,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        )");

        // File table
        dbClient->execSqlSync(R"(
            CREATE TABLE IF NOT EXISTS file (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                file_name TEXT NOT NULL,
                file_path TEXT NOT NULL,
                file_size INTEGER NOT NULL,
                md5 TEXT NOT NULL,
                download_token TEXT NOT NULL UNIQUE,
                uploaded_by INTEGER NOT NULL,
                download_count INTEGER DEFAULT 0,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (uploaded_by) REFERENCES user(id)
            )
        )");

        // Library table
        dbClient->execSqlSync(R"(
            CREATE TABLE IF NOT EXISTS library (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                app_id TEXT NOT NULL,
                lib_name TEXT NOT NULL,
                lib_path TEXT NOT NULL,
                version TEXT NOT NULL,
                md5 TEXT NOT NULL,
                uploaded_by INTEGER NOT NULL,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (uploaded_by) REFERENCES user(id)
            )
        )");

        // Survey table
        dbClient->execSqlSync(R"(
            CREATE TABLE IF NOT EXISTS survey (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                description TEXT,
                questions_json TEXT NOT NULL,
                created_by INTEGER NOT NULL,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (created_by) REFERENCES user(id)
            )
        )");

        // Feedback table
        dbClient->execSqlSync(R"(
            CREATE TABLE IF NOT EXISTS feedback (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER NOT NULL,
                issue_type TEXT NOT NULL,
                description TEXT NOT NULL,
                screenshot_path TEXT,
                video_path TEXT,
                status TEXT DEFAULT 'pending',
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (user_id) REFERENCES user(id)
            )
        )");

        // Survey response table
        dbClient->execSqlSync(R"(
            CREATE TABLE IF NOT EXISTS survey_response (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                survey_id INTEGER NOT NULL,
                user_id INTEGER NOT NULL,
                answers TEXT NOT NULL,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (survey_id) REFERENCES survey(id),
                FOREIGN KEY (user_id) REFERENCES user(id)
            )
        )");

        // Insert default user
        dbClient->execSqlSync(R"(
            INSERT OR IGNORE INTO user (name, email, password_hash) 
            VALUES ('admin', 'admin@example.com', 'default_password_hash')
        )");

        std::cout << "Database initialized successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize database: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    // Load config and run app
    app()
        .loadConfigFile("config/config.json")
        .registerBeginningAdvice([]() {
            std::cout << "XASST Service Starting..." << std::endl;
            initDatabase();
        })
        .registerPreRoutingAdvice([](const HttpRequestPtr& req, HttpResponsePtr& resp, bool& next) {
            LOG_INFO << req->methodString() << " " << req->path();
            next = true;
        })
        .setThreadNum(4)
        .run();
    
    return 0;
}
