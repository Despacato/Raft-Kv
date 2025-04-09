#pragma once
#include <unordered_map>
#include <string>
// 读取文件配置类

class MprpcConfig
{
public:
    // 负责加载配置文件
    void LoadConfigFile(const char *configfile);
    // 查询配置项信息
    std::string Load(const std::string &key);

private:
    std::unordered_map<std::string, std::string> m_configMap; // 配置文件的键值对
};