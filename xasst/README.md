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
├── CMakeLists.txt          # 项目构建配置
├── README.md               # 项目文档
├── build.sh               # 一键构建脚本
├── setup_dependencies.sh   # 依赖库安装脚本
├── public/                # 依赖库目录
│   ├── DEPENDENCIES.md    # 依赖库管理说明
│   ├── include/           # 通用头文件
│   ├── drogon/            # Drogon 框架
│   ├── brpc/              # brpc RPC框架
│   ├── openssl/           # OpenSSL 加密库
│   ├── sqlite3/           # SQLite3 数据库
│   └── lib/               # 动态库和静态库
├── config/
│   └── config.json        # 应用配置
├── src/
│   ├── main.cpp           # 程序入口
│   ├── controllers/        # Web 控制器
│   ├── services/          # 业务逻辑层
│   ├── models/             # 数据模型
│   └── utils/              # 工具类
├── views/                  # 前端页面模板
│   ├── UploadPage.csp      # 文件上传页面
│   ├── FeedbackPage.csp   # 问题反馈页面
│   └── SurveyPage.csp     # 用户调查页面
├── data/                   # 数据存储目录
│   ├── files/              # 上传文件存储
│   ├── libraries/           # 动态库存储
│   └── uploads/             # 临时上传文件
└── test/                   # 单元测试
    ├── CMakeLists.txt
    └── *_test.cpp
```

## 快速开始

### 方法一：一键安装和构建（推荐）

```bash
# 1. 安装所有依赖库（首次需要，可能需要几分钟）
chmod +x setup_dependencies.sh
./setup_dependencies.sh

# 2. 构建项目
chmod +x build.sh
./build.sh

# 3. 运行服务
./build/xasst
```

### 方法二：分步操作

#### 步骤 1：安装依赖库

依赖库会自动下载、编译并安装到 `public/` 目录：

- **Drogon**: 现代 C++ Web 框架
- **brpc**: 百度 RPC 框架
- **OpenSSL**: SSL/TLS 加密库
- **SQLite3**: 轻量级数据库
- **其他**: Zlib, GTest 等

#### 步骤 2：构建项目

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

#### 步骤 3：运行

```bash
./xasst
```

服务将在 `http://localhost:8080` 启动。

## 主要 API

### 文件管理
- `GET /upload` - 文件上传页面
- `POST /upload` - 上传文件
- `GET /download/{token}` - 下载文件（支持断点续传）
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

系统会在首次运行时自动创建所需的数据库表结构，包括：
- `user` - 用户表
- `file` - 文件表
- `library` - 动态库表
- `survey` - 问卷表
- `feedback` - 反馈表
- `survey_response` - 问卷回答表

## 测试

```bash
cd build
make test
# 或直接运行
./xasst_test
```

## 依赖库管理

所有第三方依赖库的头文件和编译后的库文件都放在 `public/` 目录下：

```
public/
├── drogon/      # Drogon 框架头文件和库
├── brpc/        # brpc 框架头文件和库
├── openssl/     # OpenSSL 头文件和库
├── sqlite3/     # SQLite3 头文件和库
└── lib/         # 其他动态库
```

详细说明请参考 [public/DEPENDENCIES.md](public/DEPENDENCIES.md)

## 许可证

MIT License
