// Clouds.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

int noofjumps(vector<int> arr, int index)
{
    if (index == arr.size()-1 || index == arr.size())return 0;

    if (1 == arr[index])return INT_MAX;
    //cout << index << endl;
    int onestep = noofjumps(arr, index + 1);
    int twosteps = noofjumps(arr, index + 2);

    if (INT_MAX == onestep && INT_MAX == twosteps)
    {
        return INT_MAX;
    }
    return 1 + min(onestep, twosteps);
}
int main()
{
    vector<int> arr = {0, 1, 0, 1, 1, 0, 1, 0};
    int jumps = noofjumps(arr, 0);
    if (INT_MAX == jumps)
        cout << "Not possible" << endl << flush;
    else
        cout << jumps << endl << flush;
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
