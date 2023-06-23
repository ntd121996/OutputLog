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

void Log::Write(const std::string &data, LogKind logKind)
{
    std::shared_ptr<LogData> logData = std::make_shared<LogData>();
    logData->strTime = LocalTimeToString();
    logData->strLogData = data;
    logData->uiKind = logKind;
    this->m_LogQueue.Enqueue(logData);
    SetEvent();
}

Log::Log()
{
    LogIndex = 1;
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
    
    struct tm *timeInfo;
    stTime = GetCurrentTime();
    std::string logFileName = MakeLogFileName();
    while (1)
    {
        WaitEvent();
        timeInfo = GetCurrentTime();
        if( (logSize > LOG_SIZE) || 
            (timeInfo->tm_mday != stTime->tm_mday) ||
            (timeInfo->tm_mon != stTime->tm_mon) ||
            (timeInfo->tm_year != stTime->tm_year))
        {
            *stTime = *timeInfo;
            if(LogIndex++ == 999) LogIndex = 0;
            logFileName = MakeLogFileName();
        }
        std::shared_ptr<LogData> logData = this->m_LogQueue.Dequeue();
        std::fstream fs(logFileName, std::fstream::app | std::fstream::in | std::fstream::out);
        if(!fs.fail())
        {
            fs << FormatLog(logData);
            fs.seekg (0, fs.end);
            logSize = fs.tellg();
            fs.close();
        }
        std::cout << "Write log success" << std::endl;
    }
    
}

void Log::WaitEvent()
{
    std::unique_lock<std::mutex> lock(this->condition_mutex);
    std::cout << "Wait add log" << std::endl;
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

std::string Log::FormatLog(const std::shared_ptr<LogData> &data)
{
    std::stringstream ss;
    std::string strKind;
    switch (data->uiKind)
    {
    case LogKind::DEBUG:
        strKind = "[DEBUG] ";
        break;
    case LogKind::INFO:
        strKind = "[INFO] ";
        break;
    case LogKind::ERROR:
        strKind = "[ERROR] ";
        break;
    default:
        strKind = "[UNKNOW] ";
        break;
    }
    ss << strKind;
    ss << data->strTime << " ";
    ss << data->strLogData;
    ss << "\r\n";
    return ss.str();
}

std::string Log::MakeLogFileName()
{
    std::stringstream ss;
    ss << std::to_string(stTime->tm_year);
    ss << std::setw(2) << std::setfill('0') << std::to_string(stTime->tm_mon);
    ss << std::to_string(stTime->tm_mday);
    ss << "-";
    ss << std::setw(3) << std::setfill('0') << std::to_string(LogIndex);
    ss << ".log";
    return ss.str();
}

Log::~Log()
{
}

void Log::Info(const std::string &data)
{
    Write(data, LogKind::INFO);
}

void Log::Debug(const std::string &data)
{
    #ifdef ENABLE_DEBUG
    Write(data, LogKind::DEBUG);
    #endif
}

void Log::Error(const std::string &data)
{
    Write(data, LogKind::ERROR);
}
