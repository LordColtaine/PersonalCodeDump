// evening3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
using namespace std;
#include<stdarg.h>

void create()
{
#ifdef XBOX
        cout << "The xbox version is called\n"<<flush;
    #endif
#ifdef PS4
        cout << "PS4 version is called\n" << flush;
#endif
}
#define print(a, ...) printf(a, __VA_ARGS__)

#ifdef _DEBUG
#define ASSERT(a) if((a))abort();
#endif

#ifndef _DEBUG
#define ASSERT(a) cout<<"Not Debug"<<endl;
#endif

#define CONCAT(a, b) a##b //CONCATENATES THE TWO ARGUMENTS

void sum(int n, ...)
{
    va_list list;
    va_start(list, n);

}
int main()
{
    //create();
    //print("%d, %d", 5, 6);
    /*
    ASSERT(0 == 0);
    int i = 2;
    switch (i)
    {
    default: cout << "default\n";
        break;
    case 0: cout << "zero\n";
        break;
    case 2: cout << "one\n";
        break;
    }
    */

    FILE* f = fopen("a.txt", "w");
    fprintf(f, "writing...");
    fclose(f);
    return 0;
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
