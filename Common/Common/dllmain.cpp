// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include<atomic>
using namespace std;
#pragma data_seg("shared")
int value[1] = { 0 };
atomic<int> READCOUNT = 0;
bool ISWRITING = false;
#pragma data_seg()
#pragma comment(linker, "/section:shared,RWS")

HANDLE wrt;// , mutex;
__declspec(dllexport) void write(int val)
{
    while (READCOUNT > 0);
    WaitForSingleObject(wrt, INFINITE);
    ISWRITING = true;
    value[0] = val;
    Sleep(5000);
    ISWRITING = false;
    ReleaseMutex(wrt);
    //ReleaseMutex(wrt);
}

__declspec(dllexport) int read()
{
    
    //WaitForSingleObject(mutex, INFINITE);
    while (ISWRITING);
    READCOUNT++;
    //if (1 == READCOUNT)
    //    WaitForSingleObject(wrt, INFINITE);
    //ReleaseMutex(mutex);
    int retval = value[0];
    Sleep(2000);
    //WaitForSingleObject(mutex, INFINITE);
    READCOUNT--;
    //if (0 == READCOUNT)
    //    ReleaseMutex(wrt);
    //ReleaseMutex(mutex);
    return retval;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    wrt = CreateMutexA(0, false, "writer");
    //mutex = CreateMutexA(0, false, "mutex");
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

