#include "Snake.h"
// Constructor
Snake::Snake() : topBoundary(0)
{
    reset();
}

// Reset - standard (no HUD wall)
void Snake::reset()
{
    reset(0);
}

// Reset with a top boundary row.
// topBoundaryRow is the first row the snake is allowed to enter.
// e.g. passing 2 means rows 0 and 1 are walls (used by HUD modes).
void Snake::reset(int topBoundaryRow)
{
    body.clear();
    growing = false;
    topBoundary = topBoundaryRow;

    // Start in the middle of the playable area
    int startX = GRID_COLS / 2;
    int startY = topBoundary + (GRID_ROWS - topBoundary) / 2;

    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
        body.push_back({ startX - i, startY });

    currentDir = RIGHT;
    pendingDir = RIGHT;
}

// Buffer a direction change (ignores reversal)
void Snake::setDirection(Direction dir)
{
    if (!isOpposite(dir, currentDir))
        pendingDir = dir;
}

// Advance one grid step.
// Returns false if the move causes a collision.
bool Snake::move()
{
    currentDir = pendingDir;

    sf::Vector2i head = body.front();

    switch (currentDir)
    {
    case UP: head.y -= 1; break;
    case DOWN: head.y += 1; break;
    case LEFT: head.x -= 1; break;
    case RIGHT: head.x += 1; break;
    }

    body.push_front(head);

    if (!growing)
        body.pop_back();
    else
        growing = false;

    if (hitsWall() || hitsSelf())
        return false;

    return true;
}

// Flag the snake to grow on the next move
void Snake::grow()
{
    growing = true;
}

// Check if a grid cell is occupied by the snake
bool Snake::occupies(sf::Vector2i cell) const
{
    for (const auto& seg : body)
        if (seg == cell)
            return true;
    return false;
}

sf::Vector2i Snake::getHead() const
{
    return body.front();
}

// Draw the snake
void Snake::draw(sf::RenderWindow& window) const
{
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));

    for (int i = 0; i < (int)body.size(); i++)
    {
        if (i == 0)
            cell.setFillColor(sf::Color(80, 220, 120));
        else
            cell.setFillColor(sf::Color(40, 150, 70));

        cell.setPosition({
            body[i].x * (float)CELL_SIZE + 1.f,
            body[i].y * (float)CELL_SIZE + 1.f
            });
        window.draw(cell);
    }
}

// Private helpers
bool Snake::isOpposite(Direction a, Direction b) const
{
    return (a == UP && b == DOWN) ||
        (a == DOWN && b == UP) ||
        (a == LEFT && b == RIGHT) ||
        (a == RIGHT && b == LEFT);
}

bool Snake::hitsWall() const
{
    sf::Vector2i head = body.front();
    return head.x < 0 || head.x >= GRID_COLS ||
        head.y < topBoundary || // HUD wall at top
        head.y >= GRID_ROWS;
}

bool Snake::hitsSelf() const
{
    sf::Vector2i head = body.front();
    for (int i = 1; i < (int)body.size(); i++)
        if (body[i] == head)
            return true;
    return false;
}