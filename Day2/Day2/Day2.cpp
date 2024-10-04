// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
using namespace std;

int length(char str[])
{
    int i;
    for (i = 0; str[i] != '\0'; i++);
    return i;
}

void transpose(int arr[][5], int m, int n, int trans[][5])
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            trans[j][i] = arr[i][j];
        }
    }
}
void removerow(int arr[][5], int m, int n, int pos)
{
    for (int i = pos + 1; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            arr[i - 1][j] = arr[i][j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        arr[m - 1][i] = -1;
    }
}

void removecolumn(int arr[][5], int m, int n, int pos)
{
    int trans[5][5];
    transpose(arr, m, n, trans);
    removerow(trans, n, m, pos);
    transpose(trans, n, m, arr);
}


void addrow(int arr[][5], int m, int n, int pos, int row[])
{
    if (arr[m - 1][0] != -1)
    {
        cout << "Matrix is full\n" << flush;
        return;
    }
    for (int i = m - 1; i > pos; i--)
    {
        for (int j = 0; j < n; j++)
            arr[i][j] = arr[i - 1][j];
    }
    for (int j = 0; j < n; j++)
    {
        arr[pos][j] = row[j];
    }
}
void addcolumn(int arr[][5], int m, int n, int pos, int column[])
{
    if (arr[0][n - 1] != -1)
    {
        cout << "matrix is full\n" << flush;
        return;
    }
    int trans[5][5];
    transpose(arr, m, n, trans);
    addrow(trans, n, m, pos, column);
    transpose(trans, n, m, arr);

}
void print(int arr[][5], int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            cout << arr[i][j]<<" ";
        cout << endl<<flush;
    }
}

void rotateanti(int arr[5][5], int m, int n, int out[][5])
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            out[i][j] = arr[j][m - 1 - i];
        }
    }
}
void copy(int arr[][5], int m, int n, int dup[][5])
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            dup[i][j] = arr[i][j];
}
void spiral(int arr[][5], int m, int n)
{
    int out[5][5], temp[5][5];
    copy(arr, m, n, out);
    int i=0, j, count = 0;
    while (i < m/ 2)
    {
        copy(out, m, n, temp);
        for (j = i; j < n - 1 - i; j++)
        {
            cout << temp[i][j]<<" ";
        }
        cout << endl;
        count++;
        if (count == 4)
        {
            count = 0;
            i++;
        }
        rotateanti(temp, m, n, out);
    }
    if (m % 2)
    {
        cout << arr[m / 2][n / 2] << endl << flush;
    }

}
void replace(char s[], char subs[], char newstr[])
{
    int slen = length(s);
    int sublen = length(subs);
    if (slen < sublen)
    {
        cout << "Substring is bigger than string\n";
        return;
    }
    int i=0, j;
    while (i + sublen <= slen)
    {
        j = 0;
        while (j < sublen)
        {
            if (subs[j] != s[i + j])break;
            j++;
        }
        if (j == sublen)
        {
            for (j = 0; j < sublen; j++)
            {
                s[i + j] = newstr[j];
            }
            return;
        }
        i++;
    }
}
int main()
{
    /*int m1, n1, m2, n2;
    cin >> m1 >> n1 >> m2 >> n2;
    int **a = new int** ()[m1];
    */
    int m1, n1, m2, n2;

    //MATRIX ADDITION
    /*
    m1 = 3, m2 = 3, n1 = 3, n2 = 3;
    int a[3][3] = { {1, 2, 3},{4, 5, 6},{7, 8, 9} };
    int b[3][3] = { {1, 0, 0},{0,1,0},{0,0,1} };
    int c[3][3];
    if (m1 == m2 && n1 == n2)
    {
        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < n1; j++)
            {
                c[i][j] = a[i][j] + b[i][j];
            }
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                cout << c[i][j] << " "<<flush;
            cout << endl;
        }

    }
    */

    //Transpose
    /*
    m1 = 3; n1 = 2;
    int arr[3][2] = { {1, 2},{3, 4}, {5, 6} };
    int transpose[2][3];
    for (int i = 0; i < m1; i++)
    {
        for (int j = 0; j < n1; j++)
        {
            transpose[j][i] = arr[i][j];
        }
    }
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m1; j++)
            cout << transpose[i][j] << " " << flush;
        cout << endl << flush;
    }
    */

    //Multiplication
    /*
    m1 = 2, n1 = 3, m2 = 3, n2 = 1;
    int a[2][3] = { {1, 1, 1},{1, 1, 1}};
    int b[3][1] = { {1}, {1}, {1} };
    if (n1 == m2)
    {
        int c[2][1];
        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < n2; j++)
            {
                c[i][j] = 0;
                for (int k = 0; k < m2; k++)
                {
                    c[i][j] += a[i][k] + b[k][j];
                }
            }
        }
        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < n2; j++)
                cout << c[i][j] << " " << flush;
            cout << endl << flush;
        }
    }
    */
    //Add/remove Row/Col
    /*
    int m = 5, n = 5;
    int arr[5][5] = { {1, 1, 1, 1, 1}, {2, 2, 2, 2, 2}, {3, 3, 5, 3, 3}, {4, 4, 4, 4, 4}, {5, 5, 5, 5, 5} };
    int row[5] = { 10, 10, 10, 10, 10 };
    print(arr, m, n);
    int index = 2;
    cout << "removing row with index " << index << endl;
    removerow(arr, m, n, index);
    //cout << "removing column with index " << index << endl;
    //removecolumn(arr, m, n, index);
    print(arr, m, n);
    index = 3;
    cout << "adding row at index " << index << endl;
    addrow(arr, m, n, index, row);
    //cout << "adding col at index " << index << endl;
    //addcolumn(arr, m, n, index, row);
    print(arr, m, n);

    //removecolumn(arr, m, n, 1);
    //print(arr, m, n);
    */

    //SPIRAL
    
    //int arr[][5] = { {1, 1, 1, 1, 1}, {2, 2, 2, 2, 2}, {3, 3, 3, 3, 3}, {4, 4, 4, 4, 4}, {5, 5, 5, 5, 5} };
    int m = 5, n = 5;
    int arr[][5] = { {1, 1, 1, 1}, {2, 2, 2, 2}, {3, 3, 3, 3}, {4, 4, 4, 4} };
    m = n = 4;
    print(arr, m, n);
    cout << endl;
    spiral(arr, m, n);
    


    //String functions
    /*
    char str1[] = "string1";
    char str2[] = "string2";
    int len1 = length(str1);
    int len2 = length(str2);
    char str3[20];
    for (int i = 0; i < len1; i++)
    {
        str3[i] = str1[i];
    }
    for (int i = 0; i < len2; i++)
    {
        str3[len1 + i] = str2[i];
    }
    str3[len1 + len2] = '\0';
    //cout << str3 << endl << flush;
    printf("%s\n", str3); fflush(stdout);

    int len3 = len1 + len2;
    char rev[20];
    for (int i = 0; i < len3; i++)
    {
        rev[i] = str3[len3 - 1 - i];
        //cout << str3[len3 - i];
    }
    rev[len3] = '\0';
    cout << rev << endl << flush;
    //printf("%s\n", rev);
    
    char s[] = "abcdefg";
    char subs[] = "bcd";
    char news[] = "xyz";
    cout << s << endl;
    replace(s, subs, news);
    cout << s << endl<<flush;
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
