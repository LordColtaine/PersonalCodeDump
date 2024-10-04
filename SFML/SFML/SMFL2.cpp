// SFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
//#include <SFML/Window.hpp>
//#include<SFML/Graphics.hpp>
//using namespace std;
//using namespace sf;
//
//void gameOver(RenderWindow& window)
//{
//    RenderWindow gameover(VideoMode(100, 100), "Game Over!");
//    Text t;
//    t.setString("Game Over!");
//    t.setCharacterSize(100);
//    //t.setFillColor(Color::White);
//    t.setPosition(gameover.getSize().x / 2, gameover.getSize().y / 2);
//    while (gameover.isOpen())
//    {
//        // check all the window's events that were triggered since the last iteration of the loop
//        sf::Event event;
//        while (gameover.pollEvent(event))
//        {
//            // "close requested" event: we close the window
//            if (event.type == sf::Event::Closed)
//                gameover.close();
//        }
//        gameover.clear(Color::White);
//        gameover.draw(t);
//        gameover.display();
//    }
//    window.close();
//}
//
//
//void moveSnake(RenderWindow& window, vector<CircleShape>& snake, int& keyCount)
//{
//    if (keyCount < 8)keyCount++;
//    if (keyCount >= 8)
//    {
//        if (Keyboard::isKeyPressed(Keyboard::A))
//        {
//            keyCount = 0;
//
//            snake[snake.size() - 1].move(snake[0].getPosition());
//            snake[0].move(-snake[0].getRadius()*2, 0);
//        }
//        if (Keyboard::isKeyPressed(Keyboard::D))
//        {
//            keyCount = 0;
//            snake[snake.size() - 1].move(snake[0].getPosition());
//            
//            snake[0].move(snake[0].getRadius() * 2, 0);
//        }
//        if (Keyboard::isKeyPressed(Keyboard::W))
//        {
//            keyCount = 0;
//            snake[snake.size() - 1].move(snake[0].getPosition());
//            snake[0].move(0, -snake[0].getRadius() * 2);
//        }
//        if (Keyboard::isKeyPressed(Keyboard::S))
//        {
//            keyCount = 0;
//            snake[0].move(0, snake[0].getRadius() * 2);
//        }
//        if (snake[0].getPosition().x <= 1 || snake[0].getPosition().x + snake[0].getRadius() * 2 >= window.getSize().x - 1 || snake[0].getPosition().y <= 1 || snake[0].getPosition().y + snake[0].getRadius() * 2 >= window.getSize().y - 1)
//            gameOver(window);
//    }
//}
//void drawScreen(RenderWindow& window, vector<CircleShape> snake)
//{
//    for (auto ele : snake)
//    {
//        window.draw(ele);
//    }
//}
//int main()
//{
//    int keyCount = 0;
//    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
//
//    window.setFramerateLimit(60);
//    vector<CircleShape> snake;
//    CircleShape Head(10);
//    Head.setPosition(window.getSize().x/2, window.getSize().y/2);
//    Head.setFillColor(Color::Red);
//
//    snake.push_back(Head);
//    for (int i = 0; i < 4; i++)
//    {
//        int x = Head.getPosition().x;
//        int y = Head.getPosition().y + Head.getRadius()*2;
//        Head.setPosition(x, y);
//        snake.push_back(Head);
//    }
//
//    while (window.isOpen())
//    {
//        // check all the window's events that were triggered since the last iteration of the loop
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // "close requested" event: we close the window
//            if (event.type == sf::Event::Closed)
//                window.close();
//            /*if (event.type == Event::KeyPressed)
//            {
//                if (event.key.code == Keyboard::Escape)
//                    window.close();
//            }*/
//            if (event.type == Event::KeyReleased)
//            {
//                ;
//            }
//        }
//        if (Keyboard::isKeyPressed(Keyboard::Escape))
//            window.close();
//
//
//        //Update
//        
//        moveSnake(window, snake, keyCount);
//
//        //Draw
//        window.clear(sf::Color::Black);
//
//        //window.draw(Head);
//        drawScreen(window, snake);
//        window.display();
//    }
//    //std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
