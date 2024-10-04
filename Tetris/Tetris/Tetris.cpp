// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/main.hpp>
using namespace std;
using namespace sf;

#define BLOCKSIZE 18
#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600

#define BOARD_SIZE_X 20
#define BOARD_SIZE_Y 30

int BLOCKCOORDS_X[7][4]
{
    {BOARD_SIZE_X/2, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2+1, BOARD_SIZE_X / 2+1},
    {BOARD_SIZE_X / 2, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2+1, BOARD_SIZE_X / 2+1},
    {BOARD_SIZE_X / 2+1, BOARD_SIZE_X / 2+1, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2},
    {BOARD_SIZE_X / 2, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2+1, BOARD_SIZE_X / 2+2},
    {BOARD_SIZE_X / 2, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2-1, BOARD_SIZE_X / 2-2},
    {BOARD_SIZE_X / 2-1, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2+1},
    {BOARD_SIZE_X / 2, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2, BOARD_SIZE_X / 2}
};

int BLOCKCOORDS_Y[7][4]
{
    {0, 1, 1, 0},
    {0, 1, 1, 2},
    {0, 1, 1, 2},
    {0, 1, 1, 1},
    {0, 1, 1, 1},
    {0, 0, 1, 0},
    {0, 1, 2, 3}
};
vector<vector<int>> board;
vector<int> filler;
int MAX_TIME = 10;
struct Block {
    vector<pair<int, int>> coords;

    Block()
    {
        int i = rand() % 7;
        coords.push_back(pair<int, int>(BLOCKCOORDS_X[i][0], BLOCKCOORDS_Y[i][0]));
        coords.push_back(pair<int, int>(BLOCKCOORDS_X[i][1], BLOCKCOORDS_Y[i][1]));
        coords.push_back(pair<int, int>(BLOCKCOORDS_X[i][2], BLOCKCOORDS_Y[i][2]));
        coords.push_back(pair<int, int>(BLOCKCOORDS_X[i][3], BLOCKCOORDS_Y[i][3]));
    }
    Block(Block& b)
    {
        coords.push_back(pair<int, int>());
        coords.push_back(pair<int, int>());
        coords.push_back(pair<int, int>());
        coords.push_back(pair<int, int>());
        for (int i = 0; i < 4; i++)
        {
            coords[i].first = b.coords[i].first;
            coords[i].second = b.coords[i].second;
        }
    }
    Block& operator=(Block& b)
    {
        for (int i = 0; i < 4; i++)
        {
            coords[i].first = b.coords[i].first;
            coords[i].second = b.coords[i].second;
        }
        return *this;
    }
    Block& operator=(Block&& b)
    {
        cout << "Move called\n";
        for (int i = 0; i < 4; i++)
        {
            coords[i].first = b.coords[i].first;
            coords[i].second = b.coords[i].second;
        }
        b.coords.clear();
        return *this;
    }
};
Block block;

