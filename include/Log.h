#include <memory>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <Queue.hpp>
#include <condition_variable>
#include <string>
#include <ctime>
struct LogData
{
    std::string strTime;
    std::string strLogData;
    ~LogData(){ std::cout << "Destroyed!" << std::endl;}
};

class Log
{
private:
    Log(/* args */);
    static std::shared_ptr<Log> _instance;
    Queue<std::shared_ptr<LogData>> m_LogQueue;
    std::mutex condition_mutex;
    std::condition_variable condition_var;
    void LogThread();
    void WaitEvent();
    void SetEvent();
    std::string LocalTimeToString();
    struct tm * GetCurrentTime();
    std::string FormatLog(const std::shared_ptr<LogData> data);
public:
    Log(const Log&) = delete;
    Log operator=(const Log&) = delete;
    static std::shared_ptr<Log> GetInstance();
    void Write(const std::string &data);

    ~Log();

};


