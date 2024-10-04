// Recursion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

int len(char* str)
{
    if (str[0] == '\0')return 0;
    return 1 + len(str+1);
}

void reverse(char* str)
{
    if(str[0] == '\0')return;
    reverse(str + 1);
    cout << str[0];
}

int fib(int n)
{
    if (n == 0 || n == 1) return 0;
    return fib(n - 1) + fib(n - 2);
}

int factorial(int n)
{
    if (n < 0) return -1;
    if (n == 0 || n == 1)return 1;
    return n * factorial(n - 1);
}
int pow(int n, int p)
{
    if (0 == p)return 1;
    if (1 == p) return n;
    int j = pow(n, p / 2);;
    if ((p & 1))
        return j * j * n;
    return j * j;
 }

void towerofhanoi(char s, char a, char d, int n)
{
    if (n == 1)cout << "moving 1 from " << s << " to " << d << endl;
    else
    {
        towerofhanoi(s, d, a, n - 1);
        towerofhanoi(s, a, d, 1);
        towerofhanoi(a, s, d, n - 1);
    }
}
int sum(vector<int> v)
{
    int sum = 0;
    for (int i = 0; i < v.size(); i++)
        sum += v[i];
    return sum;
}
void combo(vector<int> v, vector<int> v1)
{
    
    if (sum(v) == sum(v1))
    {
        for (int i = 0; i < v.size(); i++)
        {
            cout << v[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < v1.size(); i++)
        {
            cout << v1[i] << " ";
        }
        cout << endl << endl<<flush;
        
        return;
    }
    for (int i = 0; i < v.size(); i++)
    {
        vector<int> t = v;
        v1.push_back(*(t.begin() + i));
        t.erase(t.begin() + i);

        combo(t, v1);
        
        v1.pop_back();

    }
}

int main()
{
    //char str[] = "abcdefg";
    //cout << len(str) << endl << flush;
    //reverse(str);
    //cout << pow(2, 5) << " " << pow(2, 4) << endl << flush;
    //towerofhanoi('s', 'a', 'd', 3);
    vector<int> v = { 9, 8, 1, 2, 3, 4, 7 }, v1;
    combo(v, v1);

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
