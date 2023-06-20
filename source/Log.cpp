#include "Log.h"
#include <sstream>
#include <iomanip>
#include <fstream>
std::shared_ptr<Log> Log::_instance = nullptr;
std::shared_ptr<Log> Log::GetInstance()
{
    static std::mutex mutex;
    static std::lock_guard<std::mutex> lock(mutex);
    if(!_instance)
    {
        _instance = std::shared_ptr<Log>(new Log());
    }
    return _instance;
}

void Log::Write(const std::string &data)
{
    std::shared_ptr<LogData> logData = std::make_shared<LogData>();
    logData->strTime = LocalTimeToString();
    logData->strLogData = data;
    this->m_LogQueue.Enqueue(logData);
    SetEvent();
}

Log::Log()
{
    std::thread *thread = new std::thread(&Log::LogThread, this);
    if(thread == nullptr)
    {
        std::cerr << __func__ << ":" << "Create thread error" << std::endl;
    }
    thread->detach();
}

#define LOG_SIZE 1024 * 1024
void Log::LogThread()
{
    std::cout << "Create Thread success" << std::endl;
    int logSize = 0;
    int index = 1;
    struct tm *oldTime, *timeInfo;
    oldTime = GetCurrentTime();
    std::string logFileName;
    std::stringstream ss;
    ss << std::to_string(oldTime->tm_year);
    ss << std::setw(2) << std::setfill('0') << std::to_string(oldTime->tm_mon);
    ss << std::to_string(oldTime->tm_mday);
    ss << "-";
    ss << std::setw(3) << std::setfill('0') << std::to_string(index);
    ss << ".log";
    logFileName = ss.str();
    ss.clear();
    ss.str("");
    while (1)
    {
        WaitEvent();
        timeInfo = GetCurrentTime();
        if( (logSize > LOG_SIZE) || 
            (timeInfo->tm_mday != oldTime->tm_mday) ||
            (timeInfo->tm_mon != oldTime->tm_mon) ||
            (timeInfo->tm_year != oldTime->tm_year))
        {
            *oldTime = *timeInfo;
            if(index++ == 999) index = 0;
            ss << std::to_string(oldTime->tm_year);
            ss << std::setw(2) << std::setfill('0') << std::to_string(oldTime->tm_mon);
            ss << std::to_string(oldTime->tm_mday);
            ss << "-";
            ss << std::setw(3) << std::setfill('0') << std::to_string(index);
            ss << ".log";
            logFileName = ss.str();
            ss.clear();
            ss.str("");
        }
        std::shared_ptr<LogData> logData = this->m_LogQueue.Dequeue();
        std::fstream fs(logFileName.c_str(), std::ios_base::app | std::ios_base::in | std::ios_base::out);
        if(!fs.fail())
        {
            fs << FormatLog(logData);
            fs.seekg (0, fs.end);
            logSize = fs.tellg();
            fs.close();
        }
    }
    
}

void Log::WaitEvent()
{
    std::unique_lock<std::mutex> lock(this->condition_mutex);
    std::cout << "Wait" << std::endl;
    this->condition_var.wait(lock);
    

}

void Log::SetEvent()
{
    this->condition_var.notify_one();
}

std::string Log::LocalTimeToString()
{
    using namespace std::chrono;
    std::stringstream ss;
    time_point<system_clock> nowTime = system_clock::now();
    time_point<system_clock>::duration duration = nowTime.time_since_epoch();
    const long long milisec = std::chrono::duration_cast<milliseconds>(duration).count();
    const std::time_t time = system_clock::to_time_t(nowTime);
    ss << std::put_time(std::localtime(&time), "%Y/%m/%d %H:%M:%S.");
    ss << std::setw(3) << std::setfill('0') << (milisec % 1000);
    return ss.str();
}


tm *Log::GetCurrentTime()
{
    time_t rawtime;
    time (&rawtime);
    struct tm *timeInfo = localtime(&rawtime);
    timeInfo->tm_year += 1900;
    timeInfo->tm_mon += 1;
    return timeInfo;
}

std::string Log::FormatLog(const std::shared_ptr<LogData> data)
{
    std::stringstream ss;
    ss << data->strTime;
    ss << " " + data->strLogData;
    ss << "\r\n";
    return ss.str();
}

Log::~Log()
{
}