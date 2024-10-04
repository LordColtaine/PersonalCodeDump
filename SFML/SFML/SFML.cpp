// SFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
//#include <SFML/Window.hpp>
//#include<SFML/Graphics.hpp>
//using namespace std;
//using namespace sf;
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
//
//    window.setFramerateLimit(60);
//
//    sf::CircleShape circle(50.f);
//    sf::RectangleShape rect(sf::Vector2f(50.f, 100.f));// , sf::Vector2f(200.f, 100.f));
//    rect.setPosition(sf::Vector2f(200.f, 100.f));
//    rect.setFillColor(Color::Cyan);
//    CircleShape triangle;
//    triangle.setRadius(50.f);
//    triangle.setPointCount(3);
//    triangle.setFillColor(Color::Magenta);
//    Vertex Line[] = {Vertex(Vector2f(20.f, 20.f)), Vertex(Vector2f(400.f, 360.f))};
//    ConvexShape cs;
//    cs.setPointCount(5);
//    cs.setPosition(Vector2f(10.f, 500.f));
//
//    cs.setPoint(0, Vector2f(0, 0));
//    cs.setPoint(1, Vector2f(0, 30));
//    cs.setPoint(2, Vector2f(30, 20));
//    cs.setPoint(3, Vector2f(30, 5));
//    cs.setPoint(4, Vector2f(100, 69));
//
//    cs.setOutlineColor(Color::Black);
//    cs.setOutlineThickness(2);
//    sf::Texture texture;
//    if (!texture.loadFromFile("C:\\Users\\K Raviraju Prasanna\\source\\repos\\SFML\\Debug\\maxresdefault.jpg", sf::IntRect(300, 100, 600, 450)))
//    {
//        return 1;
//    }
//    texture.setRepeated( true);
//    
//    sf::Sprite sprite;
//    sprite.setTexture(texture);
//
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
//            
//        }
//
//
//        //Update
//        circle.move(1.f, 1.f);
//        rect.move(-0.1f, -0.1f);
//
//
//        //Draw
//        window.clear(sf::Color::Yellow);
//        window.draw(circle);
//        window.draw(rect);
//        window.draw(triangle);
//        window.draw(cs);
//        window.draw(Line, 2, sf::Lines);
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
