#pragma once
#include "mprpcconfig.h"
// mprpc框架的基础类

class MprpcApplication
{
public:
    // 解析命令行参数，初始化rpc框架
    static void Init(int argc, char **argv);
    static MprpcApplication &GetInstance();
    static MprpcConfig &GetConfig();

private:
    static MprpcConfig m_config;
    MprpcApplication() {}
    MprpcApplication(const MprpcApplication &) = delete;
    MprpcApplication(const MprpcApplication &&) = delete;
};