bool possibleToMove(Block temp)
{
    //for (int i = 0; i < 4; i++)cout << temp.coords[i].first << temp.coords[i].second<<endl;
    for (int i = 0; i < 4; i++)
    {
        if (temp.coords[i].first < 0 || temp.coords[i].first >= BOARD_SIZE_X || temp.coords[i].second < 0 || temp.coords[i].second >= BOARD_SIZE_Y)
            return false;
        if (board[temp.coords[i].second][temp.coords[i].first])
            return false;
    }
    return true;
}
int main()
{
    //Create the board
    for (int i = 0; i < BOARD_SIZE_Y; i++)
    {
        vector<int> temp;
        for (int j = 0; j < BOARD_SIZE_X; j++)
            temp.push_back(0);
        board.push_back(temp);
    }
    for (int i = 0; i < BOARD_SIZE_X; i++)filler.push_back(0);
    /*for (int i = 0; i < BOARD_SIZE_Y; i++)
    {
        vector<int> temp;
        for (int j = 0; j < BOARD_SIZE_X; j++)
            cout << board[i][j];
        cout << endl;
    }*/
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Tetris");
    window.setFramerateLimit(30);
    Texture t, framet;
    if(!t.loadFromFile("images\\tiles.png"))return 1;
    //if (!framet.loadFromFile("images\\frame.png"))return 1;
    Sprite s(t);// , frame(framet);
    s.setTextureRect(IntRect(36, 0, 18, 18));
    //frame.setScale(0.4, 0.4);
    
    Font font;
    if(!font.loadFromFile("images\\arialbd.ttf"))return 1;

    Text tetris, scoretext;
    tetris.setFont(font);
    tetris.setCharacterSize(40);
    tetris.setOutlineColor(Color::White);
    tetris.setFillColor(Color::Red);
    tetris.setPosition(Vector2f(3 * WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 4));
    tetris.setString("TETRIS");

    scoretext.setFont(font);
    scoretext.setCharacterSize(35);
    scoretext.setOutlineColor(Color::White);
    scoretext.setFillColor(Color::White);
    scoretext.setPosition(Vector2f( WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2 ));
    
    RectangleShape border;
    border.setOutlineThickness(3);
    border.setOutlineColor(Color::Blue);
    border.setSize(Vector2f(BOARD_SIZE_X * BLOCKSIZE, BOARD_SIZE_Y * BLOCKSIZE));
    border.setFillColor(Color::Black);
    //define required variables
    int timer = 0;
    int score = 0;
    int ColorCode=2, ColorNext=3;
    Block next;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        int x_disp = 0;
        int y_disp = 0;
        bool rotate = false;
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::A)x_disp--;
                if (event.key.code == Keyboard::D)x_disp++;
                if (event.key.code == Keyboard::S)timer=9;
                if (event.key.code == Keyboard::Space)rotate = true;
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();
        //if (Keyboard::isKeyPressed(Keyboard::A))x_disp--;
        //if (Keyboard::isKeyPressed(Keyboard::D))x_disp++;
        ////if (Keyboard::isKeyPressed(Keyboard::W))y_disp--;
        ////if (Keyboard::isKeyPressed(Keyboard::S))y_disp++;
        //if (Keyboard::isKeyPressed(Keyboard::S))timer*=2;
        //if(Keyboard::isKeyPressed(Keyboard::Space))rotate = true;
        window.clear();
        
        //Movement
        Block temp = block;
        for (int i = 0; i < 4; i++)
        {
            temp.coords[i].first += x_disp;
            //temp.coords[i].second += y_disp;
        }
        if (possibleToMove(temp))
            block = temp;

        //Rotate
        if (rotate)
        {
            pair<int, int> p = block.coords[1];
            for (int i = 0; i < 4; i++)
            {
                int y = block.coords[i].first - p.first;
                int x = block.coords[i].second - p.second;
                
                temp.coords[i].first = p.first - x;
                temp.coords[i].second = p.second + y;
                
            }
            if (possibleToMove(temp))block = temp;
        }
        //Auto downward movement
        timer++;
        if (timer>=MAX_TIME)
        {
            temp = block;
            for(int i=0;i<4;i++)
                temp.coords[i].second++;
            if (possibleToMove(temp))block = temp;
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    board[block.coords[i].second][block.coords[i].first] = ColorCode;
                }
                Block New;
                block = next;
                next = New;
                ColorCode = ColorNext;
                ColorNext = rand() % 7 + 1;
            }
            timer = 0;
        }

        //Checking if there's a line
        for (int i = BOARD_SIZE_Y - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < BOARD_SIZE_X; j++)
            {
                if (board[i][j])count++;
            }
            if (BOARD_SIZE_X == count)
            {
                board.erase(board.begin() + i);
                board.insert(board.begin(), filler);
                score += BOARD_SIZE_X;
            }
        }

        scoretext.setString("Your score : "+to_string(score));
        //Draw
        window.draw(border);
        for (int i = 0; i < BOARD_SIZE_Y; i++)
        {
            for (int j = 0; j < BOARD_SIZE_X; j++)
            {
                if (board[i][j])
                {
                    s.setPosition(j*BLOCKSIZE, i*BLOCKSIZE);
                    s.setTextureRect(IntRect((board[i][j]) * 18, 0, 18, 18));
                    window.draw(s);
                }
            }
        }
        for (int i = 0; i < 4; i++)
        {
            s.setPosition(block.coords[i].first*BLOCKSIZE, block.coords[i].second*BLOCKSIZE);
            s.setTextureRect(IntRect(ColorCode * 18, 0, 18, 18));
            window.draw(s);
        }

        for (int i = 0; i < 4; i++)
        {
            s.setPosition(3 * WINDOW_SIZE_X / 4 - 50 + next.coords[i].first * BLOCKSIZE, 3 * WINDOW_SIZE_Y / 4 + next.coords[i].second * BLOCKSIZE);
            s.setTextureRect(IntRect(ColorNext*18, 0, 18, 18));
            window.draw(s);
        }
        window.draw(tetris);
        window.draw(scoretext);
        window.display();
    }
    cout << score << endl;
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
