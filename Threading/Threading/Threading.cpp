// Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <Windows.h>
#include <vector>

using namespace std;
#define ROW1 3
#define COL1 3
#define ROW2 3
#define COL2 3
int A[ROW1][COL1], B[ROW2][COL2], C[ROW1][COL2];

//CRITICAL_SECTION CS;

DWORD WINAPI add(LPVOID parameters)
{
    
    int x = (size_t)parameters;
    int i = (x & 0xffff0000) >> 16;
    //cout <<"i : " << i;
    int j = x & 0xffff;
    //cout << " j: "<<j << endl;
    C[i][j] = A[i][j] + B[i][j];
    return 0;
}

DWORD WINAPI multiply(LPVOID parameters)
{
    int x = (size_t)parameters;
    int i = (x & 0xffff0000) >> 16;
    //cout <<"i : " << i;
    int j = x & 0xffff;
    //cout << " j: "<<j << endl;
    C[i][j] = 0;
    for (int k = 0; k < ROW2; k++)
        C[i][j] += A[i][k] * B[k][j];
    
    return 0;
}
int main()
{
    LPDWORD tid = 0;
    for (int i = 0; i < ROW1; i++)
    {
        for (int j = 0; j < COL1; j++)
        {
            A[i][j] = i * COL1 + j;
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < ROW2; i++)
    {
        for (int j = 0; j < COL2; j++)
        {
            if (i == j)B[i][j] = 1;
            else B[i][j] = 0;
            cout << B[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n\n\n";
    if (ROW1 == ROW2 && COL1 == COL2)
    {
        vector<HANDLE> handles;
        //InitializeCriticalSection(&CS);
        
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL2; j++)
            {
                size_t x = 0;
                x = (i << 16) | (j);
                //printf("%x\n", x);
                handles.push_back(CreateThread(0, 0, add, (void*)x, 0, tid));
                if (!handles[i * COL2 + j])return -1;
                //WaitForSingleObject(handles[i * COL1 + j], INFINITE);
            }
        }
        WaitForMultipleObjects(ROW1*COL2, &handles[0],true, INFINITE);
        //DeleteCriticalSection(&CS);
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL2; j++)
            {
                cout << C[i][j] << " ";
                CloseHandle(handles[i * COL2 + j]);
            }
            cout << endl;
        }
    }
    cout << endl;
    if (ROW2 == COL1)
    {
        vector<HANDLE> handles;
        
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL2; j++)
            {
                size_t x = 0;
                x = (i << 16) | (j);
                handles.push_back(CreateThread(0, 0, multiply, (void*)x, 0, tid));
                if (!handles[i * COL2 + j])return -1;
                //WaitForSingleObject(handles[i * COL2 + j], INFINITE);
            }
        }
        WaitForMultipleObjects(ROW1*COL2, &handles[0],true, INFINITE);
        //DeleteCriticalSection(&CS);
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL2; j++)
            {
                cout << C[i][j] << " ";
                CloseHandle(handles[i * COL2 + j]);
            }
            cout << endl;
        }
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
