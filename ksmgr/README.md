# KSMGR 综合管理系统

基于微服务架构的企业级综合管理平台。

## 目录结构

```
ksmgr/
├── ksmgr.xbase.service/     # 基础服务
├── ksmgr.xcore.service/     # 核心服务
├── ksmgr.xlog.service/      # 日志服务
├── ksmgr.xapi.service/      # API 网关
├── ksmgr.xres.service/      # 资源服务
├── ksmgr.xmedia.service/    # 媒体服务
├── ksmgr.xmap.service/      # 地图服务
├── ksmgr.xdocs.service/     # 文档服务
├── ksmgr.xdrf.service/      # 动态表单报表
├── ksmgr.xflow.service/     # 工作流服务
├── ksmgr.xbrst.service/     # 业务规则服务
├── ksmgr.xjob.service/      # 任务调度服务
├── third-party/             # 第三方服务
│   ├── xxl-job
│   ├── xxl-api
│   ├── xxl-conf
│   ├── licensepp
│   ├── datac
│   └── dbopx
├── public/                  # 公共依赖
└── docs/                    # 项目文档
```

## 服务说明

### 自研服务（Public Services）

自研服务采用 `ksmgr.<服务名>.service` 的命名规范。

#### 基础服务
- **ksmgr.xbase.service**: 提供数据字典、配置管理、权限管理、授时服务等基础功能。

#### 核心服务
- **ksmgr.xcore.service**: 提供服务寻址、软件许可管理等核心功能。

#### 基础设施服务
- **ksmgr.xlog.service**: 日志收集、存储、分析和查询。
- **ksmgr.xapi.service**: API 网关、路由管理、请求限流。
- **ksmgr.xres.service**: 资源管理、静态资源托管。

#### 业务服务
- **ksmgr.xmedia.service**: 媒体文件处理、转码、水印。
- **ksmgr.xmap.service**: 地图数据管理、地理编码。
- **ksmgr.xdocs.service**: 文档管理、在线协作、版本控制。
- **ksmgr.xdrf.service**: 动态表单设计、报表生成。
- **ksmgr.xflow.service**: 工作流定义、流程编排。
- **ksmgr.xbrst.service**: 业务规则定义、规则引擎。
- **ksmgr.xjob.service**: 任务调度、定时任务。

### 第三方服务

- **xxl-job**: 分布式任务调度平台
- **xxl-api**: API 管理平台
- **xxl-conf**: 配置中心
- **licensepp**: 软件许可框架
- **datac**: 数据采集工具
- **dbopx**: 数据库操作平台

## 开发指南

1. 进入具体的服务目录
2. 查看服务的 README.md 了解功能特性和架构
3. 按照服务的开发规范进行开发

## 注意事项

- 各服务应保持松耦合，通过 API 进行通信
- 建议使用统一的配置中心管理配置
- 日志应统一收集到 xlog 服务
