// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>

#pragma data_seg("shared")
#define MAX 100
int arr[MAX] = { 0 };

#pragma data_seg()
#pragma comment(linker, "/section:shared,RWS")
__declspec(dllexport) void writeshared(int *out, int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = out[i];
}

__declspec(dllexport) void readshared(int *in, int n)
{
    for (int i = 0; i < n; i++)
        in[i] = arr[i];
}

__declspec(dllexport) int fndll1(int a, double b)
{
    printf("%d %lf\n", a, b);
        
    return 101;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
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

