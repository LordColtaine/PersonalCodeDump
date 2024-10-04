// Inheritance.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

struct A {
    int i;
};
struct B {
    int j;
};
struct C :A, B {

};

struct D :A {
    int k;
};
struct E :D {

};

union color32 {
    unsigned int color;
    struct Channels {
        unsigned char r, g, b, a;
    } channels;
};

union color16 {
    unsigned short color;
    struct Channels {
        unsigned char b : 5;
        unsigned char g : 6;
        unsigned char r : 5;
    } channels;
};
int main()
{
    C c;
    A* a = &c;
    B* b = &c;
    cout << b << " "<<a<<endl;
    
    E e;
    D* d = &e;
    A* a2 = &e;
    cout << d << " " << a2 << endl;

    color32 color1;
    color1.color = 0xFF112233;
    cout << "red : " << (unsigned short)color1.channels.r << endl;
    cout << "green : " << (unsigned short)color1.channels.g << endl;
    cout << "blue : " << (unsigned short)color1.channels.b << endl;
    cout << "alpha : " << (unsigned short)color1.channels.a << endl;
    
    color16 color2;
    color2.color = 0x1111;
    //cout << "red : " << (unsigned short)color2.channels.r << endl;
    cout << "green : " << (unsigned short)color2.channels.g << endl;
    cout << "blue : " << (unsigned short)color2.channels.b << endl;
    
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
