// linking2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <Windows.h>
using namespace std;
int main()
{
    HMODULE h = LoadLibrary(TEXT("C:\\Users\\K Raviraju Prasanna\\source\\repos\\Dll1\\Debug\\Dll1.dll"));
    if (h)
    {
        cout << "Loaded" << endl;
        //int (*fptr)(int, double) = (int(*)(int, double))GetProcAddress(h, "?fndll1@@YAHHN@Z");
        void (*read)(int*, int) = (void (*)(int*, int))GetProcAddress(h, "?readshared@@YAXPAHH@Z");
        void (*write)(int*, int) = (void (*)(int*, int))GetProcAddress(h, "?writeshared@@YAXPAHH@Z");

        if (read && write)
        {
            int n = 10, arr2[10];

            
            read(arr2, 10);
            for (int i = 0; i < n; i++)
            {
                cout << arr2[i] << " ";
            }
            cout << endl << flush;
        }
        FreeLibrary(h);

    }
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
