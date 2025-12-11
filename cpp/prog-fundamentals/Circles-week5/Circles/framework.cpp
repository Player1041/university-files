
// SFML header file for graphics, there are also ones for Audio, Window, System and Network
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include "Framework.h"

extern int gScreenWidth;
extern int gScreenHeight;

sf::RenderWindow gWindow(sf::VideoMode(gScreenWidth, gScreenHeight), "GPF Circles");
sf::CircleShape gCircleShape(20, 64);
sf::RectangleShape gRectangleShape;

extern int x;
extern int y;

extern int gTimeDelayMS;

using namespace std::this_thread;
using namespace std::chrono;

bool UpdateFramework()
{
    static bool init{ true };

    if (!gWindow.isOpen())
        return false;

    if (init)
    {
        gCircleShape.setFillColor(sf::Color::Red);
        init = false;
    }

    // Handle any pending SFML events
    // These cover keyboard, mouse,joystick etc.
    sf::Event event;
    while (gWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            gWindow.close();
            break;
        default:
            break;
        }
    }

    // Get the window to display its contents
    gWindow.display();

    sleep_for(milliseconds(gTimeDelayMS));

    // We must clear the winint timeDelayMS;dow each time round the loop
    gWindow.clear();

    // draw our circle shape to the window
   // gWindow.draw(gRedCircleShape);

    return true;
}

void ChangeColour(unsigned char red, unsigned char green,
    unsigned char blue, unsigned char alpha)
{
    gCircleShape.setFillColor({ red,green,blue,alpha });
    gRectangleShape.setFillColor({ red,green,blue,alpha });
}

void DrawCircle(int x, int y, int radius)
{

    gCircleShape.setPosition((float)x, (float)y);
    gCircleShape.setRadius((float)radius);
    // draw our circle shape to the window
    gWindow.draw(gCircleShape);
}

void DrawRectangle(int x, int y, int width, int height)
{
    gRectangleShape.setPosition((float)x, (float)y);
    gRectangleShape.setSize({ (float)width, (float)height });
    // draw our circle shape to the window
    gWindow.draw(gRectangleShape);
}


void GetMousePosition(int& x, int& y)
{
    sf::Vector2i position = sf::Mouse::getPosition(gWindow);

    x = position.x;
    y = position.y;
}

bool IsButtonPressed(EButton whichButton)
{
    switch (whichButton)
    {
    case EButton::eLeft:
        return sf::Mouse::isButtonPressed(sf::Mouse::Left);
    case EButton::eRight:
        return sf::Mouse::isButtonPressed(sf::Mouse::Right);
    case EButton::eMiddle:
        return sf::Mouse::isButtonPressed(sf::Mouse::Middle);
    default:
        std::cerr << "unknown button" << std::endl;
        break;
    }

    return false;
}