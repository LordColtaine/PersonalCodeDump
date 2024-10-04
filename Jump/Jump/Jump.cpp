// Jump.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600

#define TILE_SIZE 18

#define TOWER_HEIGHT 20
#define TOWER_WIDTH 16

struct Barrier {
    int open, close;
    bool right = true;
    int colornum;
    int speed;
    int row;
    Barrier()
    {
        right = true;
        open = 0, close = 1;
        colornum = rand() % 7 + 1;
        speed = 10;
    }
    Barrier(int op, int clos, int sp, int row_, bool dir = true)
    {
        right = dir;
        open = op;
        close = clos;
        colornum = rand() % 7 + 1;
        speed = sp;
        row = row_;
    }
};

vector<vector<int>> grid;
int main()
{
    for (int i = 0; i < TOWER_HEIGHT; i++)
    {
        vector<int> temp;
        for (int j = 0; j < TOWER_WIDTH; j++)
        {
            temp.push_back(0);
        }
        grid.push_back(temp);
    }
    RenderWindow window(VideoMode(800, 600), "Jump");
    window.setFramerateLimit(20);
    Texture t;
    if (!t.loadFromFile("images\\tiles.png"))return 1;

    Sprite s(t);
    pair<int, int> player;
    
    Sprite playersp(t);
    playersp.setTextureRect(IntRect(108, 0, 18, 18));
    
    int timer = 0;
    int jumpdist = 0;
    bool jumped = false;
    int prevrow;
    vector<Barrier> barriers;
    Barrier floor(0, 0, 10, TOWER_HEIGHT - 1);
    bool dirright = true;


    for (int i = 0; i<3; i++)
    {
        Barrier barrier;
        barrier.open = 0;
        barrier.close = 3 + rand() % 4;
        barrier.speed = 10 + rand() % 11;
        barrier.right = dirright;
        barrier.row = (i+1)* TOWER_HEIGHT/4;
        
        dirright = !dirright;
        barriers.push_back(barrier);
    }
    barriers.push_back(floor);

    player.first = 0;
    player.second = floor.row-1;

    bool moveGrid = false;
    int movetimer = 6;

    int stoptime = 2;
    playersp.setPosition(player.first * TILE_SIZE, player.second*TILE_SIZE);

    Font font;
    if (!font.loadFromFile("images\\arialbd.ttf"))return 1;

    Text jumper, scoretext;
    jumper.setFont(font);
    jumper.setCharacterSize(40);
    jumper.setOutlineColor(Color::White);
    jumper.setFillColor(Color::Red);
    jumper.setPosition(Vector2f(3 * WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 4));
    jumper.setString("Jumper");

    scoretext.setFont(font);
    scoretext.setCharacterSize(35);
    scoretext.setOutlineColor(Color::White);
    scoretext.setFillColor(Color::White);
    scoretext.setPosition(Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2));

    int Score = 0;
    while (window.isOpen())
    {
        if (1500 == timer)timer = 0;
        timer++;

        bool jump = false;
        int dx=0;
        Event event;
        while (window.pollEvent(event))
        {
            if (Event::Closed == event.type)
                window.close();

            if (Event::KeyPressed == event.type)
            {
                if (Keyboard::Escape == event.key.code)
                    window.close();
                if (Keyboard::Left == event.key.code)
                {
                    if (player.first >= 1)dx = -1;
                }
                if (Keyboard::Right == event.key.code)
                {
                    if (player.first < TOWER_WIDTH - 1)dx = 1;
                }
                if (Keyboard::Space == event.key.code)
                    jump = true;
            }
        }

        
        if (!moveGrid)
        {
            for (int i = 0; i < barriers.size(); i++)
            {
                if (0 == timer % barriers[i].speed)
                {
                    if (barriers[i].right)
                    {
                        barriers[i].open++;
                        barriers[i].close++;
                        if (barriers[i].close >= TOWER_WIDTH)
                        {
                            barriers[i].close = 0;
                        }
                        if (barriers[i].open >= TOWER_WIDTH)
                        {
                            barriers[i].open = 0;
                        }
                    }
                    else
                    {
                        barriers[i].open--;
                        barriers[i].close--;
                        if (barriers[i].close < 0)
                        {
                            barriers[i].close = TOWER_WIDTH - 1;
                        }
                        if (barriers[i].open < 0)
                        {
                            barriers[i].open = TOWER_WIDTH - 1;
                        }
                    }
                }
            }
            if (dx)
            {
                player.first += dx;
                stoptime = 0;
            }
            else if (stoptime < 3)stoptime++;
            else
            {
                if (barriers[barriers.size() - 1].open != barriers[barriers.size() - 1].close && !jump && !jumpdist)
                {
                    if (barriers[barriers.size() - 1].right)
                    {
                        if (player.first < TOWER_WIDTH - 1 && 0 == timer % barriers[barriers.size() - 1].speed)player.first++;
                    }
                    else
                    {
                        if (player.first > 1 && 0 == timer % barriers[barriers.size() - 1].speed)player.first--;
                    }
                }
            }

            pair<int, int>tempPlayer = player;


            tempPlayer.second++;
            if (jump && !jumpdist)
            {
                jumpdist = 6;
                jump = false;
            }
            bool canmove = true;
            for (int i = 0; i < barriers.size(); i++)
            {
                if (barriers[i].row == tempPlayer.second)
                {
                    int x = tempPlayer.first;
                    if (barriers[i].open <= barriers[i].close)
                    {
                        if (x >= barriers[i].open && x < barriers[i].close)continue;
                        canmove = false;
                        
                        break;
                    }
                    else
                    {
                        if (x >= barriers[i].close && x < barriers[i].open)
                        {
                            canmove = false;
                            break;
                        }
                    }
                }
            }
            if (canmove)
            {
                player = tempPlayer;
            }
            else
            {
                if (jumped)
                {
                    if (player.second != barriers[barriers.size()-1].row - 1)
                    {
                        moveGrid = true;
                        movetimer = 3;
                        Score++;
                    }
                    jumped = false;
                }
            }
            canmove = true;
            if (jumpdist > 0)
            {
                jumpdist--;
                tempPlayer.second -= 2;
                for (int i = 0; i < barriers.size(); i++)
                {
                    if (barriers[i].row == tempPlayer.second)
                    {
                        int x = tempPlayer.first;
                        if (barriers[i].open <= barriers[i].close)
                        {
                            if (x >= barriers[i].open && x < barriers[i].close)continue;
                            canmove = false;
                            break;
                        }
                        else
                        {
                            if (x >= barriers[i].close && x < barriers[i].open)
                            {
                                canmove = false;
                                break;
                            }
                        }
                    }
                }
                if (canmove)
                {
                    player = tempPlayer;
                    if (0 == jumpdist)
                    {
                        jumped = true;
                    }
                }
                else
                    jumpdist = 0;
            }

            if (player.second >= TOWER_HEIGHT * 1.5 - 1)
            {
                cout << "GAME OVER\n";
                window.close();
            }
        }
        else
        {
        player.second++;
        for (int i = 0; i < barriers.size(); i++)
            barriers[i].row++;
            if (1 == movetimer)
            {
                barriers.pop_back();
                Barrier barrier;
                barrier.open = 0;
                barrier.close = 3 + rand() % 4;
                barrier.speed = 10 + rand() % 11;
                barrier.right = dirright;
                barrier.row = TOWER_HEIGHT / 4;

                dirright = !dirright;
                barriers.insert(barriers.begin(), barrier);

                player.second = barriers[barriers.size() - 1].row - 1;
                for (int i = 0; i < barriers.size(); i++)
                {
                    barriers[i].row = barrier.row = (i + 1) * TOWER_HEIGHT / 4;
                }
            }
            movetimer--;
            if (0 == movetimer)
            {
                moveGrid = false;
            }
        }
        playersp.setPosition(player.first* TILE_SIZE, player.second* TILE_SIZE);
        scoretext.setString("Your score : " + to_string(Score));
        window.clear();

        for (int i = 0; i<barriers.size(); i++)
        {
            s.setTextureRect(IntRect(barriers[i].colornum * 18, 0, 18, 18));

            if (barriers[i].open <= barriers[i].close)
            {
                for (int k = 0; k < barriers[i].open; k++)
                {
                    s.setPosition(k * TILE_SIZE, barriers[i].row * TILE_SIZE);
                    window.draw(s);
                }
                for (int k = barriers[i].close; k < TOWER_WIDTH; k++)
                {
                    s.setPosition(k * TILE_SIZE, barriers[i].row * TILE_SIZE);
                    window.draw(s);
                }
            }
            else {
                for (int k = barriers[i].close; k < barriers[i].open; k++)
                {
                    s.setPosition(k * TILE_SIZE, barriers[i].row * TILE_SIZE);
                    window.draw(s);
                }
            }
            
        }
        window.draw(jumper);
        window.draw(scoretext);
        window.draw(playersp);
        window.display();
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
