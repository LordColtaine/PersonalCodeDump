// Bricks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <vector>
#include<Windows.h>
#include<WinBase.h>
#include "Scores.h"
using namespace std;

#define SIZE_X 32
#define SIZE_Y 29

enum class Direction { UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT };

int BRICKCOUNT = 0;
struct Ball {
    int xpos, ypos;
    Direction dir = Direction::UPLEFT;
};
struct Board {
    int start;
    int end;
    int length;
};
void onHitTopWall(Ball &ball)
{
    if (Direction::UPLEFT == ball.dir)
    {
        ball.dir = Direction::DOWNLEFT;
        ball.xpos--;
        ball.ypos++;
    }
    else
    {
        ball.dir = Direction::DOWNRIGHT;
        ball.xpos++;
        ball.ypos++;
    }
}
void onHitLeftWall(Ball &ball)
{
    if (Direction::DOWNLEFT == ball.dir)
    {
        ball.dir = Direction::DOWNRIGHT;
        ball.xpos++;
        ball.ypos++;
    }
    else
    {
        ball.dir = Direction::UPRIGHT;
        ball.xpos++;
        ball.ypos--;
    }
}

void onHitRightWall(Ball& ball)
{
    if (Direction::DOWNRIGHT == ball.dir)
    {
        ball.dir = Direction::DOWNLEFT;
        ball.xpos--;
        ball.ypos++;
    }
    else
    {
        ball.dir = Direction::UPLEFT;
        ball.xpos--;
        ball.ypos--;
    }
}
void onHitBottomWall(Ball& ball)
{
    if (Direction::DOWNRIGHT == ball.dir)
    {
        ball.dir = Direction::UPRIGHT;
        ball.xpos++;
        ball.ypos--;
    }
    else
    {
        ball.dir = Direction::UPLEFT;
        ball.xpos--;
        ball.ypos--;
    }
}
void onHitBottomWall(Ball& ball, Board board)
{
    if ((board.end + board.start)/2 >=ball.xpos)
    {
        ball.dir = Direction::UPLEFT;
        ball.xpos--;
        ball.ypos--;
    }
    else
    {
        ball.dir = Direction::UPRIGHT;
        ball.xpos++;
        ball.ypos--;
    }
}
void moveBall(Ball& ball, Board board, vector<vector<char>> &grid)
{
    switch (ball.dir)
    {
    case Direction::DOWNLEFT:
        if (SIZE_Y - 2 == ball.ypos && ball.xpos >= board.start - 1 && ball.xpos <= board.end + 1)
        {
            if (1 == ball.xpos)
                ball.dir = Direction::UPRIGHT;
            else
                onHitBottomWall(ball, board);   
        }
        else if ('=' == grid[ball.ypos][ball.xpos -1])
        {
            grid[ball.ypos][ball.xpos-1] = ' ';
            BRICKCOUNT--;
            onHitLeftWall(ball);
        }
        else if ('=' == grid[ball.ypos+1][ball.xpos])
        {
            grid[ball.ypos+1][ball.xpos] = ' ';
            BRICKCOUNT--;
            onHitBottomWall(ball);
        }
        else if ('=' == grid[ball.ypos + 1][ball.xpos - 1])
        {
            grid[ball.ypos + 1][ball.xpos-1] = ' ';
            BRICKCOUNT--;
            ball.dir = Direction::UPRIGHT;
        }
        else if (1 == ball.xpos)
        {
            onHitLeftWall(ball);
        }
        else
        {
            ball.ypos++;
            ball.xpos--;
        }
        break;
    case Direction::DOWNRIGHT:
        if (SIZE_Y - 2 == ball.ypos && ball.xpos >= board.start - 1 && ball.xpos <= board.end + 1)
        {
            if (SIZE_X - 2 == ball.xpos)ball.dir = Direction::UPLEFT;
            else
                onHitBottomWall(ball, board);   
        }
        
        else if('=' == grid[ball.ypos][ball.xpos + 1])
        {
            grid[ball.ypos][ball.xpos + 1] = ' ';
            BRICKCOUNT--;
            onHitRightWall(ball);
        }
        else if ('=' == grid[ball.ypos+1][ball.xpos])
        {
            grid[ball.ypos+1][ball.xpos] = ' ';
            BRICKCOUNT--;
            onHitBottomWall(ball);
        }
        else if ('=' == grid[ball.ypos + 1][ball.xpos + 1])
        {
            grid[ball.ypos + 1][ball.xpos + 1] = ' ';
            BRICKCOUNT--;
            ball.dir = Direction::UPLEFT;
        }
        else if (SIZE_X - 2 == ball.xpos)
            onHitRightWall(ball);
        else {
            ball.xpos++;
            ball.ypos++;
        }
        break;
    case Direction::UPLEFT:
        if (1 == ball.ypos)
        {
            if (1 == ball.xpos)ball.dir = Direction::DOWNRIGHT;
            else
            {
                onHitTopWall(ball);
            }
        }
        
        else if ('=' == grid[ball.ypos][ball.xpos -1])
        {
            grid[ball.ypos][ball.xpos - 1] = ' ';
            BRICKCOUNT--;
            onHitLeftWall(ball);
        }
        else if ('=' == grid[ball.ypos -1][ball.xpos])
        {
            grid[ball.ypos -1][ball.xpos] = ' ';
            BRICKCOUNT--;
            onHitTopWall(ball);
        }
        else if ('=' == grid[ball.ypos - 1][ball.xpos - 1])
        {
            grid[ball.ypos - 1][ball.xpos - 1] = ' ';
            BRICKCOUNT--;
            ball.dir = Direction::DOWNRIGHT;
        }
        else if (1 == ball.xpos)
        {
            onHitLeftWall(ball);
        }
        else
        {
            ball.xpos--;
            ball.ypos--;
        }
        break;
    case Direction::UPRIGHT:
        if (1 == ball.ypos)
        {
            if (SIZE_X - 2 == ball.xpos) ball.dir = Direction::DOWNLEFT;
            else
            {
                onHitTopWall(ball);
            }
        }
        
        else if ('=' == grid[ball.ypos][ball.xpos+1])
        {
            grid[ball.ypos][ball.xpos+1] = ' ';
            BRICKCOUNT--;
            onHitRightWall(ball);
        }
        else if ('=' == grid[ball.ypos - 1][ball.xpos])
        {
            grid[ball.ypos-1][ball.xpos] = ' ';
            BRICKCOUNT--;
            onHitTopWall(ball);
        }
        else if ('=' == grid[ball.ypos - 1][ball.xpos + 1])
        {
            grid[ball.ypos - 1][ball.xpos+1] = ' ';
            BRICKCOUNT--;
            ball.dir = Direction::DOWNLEFT;
        }
        else if (SIZE_X - 2 == ball.xpos)
        {
            onHitRightWall(ball);
        }
        else
        {
            ball.xpos++;
            ball.ypos--;
        }
        break;
    }
}

