// Castle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include<queue>
using namespace std;
int minimumMoves(vector<string> grid, int startX, int startY, int goalX, int goalY) {
    
    if (startX == goalX && startY == goalY)return 0;
    pair<int, pair<int, int>> v;
    vector<vector<bool>> visited(grid.size());
    for (int i = 0; i < grid.size(); i++)
    {
        visited[i] = vector<bool>(grid[i].length());
        fill(visited[i].begin(), visited[i].end(), false);
    }
    queue<pair<int, pair<int, int>>> q;
    v.first = 1;
    v.second.first = startX;
    v.second.second = startY;
    q.push(v);
    visited[startX][startY] = true;
    while (!q.empty())
    {
        v = q.front();
        
        if (v.second.first == goalX && v.second.second == goalY)
        {
            
            return v.first;
        }
        q.pop();
        int dist = v.first;
        int i, j;

        i = v.second.first;
        j = v.second.second;
        while (i >= 0)
        {
            if ('X' == grid[i][j]) break;
            i--;
        }
        i++;
        if (!visited[i][j]) {
            visited[i][j] = true;
            v.first = dist + 1;
            v.second.first = i;
            v.second.second = j;
            q.push(v);
        }

        i = v.second.first;
        j = v.second.second;
        while (i <grid.size())
        {
            if ('X' == grid[i][j]) break;
            i++;
        }
        i--;
        if (!visited[i][j]) {
            visited[i][j] = true;
            v.first = dist + 1;
            v.second.first = i;
            v.second.second = j;
            q.push(v);
        }

        i = v.second.first;
        j = v.second.second;
        while (j >= 0)
        {
            if ('X' == grid[i][j]) break;
            j--;
        }
        j++;
        if (!visited[i][j]) {
            visited[i][j] = true;
            v.first = dist + 1;
            v.second.first = i;
            v.second.second = j;
            q.push(v);
        }

        i = v.second.first;
        j = v.second.second;
        while (j < grid.size())
        {
            if ('X' == grid[i][j]) break;
            j++;
        }
        j--;
        if (!visited[i][j]) {
            visited[i][j] = true;
            v.first = dist + 1;
            v.second.first = i;
            v.second.second = j;
            q.push(v);
        }
    }
    return -1;
}
int main()
{
    vector<string>grid = { ".X.X", "....", "..X.","X..." };
    for (int i = 0; i < grid.size(); i++)
    {

    }
    int n = minimumMoves(grid, 0, 2, 2, 0);
    cout << "Min jumps : " << n << endl;

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
