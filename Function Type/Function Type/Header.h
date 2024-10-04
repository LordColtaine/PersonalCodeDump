#pragma once
#include<iostream>
using namespace std;

inline void func3()
{
	static int y = 0;
	cout << ++y << endl;
	cout << "Header\n";
}

struct A {
	void funcn()
	{
		cout << "ABCD\n";
	}
};