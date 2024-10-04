// N Queens.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
using namespace std;


bool checkHorizontal(vector<vector<char>> grid, int curI, int curJ)
{
    for (int i = 0; i < grid.size(); i++)
    {
        if ('Q' == grid[curI][i]) return false;
    }
    return true;
}

bool checkVertical(vector<vector<char>> grid, int curI, int curJ)
{
    for (int i = 0; i < grid.size(); i++)
    {
        if ('Q' == grid[i][curJ]) return false;
    }
    return true;
}

bool checkLeftDiag(vector<vector<char>> grid, int curI, int curJ)
{
    int i = curI, j = curJ;
    while (i > 0 && j < grid.size()-1)i--, j++;
    while (i < grid.size() && j >= 0 )
    {
        if ('Q' == grid[i][j])return false;
        i++; j--;
    }
    return true;
}

bool checkRightDiag(vector<vector<char>> grid, int curI, int curJ)
{
    int i = curI, j = curJ;
    while (i > 0 && j > 0)i--, j--;
    while (i <grid.size()  && j < grid.size())
    {
        if ('Q' == grid[i][j])return false;
        i++; j++;
    }
    return true;
}

bool nQueens(vector<vector<char>> &grid, int curI = 0)
{
    int n = grid.size();
    if (curI == n)return true;
    for (int j = 0; j < n; j++)
    {
        if (checkHorizontal(grid, curI, j) && checkVertical(grid, curI, j) && checkLeftDiag(grid, curI, j) && checkRightDiag(grid, curI, j))
        {
            grid[curI][j] = 'Q';
            if (nQueens(grid, curI + 1))return true;
            grid[curI][j] = '-';
        }
    }
    return false;

}
int main()
{
    int n;
    cin >> n;
    vector<vector<char>>grid;
    for (int i = 0; i < n; i++)
    {
        vector<char> temp;
        for (int j = 0; j < n; j++)temp.push_back('-');
        grid.push_back(temp);
    }
    nQueens(grid);
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)cout<<grid[i][j]<<" ";
        cout << endl;
    }
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
