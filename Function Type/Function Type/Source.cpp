#include<iostream>
#include"Header.h"
using namespace std;

inline void func()
{
	cout << "In source.cpp\n";
}

void func2()
{
	func();
	func3();
}