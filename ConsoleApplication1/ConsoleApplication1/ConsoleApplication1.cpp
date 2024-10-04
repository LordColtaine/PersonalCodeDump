// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

void rotateleft(int arr[], int n)
{
    int temp = arr[0];
    for (int i = 1; i < n; i++)arr[i - 1] = arr[i];
    arr[n - 1] = temp;
}
void rotateright(int arr[], int n)
{
    int temp = arr[n-1];
    for (int i = n-1; i >0; i--)arr[i] = arr[i-1];
    arr[0] = temp;
}
void rotate(int arr[], int n, int dir, int steps)
{
    int i;
    if (dir)
    {
        for (i = 0; i < steps; i++)
        {
            rotateright(arr, n);
        }
    }
    else
    {
        for (i = 0; i < steps; i++)
        {
            rotateleft(arr, n);
        }
    }
}

void shift(int arr[], int n, int startpos)
{
    for (int i = startpos; i < n-1; i++)arr[i] = arr[i+1];
    arr[n - 1] = -1;
}
void insert(int arr[], int n, int val)
{
    int i;
    if (arr[n - 1] != -1)
    {
        cout << "Array full\n";
        return;
    }
    i = n - 1;
    while (i >= 0 && arr[i] == -1)i--;
    arr[++i] = val;
}
void remove(int arr[], int n, int pos)
{
    if (arr[pos] == -1)cout << "Element does not exist at this position\n";
    else
    shift(arr, n, pos);
}
void print(int arr[], int n)
{
    for (int i = 0; i < n; i++)cout << arr[i] << " ";
    cout << endl;
}
void search(int arr[], int n, int ele)
{
    for (int i = 0; i < n; i++)
        if (arr[i] == ele) {
            cout << "Element found at index " << i << endl;
            return;
        }
    cout << "Element not found in array\n";
}
int main()
{
    //!!!Prime Number!!!
    /*
    int n, res=0;
    cin>>n;
    _asm {
        mov ecx, n
        mov ebx, 2
        mov edx, 0

        next1:
        mov eax, ecx
        div ebx
        cmp edx, 0
        je notprime
        add ebx, 1
        mov eax, ebx
        mul ebx
        cmp eax, ecx
        jg prime
        jmp next1
    notprime:
        mov ecx, 0
        jmp finished
    prime:
        mov ecx, 1

            finished :
            mov res, ecx
        mov eax,0
    }
    cout << res << endl;
    */
    
    //!!!FizzBuzz!!!
    
    int n, flag = 0, three = 3, five = 5, fifteen = 15, val;
    cin >> n;
    _asm {
        
        mov eax, 0
        mov ebx, 1

        lab3:
        mov ecx, 0
        mov eax, ebx
        mov edx, 0
        div three
        cmp edx, 0
        je fizz

        lab5:
        mov eax, ebx
        mov edx, 0
        div five
        cmp edx, 0
        je buzz
        cmp ecx, 1
        je increment
        jmp printval

        fizz:
    }
    cout << "Fizz";
    _asm {
        mov ecx, 1
        jmp lab5

        buzz:
    }
    cout << "Buzz";
    _asm {
        jmp increment
        printval:
        mov val, ebx
    }
    cout << val;
    _asm {
    increment:

        inc ebx
    }

    cout << endl;
    _asm {
        cmp ebx, n
     jle lab3
     mov ecx, 0
    }
    
    
    /*
    int n, i;
    cout << "Enter size of array : ";
    cin >> n;
    int *arr = new int[n]();
    cout << "Enter elements :\n";
    for (i = 0; i < n; i++)cin >> arr[i];
    //ROTATION
    /*
    int  dir, steps;
    cout << "Enter direction (0 : left, 1 : right) and no. of steps :";
    cin >> dir >> steps;
    rotate(arr, n, dir, steps);
    cout << "Rotated array :\n";
    print(arr, n);
    */

    //Insert, delete, search
    /*
    int ch, pos, ele;
    do
    {
        cout << "1:Insert\n2:Delete\n3:Search\n4::Print\n5:Exit\nEnter choice : ";
        cin >> ch;
        switch (ch)
        {
        case 1:cout << "Enter value : "; cin >> ele;
            insert(arr, n, ele);
            break;
        case 2:cout << "Enter position : "; cin >> pos;
            remove(arr, n, pos);
            break;
        case 3:cout << "Enter element : "; cin >> ele;
            search(arr, n, ele); break;
        case 4:cout << "The Array is : \n"; print(arr, n); break;
        default: ch = 5;
        }
    } while (ch != 5);
    
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
