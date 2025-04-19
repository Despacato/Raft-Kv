#pragma once
#include "lockqueue.h"

enum LogLevel
{
    INFO,
    ERROR,
};

class Logger
{
public:
    static Logger &GetInstance() // 在类外实现静态成员不需要static
    {
        static Logger instance;
        return instance;
    }
    void SetLogLevel(LogLevel level);
    void Log(std::string msg);

private:
    int m_loglevel;
    LockQueue<std::string> m_logQueue;

    Logger();
    Logger(const Logger &) = delete;
    Logger(const Logger &&) = delete;
};

#define LOG_INFO(LOGMSGFORMAT, ...)                          \
    do                                                       \
    {                                                        \
        Logger &logger = Logger::GetInstance();              \
        logger.SetLogLevel(INFO);                            \
        char c[1024];                                        \
        snprintf(c, sizeof(c), LOGMSGFORMAT, ##__VA_ARGS__); \
        logger.Log(c);                                       \
    } while (0)

#define LOG_ERR(LOGMSGFORMAT, ...)                           \
    do                                                       \
    {                                                        \
        Logger &logger = Logger::GetInstance();              \
        logger.SetLogLevel(ERROR);                           \
        char c[1024];                                        \
        snprintf(c, sizeof(c), LOGMSGFORMAT, ##__VA_ARGS__); \
        logger.Log(c);                                       \
    } while (0)