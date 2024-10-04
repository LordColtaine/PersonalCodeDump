// Bits.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;


/*//Code for the function call in asm
typedef struct A {
	char x[4];
}a;
void __stdcall func(a a1)
{
	//no need to pop after call
	cout << "In func\n";
}
void func1(a a1)
{
//need to pop after call
	cout << "In func1\n";
}
*/

unsigned int add(unsigned int a, unsigned int b, bool subtract = false)
{
	unsigned int ans, carry;
	if (subtract)
	{
		b = add(~b, 1);
	}
	while (true)
	{
		ans = a ^ b;
		carry = a & b;
		carry = carry << 1;
		if (carry == 0)break;
		a = ans;
		b = carry;
	}
	return ans;
}
int main()
{
	unsigned int a = 0x44, b = 0x2f, carry = 0, ans = 0;
	printf("%x %x\n", a, b);
	ans = add(a, b);
	printf("%x\n", ans);
	ans = add(b, a, true);
	printf("%d\n", ans);
	
	//Types of function call
	/*
	_asm {
		mov eax, 1
		//pushing bytes equal to the size of argument
		push eax
		call func
		push eax
		call func1
		pop eax
	}
	*/


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
