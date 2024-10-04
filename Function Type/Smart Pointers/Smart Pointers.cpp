// Smart Pointers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
/*struct P {

    int count = 0;

    int* ptr = nullptr;
    void addRef()
    {
        count++;
    }
    void removeRef()
    {
        count--;
        if (0 == count)
        {
            delete this;
        }
    }
    ~P()
    {
        delete ptr;
    }
    void printCount()
    {
        cout << "Count : " << count << endl;
    }
};

struct A {
    P* p;
    A(int* i)
    {
        cout << "Constructor called\n";
        p = new P;
        //p->ptr = new int;
        p->ptr = i;
        p->addRef();
    }
    ~A()
    {
        p->removeRef();
    }
    A& operator =(const A& a)
    {
        cout << "Equalto called\n";
        if (this != &a)
        {
            p->removeRef();

            p = a.p;
            p->addRef();
        }
        return *this;
    }
};*/

template<typename T>
struct P
{
    int count = 0;

    T* ptr = nullptr;
    void addRef()
    {
        count++;
    }
    void removeRef()
    {
        count--;
        if (0 == count)
        {
            delete this;
        }
    }
    ~P()
    {
        delete ptr;
    }
    void printCount()
    {
        cout << "Count : " << count << endl;
    }
};

template <typename T>
struct A {
    P<T>* p;
    A()
    {
        p = new P<T>;
        p->ptr = nullptr;
    }
    A(T* i)
    {
        //cout << "Constructor called\n";
        p = new P<T>;
        //p->ptr = new int;
        p->ptr = i;
        p->addRef();
    }
    ~A()
    {
        p->removeRef();
    }
    A& operator =(const A& a)
    {
        //cout << "Equalto called\n";
        if (this != &a)
        {
            p->removeRef();

            p = a.p;
            p->addRef();
        }
        return *this;
    }
};

int main()
{
    int* p1 = new int(5);
    int* p2 = new int(10);
    A<int> a(p1);
    A<int> b(p2);
    b = a;
    a.p->printCount();

    char* c1 = new char('a');
    char* c2 = new char('b');
    A<char> c(c1);
    A<char> d(c2);

    d = c;
    a.p->printCount();
    /*A* c = b;
    A* d;
    d = c = b = a;
    a->p->printCount();*/
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
