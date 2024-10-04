// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<omp.h>
#include<thread>
#include<Windows.h>
#include<vector>
using namespace std;

#define ROW1 10
#define COL1 10
#define ROW2 1
#define COL2 10
int M[ROW1][COL1];
int N[ROW2][COL2];
int res[ROW1][COL2] = { 0 };

void hi(int i)
{
    //Sleep(1000);
    cout << i << endl;
}
void add(int i, int j)
{
    res[i][j] = M[i][j] + N[i][j];
}
void multiply(int i, int j)
{
    res[i][j] = 0;
    for (int k = 0; k < ROW2; k++)
    {
        res[i][j] += M[i][k] + N[k][j];
    }
}
int main()
{
#pragma omp parallel for
    for (int i = 0; i < ROW1; i++)
    {
        for (int j = 0; j < COL1; j++)
        {
            M[i][j] = i * COL1 + j;
        }
    }
#pragma omp parallel for
    for (int i = 0; i < ROW2; i++)
    {
        for (int j = 0; j < COL2; j++)
        {
            N[i][j] = ROW2 - 1;
        }
    }
#if ROW1 == ROW2 && COL1 == COL2
/*
#pragma omp parallel for
    
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL1; j++)
            {
                add(i, j);
            }
        }
        */
    for (int i = 0; i < ROW1; i++)
    {
        for (int j = 0; j < COL1; j++)
        {
            thread(add, i, j).join();
            //threads.push_back(t);
        }
    }
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL2; j++)cout << res[i][j] << " ";
            cout << endl;
        }
#endif
#if ROW2 == COL1
//#pragma omp parallel for
//
//        for (int i = 0; i < ROW1; i++)
//        {
//            for (int j = 0; j < COL2; j++)
//            {
//                    multiply(i, j);
//            }
//        }
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL2; j++)
                thread(multiply, i, j).join();
        }
        for (int i = 0; i < ROW1; i++)
        {
            for (int j = 0; j < COL2; j++)cout << res[i][j] << " ";
            cout << endl;
        }
#endif
#pragma omp parallel for
        for (int i = 0; i < 50; i++)
        {
            //thread(hi, i).join();
            hi(i);
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
