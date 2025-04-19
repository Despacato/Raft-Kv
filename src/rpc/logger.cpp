#include "logger.h"
#include <iostream>
#include <time.h>

Logger::Logger()
{
    std::thread writeLogTask([&]()
                             {
        while (true) {
            time_t now = time(nullptr);
            tm *localTime = localtime(&now);
            char file_name[128];
            sprintf(file_name, "%04d-%02d-%02d.txt", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday);
            
            FILE *pf = fopen(file_name, "a+");
            if(pf == nullptr)
            {
                std::cout << "open log file error!" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string logMsg = m_logQueue.Pop();  
            char time_buf[128] = {0};

            sprintf(time_buf, "%d:%d:%d =>[%s]", localTime->tm_hour, localTime->tm_min, localTime->tm_sec,(m_loglevel==INFO? "INFO":"ERROR"));
            
            logMsg.insert(0, time_buf);
            logMsg.append("\n");
            fputs(logMsg.c_str(), pf);
            fclose(pf); 
        } });

    writeLogTask.detach();
};

void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

void Logger::Log(std::string msg)
{
    m_logQueue.Push(msg);
}