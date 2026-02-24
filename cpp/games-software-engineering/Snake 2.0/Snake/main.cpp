/*
    Snake - Start Project

    This project is set up to link to SFML (a library supporting 2D graphics, sound, user input and networing)
    There is a subdirectory in the project called SFML-3.0.0 containing SFML headers and libraries
    The project settings are configured to look for these files

    SFML reference: https://www.sfml-dev.org/documentation/3.0.0/
    SFML Tutorials: https://www.sfml-dev.org/tutorials/3.0/
*/

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const int CELL_SIZE = 32;           // Each grid square is 32x32 pixels
const int GRID_COLS = WINDOW_WIDTH / CELL_SIZE;   // 20 columns
const int GRID_ROWS = WINDOW_HEIGHT / CELL_SIZE;   // 20 rows
const float MOVE_INTERVAL = 0.15f;     // Seconds between each snake step


enum Direction { UP, DOWN, LEFT, RIGHT };

sf::Vector2f gridToPixel(int col, int row)
{
    return sf::Vector2f(
        static_cast<float>(col * CELL_SIZE),
        static_cast<float>(row * CELL_SIZE)
    );
}

// Place food at a random grid cell
sf::Vector2i randomFoodPos()
{
    return sf::Vector2i(std::rand() % GRID_COLS,
        std::rand() % GRID_ROWS);
}

void drawCell(sf::RenderWindow& window, int col, int row, sf::Color colour)
{
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    cell.setFillColor(colour);
    cell.setPosition(gridToPixel(col, row));
    window.draw(cell);
}


int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)),
        "Snake"
    );
    window.setFramerateLimit(60);

    // Head is always at index 0.
    // Snake has 3 parts to start
    std::vector<sf::Vector2i> snake;
    snake.push_back({ GRID_COLS / 2,     GRID_ROWS / 2 });
    snake.push_back({ GRID_COLS / 2 - 1, GRID_ROWS / 2 });
    snake.push_back({ GRID_COLS / 2 - 2, GRID_ROWS / 2 });

    Direction dir = RIGHT;
    Direction nextDir = RIGHT; // Buffer to allow one change per step

    sf::Vector2i food = randomFoodPos();

    float timeSinceLastMove = 0.f;
    bool gameOver = false;

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // Press R to restart if game over
                if (gameOver && keyPressed->code == sf::Keyboard::Key::R)
                {
                    snake.clear();
                    snake.push_back({ GRID_COLS / 2,     GRID_ROWS / 2 });
                    snake.push_back({ GRID_COLS / 2 - 1, GRID_ROWS / 2 });
                    snake.push_back({ GRID_COLS / 2 - 2, GRID_ROWS / 2 });
                    dir = nextDir = RIGHT;
                    food = randomFoodPos();
                    timeSinceLastMove = 0.f;
                    gameOver = false;
                }

                // Arrow keys / WASD, checks for moving backwards
                if (!gameOver)
                {
                    if ((keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) && dir != DOWN)
                        nextDir = UP;
                    if ((keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) && dir != UP)
                        nextDir = DOWN;
                    if ((keyPressed->code == sf::Keyboard::Key::Left || keyPressed->code == sf::Keyboard::Key::A) && dir != RIGHT)
                        nextDir = LEFT;
                    if ((keyPressed->code == sf::Keyboard::Key::Right || keyPressed->code == sf::Keyboard::Key::D) && dir != LEFT)
                        nextDir = RIGHT;
                }
            }
        }

        if (!gameOver)
        {
            timeSinceLastMove += dt;

            if (timeSinceLastMove >= MOVE_INTERVAL)
            {
                timeSinceLastMove = 0.f;
                dir = nextDir;

                // Calculate new head position
                sf::Vector2i newHead = snake[0];
                if (dir == UP)    newHead.y -= 1;
                if (dir == DOWN)  newHead.y += 1;
                if (dir == LEFT)  newHead.x -= 1;
                if (dir == RIGHT) newHead.x += 1;

                // Wall collision
                if (newHead.x < 0 || newHead.x >= GRID_COLS ||
                    newHead.y < 0 || newHead.y >= GRID_ROWS)
                {
                    gameOver = true;
                }

                // Kill if colliding with self
                for (std::size_t i = 0; i < snake.size(); ++i)
                {
                    if (snake[i] == newHead)
                    {
                        gameOver = true;
                        break;
                    }
                }

                if (!gameOver)
                {
                    // Move
                    snake.insert(snake.begin(), newHead);

                    // Check for food
                    if (newHead == food)
                    {
                        // Snake grows
                        food = randomFoodPos();

                        // Make sure food doesn't spawn on the snake
                        bool onSnake = true;
                        while (onSnake)
                        {
                            // Retry if the food spawns on the snake
                            onSnake = false;
                            for (std::size_t i = 0; i < snake.size(); ++i)
                            {
                                if (snake[i] == food)
                                {
                                    onSnake = true;
                                    food = randomFoodPos();
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        // Remove the tail so the snake stays the same length
                        snake.pop_back();
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        // Draw grid lines
        for (int col = 0; col < GRID_COLS; ++col)
        {
            for (int row = 0; row < GRID_ROWS; ++row)
            {
                sf::RectangleShape gridCell(sf::Vector2f(CELL_SIZE - 1.f, CELL_SIZE - 1.f));
                gridCell.setFillColor(sf::Color(40, 40, 40));
                gridCell.setPosition(gridToPixel(col, row));
                window.draw(gridCell);
            }
        }

        // Draw food
        drawCell(window, food.x, food.y, sf::Color(220, 50, 50));

        // Draw snake body
        for (std::size_t i = 0; i < snake.size(); ++i)
        {
            sf::Color colour = (i == 0)
                ? sf::Color(50, 220, 80)    // Head
                : sf::Color(30, 160, 60);   // Body
            drawCell(window, snake[i].x, snake[i].y, colour);
        }

        // Game over overlay
        if (gameOver)
        {
            // Draw a translucent rectangle over window as a death indicator
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 160));
            window.draw(overlay);

            // TODO - Add gameover message and restart instructions, needs to do with font loading, will add later
        }

        window.display();
    }

    return 0;
}