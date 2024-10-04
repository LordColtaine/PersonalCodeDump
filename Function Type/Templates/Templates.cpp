// Templates.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

template<int i>
struct Fact
{
    enum
    {
        val = i * Fact<i - 1>::val
    };
};
template<>
struct Fact<1>
{
    enum {
        val = 1
    };
};

template<int i>
struct Fib
{
    enum
    {
        val = Fib<i - 1>::val + Fib<i-2>::val
    };
};
template<>
struct Fib<1>
{
    enum {
        val = 1
    };
};
template<>
struct Fib<0>
{
    enum {
        val = 1
    };
};

template<typename T>
struct A {

};
template<template<typename T> typename A>
struct B {
    A<char> a;
};

template<template<template<typename T>typename A>typename B>
struct C {
    B<A> b;
};


int add()
{
    return 0;
}
template <typename T1, typename ...T>
int add(T1 a1, T ...a)
{
    //sizeof(a...) will give the number of arguments left
    return a1 + add(a...);
}


template <typename T1, typename ...T>
struct X:X<T...> {
    T1 x;
    X<T...> *getNext()
    {
        return this;
    }
};

template<typename T1>
struct X<T1> {
    T1 x;
};
int main()
{
    
    cout << Fact<5>::val << endl;;
    cout << Fib<5>::val << endl;
    //A<char> a;
    //B<A> b;
    //C<B> c;
    
    X<int, int, int> M;
    //M.getNext()->getNext()->x;
    cout << add(1, 2, 3, 4, 5) << endl;
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
