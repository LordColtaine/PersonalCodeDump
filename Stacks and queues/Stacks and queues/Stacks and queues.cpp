// Stacks and queues.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string.h>
using namespace std;

struct stack
{
    int* arr;
    int top;
    stack()
    {
        arr = NULL;
        top = -1;
    }
    stack(const stack& s)
    {

        delete[] arr;
        arr = new int[s.top + 1];
        memcpy(arr, s.arr, sizeof(int) * (s.top + 1));
        top = s.top;
    }
    stack& operator =(const stack& s)
    {
        if (this != &s)
        {
            delete[] arr;
            arr = new int[s.top + 1];
            memcpy(arr, s.arr, sizeof(int) * (s.top + 1));
            top = s.top;
        }
        return *this;
    }
    ~stack()
    {
        delete[] arr;
    }
    bool empty()
    {
        return (top == -1);
    }
    void push(int a)
    {
        if (empty())
        {
            arr = new int;
            arr[++top] = a;
            return;
        }
        top++;
        int* t = new int[(top+1)];
        memcpy(t, arr, top * sizeof(int));
        delete[] arr;
        t[top] = a;
        arr = t;
        t = NULL;
    }

    void printstack()
    {
        for (int i = 0; i <= top; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
    void pop()
    {
        if (empty())
        {
            cout << "Stack is empty\n";
            return;
        }
        int* t = new int[top];
        memcpy(t, arr, top * sizeof(int));
        delete[] arr;
        top--;
        arr = t;
        t = NULL;
    }
};

struct queue 
{
    int* arr;
    int f, r;
    queue()
    {
        f = r = -1;
        arr = NULL;
    }
    ~queue()
    {
        delete[] arr;
    }
    bool empty()
    {
        return (-1 == r);
    }
    void enqueue(int a)
    {
        if (empty())
        {
            arr = new int;
            arr[++r] = a;
            return;
        }
        r++;
        int* t = new int[(r + 1)];
        memcpy(t, arr, r * sizeof(int));
        delete[] arr;
        t[r] = a;
        arr = t;
        t = NULL;
    }
    void printqueue()
    {
        for (int i = 0; i <= r; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
    void dequeue()
    {
        if (empty())
        {
            cout << "Queue is empty\n";
            return;
        }
        int* t = new int[r];
        memcpy(t, arr+1, r * sizeof(int));
        delete[] arr;
        r--;
        arr = t;
        t = NULL;
    }
};
int main()
{
    stack s;
    s.push(5); s.printstack();
    s.push(10); s.printstack();
    s.pop(); s.printstack();

    queue q;
    q.enqueue(4);
    q.enqueue(8);
    q.enqueue(12);
    q.enqueue(16); q.printqueue();
    q.dequeue();
    q.dequeue(); q.printqueue();
    
    
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
