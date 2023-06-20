#include <Log.h>

int main(int argc, const char** argv) 
{
    std::shared_ptr<Log> Log = Log::GetInstance();
    int i = 0;
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::string strTest = "Test" + std::to_string(i++);
        Log->Write(strTest);

    }
    
    return 0;
}