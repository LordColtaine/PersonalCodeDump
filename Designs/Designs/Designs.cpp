// Designs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<mutex>
using namespace std;
enum class Type{WARNING_1, WARNING_2, ERROR};
struct Logger
{
    static Logger& getLogger()
    {
        Logger logger;
        return logger;
    }
    void Log(Type t)
    {
        //FILE *f = fopen("")
        switch (t)
        {
        case Type::WARNING_1 : cout << "WARNING1\n";
            break;
        case Type::WARNING_2: cout << "WARNING2\n";
            break;
        case Type::ERROR: cout << "ERROR\n";
            break;
        }
    }
private:
    Logger(){}
    Logger operator=(Logger &logger){}
};
int main()
{
    Logger logger1 = Logger::getLogger();
    logger1.Log(Type::WARNING_1);
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
