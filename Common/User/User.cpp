// User.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

using namespace std;
int main()
{
    HMODULE h = LoadLibrary(TEXT("C:\\Users\\K Raviraju Prasanna\\source\\repos\\Common\\Debug\\Common.dll"));
    if (h)
    {
        cout << "Loaded" << endl;
        //int (*fptr)(int, double) = (int(*)(int, double))GetProcAddress(h, "?fndll1@@YAHHN@Z");
        int (*read)() = (int (*)())GetProcAddress(h, "?read@@YAHXZ");
        void (*write)(int) = (void (*)(int))GetProcAddress(h, "?write@@YAXH@Z");

        if (read && write)
        {
            write(500);
            cout << read() << endl;
        }
        FreeLibrary(h);
    }
    else cout << "Not Loaded\n";
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
