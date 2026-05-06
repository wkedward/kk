# KSMGR - 综合管理系统

基于微服务架构的综合管理平台，提供多种业务能力和基础设施服务。

## 系统架构

```
ksmgr/
├── 自研服务
│   ├── ksmgr.xbase.service     # 基础服务（数据字典、配置管理、权限管理、授时）
│   ├── ksmgr.xcore.service     # 核心服务（服务寻址、软件许可）
│   ├── ksmgr.xlog.service      # 日志服务
│   ├── ksmgr.xapi.service      # API 网关
│   ├── ksmgr.xres.service      # 资源服务
│   ├── ksmgr.xmedia.service    # 媒体服务
│   ├── ksmgr.xmap.service      # 地图服务
│   ├── ksmgr.xdocs.service     # 文档服务
│   ├── ksmgr.xdrf.service      # 动态表单报表
│   ├── ksmgr.xflow.service     # 工作流服务
│   ├── ksmgr.xbrst.service     # 业务规则服务
│   └── ksmgr.xjob.service      # 任务调度服务
├── 第三方服务
│   ├── xxl-job
│   ├── xxl-api
│   ├── xxl-conf
│   ├── licensepp
│   ├── datac
│   └── dbopx
└── 开源中间件
    ├── minio
    ├── redis
    ├── postgres
    ├── rabbitmq
    ├── kafka
    └── elasticsearch
```

## 项目结构

```
/workspace/
├── xasst/                    # 文件服务（已存在）
├── ksmgr/                    # 综合管理系统
│   ├── ksmgr.xbase.service/
│   ├── ksmgr.xcore.service/
│   ├── ksmgr.xlog.service/
│   ├── ksmgr.xapi.service/
│   ├── ksmgr.xres.service/
│   ├── ksmgr.xmedia.service/
│   ├── ksmgr.xmap.service/
│   ├── ksmgr.xdocs.service/
│   ├── ksmgr.xdrf.service/
│   ├── ksmgr.xflow.service/
│   ├── ksmgr.xbrst.service/
│   ├── ksmgr.xjob.service/
│   ├── third-party/
│   ├── public/
│   └── docs/
└── README.md
```

## 服务说明

### 自研服务

| 服务名 | 功能说明 |
|-------|---------|
| ksmgr.xbase.service | 基础服务：数据字典、配置管理、权限管理、授时服务 |
| ksmgr.xcore.service | 核心服务：服务寻址、软件许可 |
| ksmgr.xlog.service | 日志服务：日志收集、分析、查询 |
| ksmgr.xapi.service | API 网关：路由、限流、监控 |
| ksmgr.xres.service | 资源服务：静态资源托管 |
| ksmgr.xmedia.service | 媒体服务：文件处理、转码、水印 |
| ksmgr.xmap.service | 地图服务：地理编码、路径规划 |
| ksmgr.xdocs.service | 文档服务：文档管理、在线协作 |
| ksmgr.xdrf.service | 动态表单报表：表单设计、报表生成 |
| ksmgr.xflow.service | 工作流服务：流程编排、任务管理 |
| ksmgr.xbrst.service | 业务规则服务：规则定义、规则引擎 |
| ksmgr.xjob.service | 任务调度服务：定时任务、任务监控 |

### 第三方服务

| 服务名 | 功能说明 |
|-------|---------|
| xxl-job | 分布式任务调度平台 |
| xxl-api | API 管理平台 |
| xxl-conf | 配置中心 |
| licensepp | 软件许可框架 |
| datac | 数据采集工具 |
| dbopx | 数据库操作平台 |

### 开源中间件

| 组件名 | 功能说明 |
|-------|---------|
| minio | 对象存储 |
| redis | 缓存、消息队列 |
| postgres | 关系型数据库 |
| rabbitmq | 消息队列 |
| kafka | 消息队列、流处理 |
| elasticsearch | 搜索、日志分析 |

## 快速开始

1. 查看各服务的 README 文档了解详情
2. 根据需求启动相应的服务
3. 配置服务间通信

## 开发规范

- 自研服务命名：`ksmgr.<服务名>.service`
- 使用 Drogon 作为 Web 框架（C++ 服务）
- 配置文件统一使用 JSON 格式
- 数据库使用 SQLite 或 PostgreSQL
