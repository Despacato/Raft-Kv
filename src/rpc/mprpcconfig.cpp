#include <iostream>
#include "mprpcconfig.h"

void MprpcConfig::LoadConfigFile(const char *configfile)
{
    FILE *fp = fopen(configfile, "r");
    if (fp == nullptr)
    {
        std::cout << configfile << "is not exist" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (!feof(fp))
    {
        char buffer[512] = {0};
        fgets(buffer, 512, fp);
        std::string line(buffer);
        // 去掉行末的换行符
        line.erase(line.find_last_not_of("\n") + 1);
        // 去掉行首的空格
        line.erase(0, line.find_first_not_of(" "));
        if (line.empty() || line[0] == '#')
            continue;
        // 查找等号的位置
        size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        m_configMap.insert({key, value});
    }
}

std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        std::cout << key << "is not exist" << std::endl;
        return "";
    }
    return m_configMap[key];
}