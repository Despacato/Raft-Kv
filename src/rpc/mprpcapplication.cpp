#include <iostream>
#include "mprpcapplication.h"
#include <unistd.h>

MprpcConfig MprpcApplication::m_config; // 静态成员变量的定义

void ShowArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
    if (argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    while ((c = getopt(argc, argv, "i:")) != -1)
    {
        std::string config_file;
        switch (c)
        {
        case 'i':
            // 读取配置文件
            config_file = optarg; // optarg
            break;
        case '?':

            std::cout << "invalid option: " << std::endl;
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            std::cout << "need <configfile>" << std::endl;
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }

        // 开始加载配置文件
        // 静态方法没有this指针，不能访问类内的具体实例 所以m_config也需要是静态的
        m_config.LoadConfigFile(config_file.c_str());

        std::cout << "rpcsercerverip: " << m_config.Load("rpcserverip") << std::endl;
        std::cout << "rpcsercerport: " << m_config.Load("rpcserverport") << std::endl;
    }
}

MprpcApplication &MprpcApplication::GetInstance()
{
    static MprpcApplication instance;
    return instance;
}
MprpcConfig &MprpcApplication::GetConfig()
{
    return m_config;
}