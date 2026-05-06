# XASST - 现代化后台服务系统

一个基于 brpc 和 drogon 开发的高性能后台服务系统，提供文件管理、动态库热更新和用户反馈功能。

## 功能特性

### 1. 文件下载中心
- 📤 文件上传与下载
- 🔗 下载链接生成
- ⏸️ 断点续传
- 📊 下载统计

### 2. 动态库热更新
- 📦 应用注册
- 📥 动态库上传
- 🔄 版本管理
- 🚀 自动更新检查

### 3. 用户问卷与问题反馈
- 📝 用户调查问卷
- 💬 问题反馈提交
- 📷 支持截图与视频上传
- 🔍 反馈管理与状态追踪

## 技术栈

- **Web 框架**: Drogon (C++17)
- **RPC 框架**: brpc
- **数据库**: SQLite
- **ORM**: Drogon ORM
- **构建系统**: CMake

## 项目结构

```
xasst/
├── CMakeLists.txt
├── config/
│   └── config.json
├── src/
│   ├── main.cpp
│   ├── controllers/
│   │   ├── FileController.h
│   │   ├── FileController.cpp
│   │   ├── HotUpdateController.h
│   │   ├── HotUpdateController.cpp
│   │   ├── FeedbackController.h
│   │   └── FeedbackController.cpp
│   ├── services/
│   │   ├── FileService.h
│   │   ├── FileService.cpp
│   │   ├── HotUpdateService.h
│   │   ├── HotUpdateService.cpp
│   │   ├── FeedbackService.h
│   │   └── FeedbackService.cpp
│   ├── models/
│   │   ├── User.h
│   │   ├── File.h
│   │   ├── Library.h
│   │   ├── Survey.h
│   │   └── Feedback.h
│   └── utils/
│       ├── MD5Util.h
│       ├── MD5Util.cpp
│       ├── TokenUtil.h
│       └── TokenUtil.cpp
├── views/
│   ├── UploadPage.csp
│   ├── FeedbackPage.csp
│   └── SurveyPage.csp
├── data/
│   ├── files/
│   ├── libraries/
│   └── uploads/
└── test/
    ├── CMakeLists.txt
    ├── test_main.cpp
    ├── file_service_test.cpp
    ├── hot_update_service_test.cpp
    └── feedback_service_test.cpp
```

## 构建与运行

### 前置要求

- CMake 3.14+
- C++17 编译器
- Drogon 框架
- brpc 框架
- OpenSSL
- SQLite3

### 构建

```bash
mkdir build
cd build
cmake ..
make
```

### 运行

```bash
./xasst
```

服务将在 `http://localhost:8080` 启动。

## 主要 API

### 文件管理
- `GET /upload` - 文件上传页面
- `POST /upload` - 上传文件
- `GET /download/{token}` - 下载文件
- `GET /files` - 文件列表

### 动态库热更新
- `POST /hotupdate/register` - 注册应用
- `POST /hotupdate/upload` - 上传动态库
- `POST /hotupdate/check` - 检查更新
- `GET /hotupdate/download/{appId}/{libName}` - 下载动态库

### 问卷与反馈
- `GET /feedback` - 反馈页面
- `POST /feedback/submit` - 提交反馈
- `GET /feedback/user` - 用户反馈列表
- `GET /feedback/all` - 所有反馈
- `PUT /feedback/{id}/status` - 更新反馈状态
- `GET /survey/{id}` - 调查页面
- `POST /survey/create` - 创建调查
- `POST /survey/{id}/submit` - 提交调查

## 数据库

系统会在首次运行时自动创建所需的数据库表结构。

## 测试

```bash
cd build
make test
# 或
./xasst_test
```

## 许可证

MIT License
