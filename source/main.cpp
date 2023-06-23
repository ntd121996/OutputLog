#include <Log.h>

int main(int argc, const char** argv) 
{
    std::shared_ptr<Log> Log = Log::GetInstance();
    int i = 0;
    std::string strTest;
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        strTest = "Test" + std::to_string(i++);
        std::cout << "Add log\n";
        Log->Debug(strTest);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        strTest = "Test" + std::to_string(i++);
        std::cout << "Add log\n";
        Log->Info(strTest);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        strTest = "Test" + std::to_string(i++);
        std::cout << "Add log\n";
        Log->Error(strTest);
    }
    
    return 0;
}