// isMatched3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<SFML\Graphics.hpp>

using namespace std;
using namespace sf;


#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600

#define TILE_SIZE 18

#define BOARD_SIZE_X 10
#define BOARD_SIZE_Y 10

#define OFFSET 20
struct Block {
    int color;
    int row, col;
    bool isMatched = false;
    Block()
    {
        color = row = col = -1;
    }
    Block(int row_, int col_, int color_)
    {
        row = row_;
        col = col_;
        color = color_;
    }
};
vector<vector<Block>> grid;
vector<vector<bool>> visited;

void initGrid()
{
    for (int i = 0; i < BOARD_SIZE_Y; i++)
    {
        vector<Block> temp;
        vector<bool> temp2;
        for (int j = 0; j < BOARD_SIZE_X; j++)
        {
            temp.push_back(Block(i, j, rand()%7+1));
            temp2.push_back(false);
        }
        grid.push_back(temp);
        visited.push_back(temp2);
    }
}

void swap(Block a, Block b)
{
    std::swap(a.col, b.col);
    std::swap(a.row, b.row);

    grid[a.row][a.col] = a;
    grid[b.row][b.col] = b;
}



int main()
{
    initGrid();
    
    RenderWindow window(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Match 3");
    window.setFramerateLimit(30);
    Texture t;
    Sprite s(t);
    if (!t.loadFromFile("images\\tiles.png"))return 1;
    Vector2i pos;
    int mouseclickcount = 0;
    int x0, y0, x1, y1;
    int swapcount = 0;
    int Score = 0;
    bool CountScore = false;

    Font font;
    if (!font.loadFromFile("images\\arialbd.ttf"))return 1;

    Text Match3, scoretext;
    Match3.setFont(font);
    Match3.setCharacterSize(40);
    Match3.setOutlineColor(Color::White);
    Match3.setFillColor(Color::Red);
    Match3.setPosition(Vector2f(3 * WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 4));
    Match3.setString("Match 3");

    scoretext.setFont(font);
    scoretext.setCharacterSize(35);
    scoretext.setOutlineColor(Color::White);
    scoretext.setFillColor(Color::White);
    scoretext.setPosition(Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2));

    while (window.isOpen())
    {
        bool con = true;
        Event event;
        while (window.pollEvent(event))
        {
            if (Event::Closed == event.type)
            {
                window.close();
            }
            if (Event::KeyPressed == event.type)
            {
                if (Keyboard::Escape == event.key.code)
                    window.close();
            }
            if (Event::MouseButtonPressed == event.type)
            {
                if (Mouse::Left == event.key.code)
                {
                    mouseclickcount++;
                    pos = Mouse::getPosition();
                }

            }
        }
        if (1 == mouseclickcount)
        {
            CountScore = true;
            x0 = pos.x / (TILE_SIZE) + 1 - 17;
            y0 = pos.y / (TILE_SIZE) + 1 - 7;
        }
        
        if (2 == mouseclickcount)
        {
            x1 = pos.x / (TILE_SIZE) + 1 - 17;
            y1 = pos.y / (TILE_SIZE) + 1 -7;

            if (1 == abs(x1 - x0) + abs(y1 - y0))
            {
                int c = 0;
                swap(grid[y0][x0], grid[y1][x1]);
                swapcount++;
            
            }
            mouseclickcount = 0;
        }
        for (int i = 1; i <= BOARD_SIZE_Y - 2; i++)
        {
            for (int j = 0; j <= BOARD_SIZE_X - 1; j++)
            {
                if (grid[i][j].color == grid[i + 1][j].color)
                    if (grid[i][j].color == grid[i - 1][j].color)
                    {
                        for (int n = -1; n <= 1; n++) grid[i + n][j].isMatched++;
                        if (mouseclickcount)
                            swapcount = 1;
                    }
            }
        }
        for (int i = 0; i < BOARD_SIZE_Y; i++)
        {
            for (int j = 1; j < BOARD_SIZE_X - 1; j++)
            {
                if (grid[i][j].color == grid[i][j + 1].color)
                    if (grid[i][j].color == grid[i][j - 1].color)
                    {
                        for (int n = -1; n <= 1; n++) grid[i][j + n].isMatched++;
                        if (mouseclickcount)
                            swapcount = 1;
                    }
            }
        }

        for (int i = BOARD_SIZE_Y - 1; i >= 0; i--)
        {
            for (int j = 0; j < BOARD_SIZE_X; j++)
            {
                if (grid[i][j].isMatched)
                {
                    for (int n = i-1; n >= 0; n--)
                    {
                        if (!grid[n][j].isMatched)
                        {
                            swap(grid[n][j], grid[i][j]);
                            break;
                        }
                    }
                }
            }
        }
        if (2 == swapcount)swapcount = 0;
        else if (1 == swapcount)
        {
            swapcount = 5;
        }
        else if (5 == swapcount)
        {
            swap(grid[y0][x0], grid[y1][x1]);
            swapcount = 0;
        }
        else {
            ;
        }
        for (int j = 0; j < BOARD_SIZE_Y; j++)
        {
            for (int i = BOARD_SIZE_X - 1; i >= 0; i--)
            {
                if (grid[i][j].isMatched)
                {
                    if (CountScore)
                        Score++;
                    grid[i][j].color = 1 + rand() % 7;
                    grid[i][j].row = i;
                    grid[i][j].col = j;
                    grid[i][j].isMatched = 0;
                }
            }
        }
        scoretext.setString("Your score : " + to_string(Score));
        window.clear();
        for (int i = 0; i < BOARD_SIZE_Y; i++)
        {
            for (int j = 0; j < BOARD_SIZE_X; j++)
            {
                s.setTextureRect(IntRect(grid[i][j].color*18, 0, 18, 18));
                //s.scale(Vector2f(1.5f, 1.5f));
                s.setPosition(grid[i][j].col * TILE_SIZE , grid[i][j].row * TILE_SIZE );
               
                window.draw(s);
            }
        }
        window.draw(Match3);
        window.draw(scoretext);
        window.display();
    }
    cout << "Your score : "<< Score << endl;
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
