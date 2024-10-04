// Virtual Functions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

class A
{
public:
	static void v_func()
	{
		printf("A::func()\n");
	}
	static void v_func1()
	{
		printf("A::func1()\n");
	}
	void (*(*vftl))();
	A(); ~A();
};
void (*A_func[10])() = { A::v_func,A::v_func1 };
A::A()
{
	vftl = A_func;  //notice that the pointer is set to A_func  (in constructor & destructor)
}
A::~A()
{
	vftl = A_func;
}
class A2 {
public:
	static void v_func()
	{
		printf("A2::func()\n");
	}
	static void v_func1()
	{
		printf("A2::func1()\n");
	}
	void (*(*vftl))();
	A2(); ~A2();
};
void (*A2_func[10])() = { A2::v_func,A2::v_func1 };
A2::A2()
{
	vftl = A2_func;
}
A2::~A2()
{
	vftl = A2_func;
}
class B :public A, public A2
{
public:
	static void v_func()
	{
		printf("B::func()\n");
	}
	static void v_func1()
	{
		printf("B::func1()\n");
	}
	//void (*(*vftl))(); //Since Vtable exists , no addition vtable pointer is needed to be declared
	B(); ~B();
};
void (*B_func[10])() = { B::v_func,B::v_func1 };
B::B()
{
	A::vftl = B_func;
	A2::vftl = B_func; //notice that the pointer is set to B_func  (in constructor & destructor)
}
B::~B()
{
	A::vftl = B_func;
	A2::vftl = B_func;
}
int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	B b;
	A* a = &b;
	A2* a2 = &b;
	(a->vftl[0])();
	(a2->vftl[0])();

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
