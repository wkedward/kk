# XCore - 核心服务

xcore 提供服务寻址、软件许可等核心功能。

## 功能特性

- 服务寻址
- 软件许可管理（集成 licensepp）

## 架构

```
xcore/
├── src/
│   ├── controllers/    # API 控制器
│   ├── models/         # 数据模型
│   ├── services/       # 业务逻辑
│   └── utils/          # 工具类
├── config/             # 配置文件
├── data/               # 数据目录
└── public/             # 依赖库
```
