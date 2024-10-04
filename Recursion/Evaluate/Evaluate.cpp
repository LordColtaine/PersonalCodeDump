// Evaluate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
using namespace std;
int evaluate(char* str)
{
    int len = strlen(str);
    int index=0, i;
    char left[64], right[64];
    bool MinusFlag = false;
    for (i = 0; i != len ; i++)
    {
        if (str[i] == '+' || i!=0 && str[i] == '-')break;
        left[i] = str[i];
    }
    left[i] == '\0';
    if (i == len)
    {
        for (i = 0; i != len; i++)
        {
            if (str[i] == '*' || str[i] == '/' || str[i] == '%' )break;
            left[i] = str[i];
        }
        left[i] = '\0';
        
        if (i == len)
        {
            return atoi(left);
        }
        else
        {
            int c = 0;
            char op = str[i++];
            for (; i < len; i++)
            {
                right[c++] = str[i];
            }
            right[c] = '\0';
            switch (op)
            {
            case '*' :return atoi(left) * evaluate(right);
            case '/':return atoi(left) / evaluate(right);
            case '%':return atoi(left) % evaluate(right);
            }
        }
    }
    else
    {
        left[i] = '\0';
        int c = 0;
        char op = str[i++];
        if (op == '-')i--;
        for (; i < len; i++)
        {
            right[c++] = str[i];
        }
        right[c] = '\0';
        //cout << right << endl;
        switch (op)
        {
        case '+': return evaluate(left) + evaluate(right);
        case '-': return evaluate(left) + evaluate(right);
        }
    }
}
int main()
{
    char str[64] = "-10-4*20/5+10";
    cout << evaluate(str) << endl << flush;
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
