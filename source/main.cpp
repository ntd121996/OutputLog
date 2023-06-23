#include <Log.h>


int main(int argc, const char** argv) 
{
    int i = 0;
    std::string strTest;
    while (i< 25)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        strTest = "Test" + std::to_string(i++);
        std::cout << "Add log\n";
        TRACE(strTest);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        strTest = "Test" + std::to_string(i++);
        std::cout << "Add log\n";
        INFO(strTest);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        strTest = "Test" + std::to_string(i++);
        std::cout << "Add log\n";
        ERROR(strTest);
    }
    
    return 0;
}