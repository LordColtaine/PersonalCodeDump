// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_DEPRECATE
//#define SIZE_X 168
//#define SIZE_X 86
//#define SIZE_Y 43
#define SIZE_X 32
#define SIZE_Y 29
#include <iostream>
#include<Windows.h>
#include<WinBase.h>
#include "Scores.h"
using namespace std;
enum class Direction{LEFT, UP, RIGHT, DOWN};

int SCORE = 0;
int FOODTIMER = 20, NOFOODTIMER = 5, OBSTACLECOUNT = 3;
struct Body
{
    int m_x = 0;
    int m_y = 0;
    Body* m_next = nullptr;
    Body operator=(const Body b)
    {
        if (this != &b)
        {
            m_x = b.m_x;
            m_y = b.m_y;
            m_next = b.m_next;
        }
        return *this;
    }
};

struct Head
{
    int m_x = 0;
    int m_y = 0;
    Body* m_next = nullptr;
    Direction m_dir = Direction::DOWN;
};
Head Initialize()
{
    Head snake;
    snake.m_x = SIZE_X / 2;
    snake.m_y = SIZE_Y / 2;
    Body** b = new Body*[5];

    for (int i = 0; i < 5; i++)
        b[i] = new Body;

    b[0]->m_x = snake.m_x;
    b[0]->m_y = snake.m_y - 1;
    snake.m_next = b[0];
    for (int i = 0; i < 4; i++)
    {
        b[i]->m_next = b[i + 1];
        b[i + 1]->m_x = b[i]->m_x;
        b[i + 1]->m_y = b[i]->m_y-1;
    }
    b[4]->m_next = nullptr;
    return snake;
}
void update(Body*& body)
{
    if (!body->m_next)return;
    update(body->m_next);
    body->m_next->m_x = body->m_x;
    body->m_next->m_y = body->m_y;
}
void updateSnakePos(Head& snake)
{
    update(snake.m_next);
    snake.m_next->m_x = snake.m_x;
    snake.m_next->m_y = snake.m_y;
    if (snake.m_dir == Direction::DOWN)snake.m_y++;
    if (snake.m_dir == Direction::LEFT)snake.m_x--;;
    if (snake.m_dir == Direction::UP)snake.m_y--;
    if (snake.m_dir == Direction::RIGHT)snake.m_x++;
}
void removeFood(char arr[SIZE_Y][SIZE_X], pair<int, int> coords)
{
    arr[coords.second][coords.first] = ' ';
}
void increaseSnakeSize(Head &snake)
{
    Body* temp = snake.m_next;
    Body* extension = new Body;
    while (temp->m_next)
        temp = temp->m_next;
    temp->m_next = extension;
}
void eatFood(Head snake, pair<int,int> food, char arr[SIZE_Y][SIZE_X])
{
    if (snake.m_x == food.first && snake.m_y == food.second)
    {
        SCORE++;
        NOFOODTIMER = 5;
        FOODTIMER = 20;
        removeFood(arr, food);
        increaseSnakeSize(snake);
    }
}

bool updateScreen(Head snake, char arr[SIZE_Y][SIZE_X], pair<int, int> food)
{
    Body* temp = snake.m_next;
    while (temp)
    {
        arr[temp->m_y][temp->m_x] = 'o';
        temp = temp->m_next;
    }
    if ('0' == arr[snake.m_y][snake.m_x] || 'o' == arr[snake.m_y][snake.m_x])
    {
        cout << "YOU CRASHED!\n"; return false;
    }
    eatFood(snake, food, arr);
    arr[snake.m_y][snake.m_x] = 'o';
    if (0 == OBSTACLECOUNT)
    {
        OBSTACLECOUNT = 3;
        int x = rand() % (SIZE_X - 2) + 1;
        int y = rand() % (SIZE_Y - 2) + 1;
        arr[y][x] = '0';
    }
    OBSTACLECOUNT--;
    return true;
}

void revertScreen(Head snake, char arr[SIZE_Y][SIZE_X])
{
    Body* temp = snake.m_next;
    while (temp)
    {
        arr[temp->m_y][temp->m_x] = ' ';
        temp = temp->m_next;
    }
    arr[snake.m_y][snake.m_x] = ' ';
}

pair<int, int> addFood(char arr[SIZE_Y][SIZE_X])
{
    int x = rand() % (SIZE_X - 2) + 1;
    int y = rand() % (SIZE_Y - 2) + 1;
    arr[y][x] = '@';
    pair<int, int> coords(x, y);
    return coords;
}

int main()
{
    Head snake = Initialize();
    char arr[SIZE_Y][SIZE_X];
    for (int i = 0; i < SIZE_Y; i++)
    {
        for (int j = 0; j < SIZE_X; j++)
        {
            if (0 == i || 0 == j || SIZE_Y - 1 == i || SIZE_X - 1 == j)arr[i][j] = '0';
            else arr[i][j] = ' ';
        }
    }
    
    pair<int, int>food;
    while (1)
    {
        if (GetAsyncKeyState(VK_LEFT) && (snake.m_dir == Direction::DOWN || snake.m_dir == Direction::UP))
            snake.m_dir = Direction::LEFT;
        if (GetAsyncKeyState(VK_UP) && (snake.m_dir == Direction::LEFT || snake.m_dir == Direction::RIGHT))
            snake.m_dir = Direction::UP;
        if (GetAsyncKeyState(VK_RIGHT) && (snake.m_dir == Direction::DOWN || snake.m_dir == Direction::UP))
            snake.m_dir = Direction::RIGHT;
        if (GetAsyncKeyState(VK_DOWN) && (snake.m_dir == Direction::RIGHT || snake.m_dir == Direction::LEFT))
            snake.m_dir = Direction::DOWN;

        if(NOFOODTIMER)NOFOODTIMER--;
        if (NOFOODTIMER == 0)
        {
            if (20 == FOODTIMER)food = addFood(arr);
            FOODTIMER--;
            if (0 == FOODTIMER)
            {
                NOFOODTIMER = 5;
                FOODTIMER = 20;
                removeFood(arr, food);
            }
        }
        updateSnakePos(snake);

        if (!updateScreen(snake, arr, food))break;

        for (int i = 0; i < SIZE_Y; i++)
        {
            for (int j = 0; j < SIZE_X; j++)
            {
                if ('0' == arr[i][j])
                    printf("\u001b[36m%c", arr[i][j]);
                else if ('o' == arr[i][j])printf("\u001b[32m%c", arr[i][j]);
                else if ('@' == arr[i][j])printf("\u001b[31m%c", arr[i][j]);
                else printf("%c", arr[i][j]);
            }
                //cout << arr[i][j];
            //cout << endl;
            printf("\n");
        }
        Sleep(500);
        if (GetAsyncKeyState(VK_ESCAPE))break;
        revertScreen(snake, arr);
    }
    cout << "Score : " << SCORE << endl;
    writeToFile(SCORE);
    printTop5();
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