int main()
{
    vector<vector<char>> grid;
    for (int i = 0; i < SIZE_Y; i++)
    {
        vector<char> temp;
        for (int j = 0; j < SIZE_X; j++)
        {
            if (0 == i)temp.push_back('_');
            else if (0 == j || SIZE_X - 1 == j)temp.push_back('|');
            else if (i >= 5 && i <= 20 && j >= 5 && j <= 20)
            {
                BRICKCOUNT++;
                temp.push_back('=');
            }
            else temp.push_back(' ');
        }
        grid.push_back(temp);
    }
    const int tot = BRICKCOUNT;
    int score = 0;
    Board board;
    board.length = 5;
    board.start = SIZE_X / 2 - 2;
    board.end = board.start + board.length-1;

    Ball ball;
    ball.xpos = (board.start +board.end)/2;
    ball.ypos = SIZE_Y - 2;
    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE))break;
        if (GetAsyncKeyState(VK_LEFT))
        {
            board.start = max(1, board.start - 1);
            board.end = board.start + board.length - 1;
        }
        if (GetAsyncKeyState(VK_RIGHT))
        {
            board.end = min(SIZE_X - 2, board.end + 1);
            board.start = board.end - board.length + 1;
        }


        //if (1 == start) end = LengthOfBoard, MidPos = (start + end)/2;
        //if (SIZE_X-2 == end) start = SIZE_X -1 - LengthOfBoard;
        for (int i = board.start; i <= board.end; i++)
            grid[SIZE_Y - 1][i] = 'T';


        moveBall(ball, board, grid);
        grid[ball.ypos][ball.xpos] = 'O';

        for (int i = 0; i < SIZE_Y; i++)
        {
            for (int j = 0; j < SIZE_X; j++)
            {
                if ('|' == grid[i][j] || '_' == grid[i][j])
                    cout << "\u001b[36m" << grid[i][j];
                else if ('T' == grid[i][j])
                    cout << "\u001b[33m" << grid[i][j];
                else if ('=' == grid[i][j])
                    cout << "\u001b[34m" << grid[i][j];
                else if ('O' == grid[i][j])
                    cout << "\u001b[32m" << grid[i][j];
                else
                    cout << grid[i][j];
            }
            if (0 == i)
            {
                score = tot - BRICKCOUNT;
                cout << "Score : " << score;
            }
            cout << endl;
        }
        if (SIZE_Y - 1 == ball.ypos)
        {
            cout << "GAME OVER\n";
            break;
        }
        for (int i = board.start; i <= board.end; i++)
            grid[SIZE_Y - 1][i] = ' ';
        grid[ball.ypos][ball.xpos] = ' ';

        Sleep(500);
    }
    cout << "Final Score : " << score << endl;
    writeToFile(score);
    printTop5();
    getchar();
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
