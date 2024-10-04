// Island.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

bool FindIslands(vector<vector<char>> &grid, int i, int j)
{
    if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size())return false;
    if (1 == grid[i][j])
    {
        grid[i][j] = 2;
        FindIslands(grid, i+1, j);
        FindIslands(grid, i-1, j);
        FindIslands(grid, i, j-1);
        FindIslands(grid, i, j+1);
        return true;
    }
    return false;
}
int main()
{
    vector<vector<char>> grid = { {1, 0, 0, 0, 0, 1},
                                  {0, 0, 0, 0, 0, 0},
                                  {0, 1, 1, 0, 0, 1},   
                                  {0, 1, 0, 0, 0, 1},
                                  {0, 1, 1, 0, 0, 0},
                                  {1, 0, 0, 0, 1, 1} };
    int count = 0;
    
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            cout << (int)grid[i][j] << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (FindIslands(grid, i, j))count++;
        }
    }
    
    cout << count << endl;
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
