// Pi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<thread>
#include<omp.h>
using namespace std;

double f(double x)
{
    if (0 == x)return 1;
    return sin(x) / x;
}
int main()
{
    double x, sum = 0, sumtemp;
    int numberofsteps = 1000;
    double h = 1.0 / numberofsteps;
//#pragma omp parallel for
    for (long int k = 0; ; k++)
    {
        sumtemp = 0;
#pragma omp parallel for
        for (long int i = 0; i < numberofsteps; i++)
        {
            sumtemp += f(k + i*h) * h;
        }
#pragma omp critical
        {
            sum += sumtemp;
            cout << sum*2 << endl;
        }
        //cout << sum * 2 << endl;
    }
    sum *= 2;
    
    
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
