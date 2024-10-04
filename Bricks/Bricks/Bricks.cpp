// Bricks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include<Windows.h>
#include<WinBase.h>

using namespace std;

#define SIZE_X 3
#define SIZE_Y 3

enum class Direction {UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT};

struct Ball {
    int m_xpos;
    int m_ypos;
    int m_model = '0';
    Direction dir;
    void onHit(Ball ball)
    {
        ;
    }
};

struct Object {
    int m_xpos;
    int m_ypos;
    char m_model;
    virtual void printGrid() = 0;
    
    
    virtual void onHit(Ball ball) = 0;
};


struct Space : Object {
    Space()
    {
        m_model = ' ';
    }
    Space(int x, int y)
    {
        m_model = ' ';
        m_xpos = x;
        m_ypos = y;
    }
    void onHit(Ball ball)
    {
        if (Direction::UPLEFT == ball.dir)
        {
            ball.m_xpos--;
            ball.m_ypos--;
        }
        if (Direction::DOWNRIGHT == ball.dir)
        {
            ball.m_xpos++;
            ball.m_ypos++;
        }
        if (Direction::DOWNLEFT == ball.dir)
        {
            ball.m_xpos++;
            ball.m_ypos--;
        }
        if (Direction::UPRIGHT == ball.dir)
        {
            ball.m_xpos--;
            ball.m_ypos++;
        }
    }
    void printGrid()
    {
        cout << (char)m_model;
    }
};

struct Wall : Object {
    Wall()
    {
        m_model = '#';
    }
    Wall(int x, int y)
    {
        m_model = '#';
        m_xpos = x;
        m_ypos = y;
    }
    void onHit(Ball ball)
    {
        if (Direction::UPLEFT == ball.dir)
        {
            if (0 == m_ypos)
            {
                ball.dir = Direction::DOWNLEFT;
                ball.m_xpos++;
                ball.m_ypos--;
            }
            else
            {
                ball.dir = Direction::UPRIGHT;
                ball.m_xpos--;
                ball.m_ypos++;
            }
        }
        if (Direction::DOWNRIGHT == ball.dir)
        {
            ball.dir = Direction::DOWNLEFT;
            ball.m_xpos++;
            ball.m_ypos--;
        }
        if (Direction::DOWNLEFT == ball.dir)
        {
            ball.dir = Direction::DOWNRIGHT;
            ball.m_xpos++;
            ball.m_ypos++;
        }
        if (Direction::UPRIGHT == ball.dir)
        {
            if (0 == m_ypos)
            {
                ball.dir = Direction::DOWNRIGHT;
                ball.m_xpos++;
                ball.m_ypos++;
            }
            else
            {
                ball.dir = Direction::UPLEFT;
                ball.m_xpos--;
                ball.m_ypos--;
            }
        }
    }
    void printGrid()
    {
        cout << (char)m_model;
        if (SIZE_Y - 1 == m_ypos);
            //cout <<m_xpos<<" "<<m_ypos<< endl;
    }
};
int main()
{
    vector<vector<Object*>> objectgrid;
    for (int i = 0; i < SIZE_Y; i++)
    {
        vector<Object*> temp;
        for (int j = 0; j < SIZE_X; j++)
        {
            if (0 == i || SIZE_X - 1 == j || 0 == j)
            {
                Wall wall(j, i);
                Object* object = &wall;
                
                temp.push_back(object);
                object = nullptr;
                delete object;
            }
            else
            {
                Space space(j, i);
                Object* object = &space;
                temp.push_back(object);
                object = nullptr;
                delete object;
            }
        }
        objectgrid.push_back(temp);
        for (int j = 0; j < temp.size(); j++)
        {
            temp[j] = nullptr;
            delete temp[j];
        }
    }
    for (int i = 0; i < SIZE_Y; i++)
    {
        for (int j = 0; j < SIZE_X; j++)
        {
            //cout << objectgrid[i][j]->m_xpos << objectgrid[i][j]->m_ypos << endl;
            objectgrid[i][j]->printGrid();
            ;
        }
    }
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
