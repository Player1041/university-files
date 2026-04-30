#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Constants.h"

class Snake
{
public:
    Snake();

    void reset();
    void reset(int topBoundaryRow); // reset with a custom top wall row

    void setDirection(Direction dir);
    bool move(); // moves one step, returns false if collision
    void grow();

    bool occupies(sf::Vector2i cell) const;
    sf::Vector2i getHead() const;

    void draw(sf::RenderWindow& window) const;

private:
    std::deque<sf::Vector2i> body;
    Direction currentDir;
    Direction pendingDir;
    bool growing;
    int topBoundary; // first valid row (0 for classic, >0 when HUD takes rows)

    bool isOpposite(Direction a, Direction b) const;
    bool hitsWall() const;
    bool hitsSelf() const;
};