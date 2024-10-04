// Observers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
using namespace std;
struct Observer
{
    int i;
    Observer(int val)
    {
        i = val;
    }
    void func()
    {
        cout << "Observer " << i << " observing\n";
    }
};

struct A {
    vector<Observer*> observers;

    void addObservers()
    {
        cout << "Adding observers\n";
        for (int i = 0; i < 4; i++)
        {
            Observer* observer = new Observer(i);
            observers.push_back(observer);
        }
    }
    void removeObservers()
    {
        cout << "Removing observers\n";
        observers.clear();
    }
    void func()
    {
        for (auto ob : observers)
            ob->func();
        cout << "in observed class\n";
    }
};
int main()
{
    A a;
    a.addObservers();
    a.func();
    a.removeObservers();
    cout << "Hello World!\n";
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
