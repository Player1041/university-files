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
#include <iostream>
#include <fstream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const int CELL_SIZE = 32;           // Each grid square is 32x32 pixels
const int GRID_COLS = WINDOW_WIDTH / CELL_SIZE;   // 20 columns
const int GRID_ROWS = WINDOW_HEIGHT / CELL_SIZE;   // 20 rows
const int INITIAL_SNAKE_LENGTH = 12;
const float MOVE_INTERVAL = 0.15f;     // Seconds between each snake step+



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

using namespace std;
int main()
{
    int highScores[5] = { 0 };
	bool highScoresLoaded = false;
    while (!highScoresLoaded)
    {
        ifstream inFile;
        inFile.open("highscore.txt");

        if (inFile.is_open())
        {
			cout << "High scores loaded successfully!" << endl;
            for (int i = 0; i < 5; ++i)
            {
                inFile >> highScores[i];
            }
            inFile.close();
			highScoresLoaded = true;
        }
        else
        {
            cerr << "Unable to open highscore.txt, creating file" << endl;
            std::ofstream outfile("highscore.txt");

            outfile.close();
        }

    }
    cout << "High Scores:" << endl;
    for (int i = 0; i < 5; ++i)
    {
        cout << i + 1 << ". " << highScores[i] << endl;
    }
    sf::Font font;
    if (!font.openFromFile("font.ttf"))  // openFromFile() is new in 3.0
        return -1;

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)),
        "Snake"
    );
    window.setFramerateLimit(60);

    // Head is always at index 0
    // Snake has 3 parts to start
    std::vector<sf::Vector2i> snake;
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i) {
        snake.push_back({ GRID_COLS / 2 - i, GRID_ROWS / 2 });
    }

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
                    for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i) {
						snake.push_back({ GRID_COLS / 2 - i, GRID_ROWS / 2 });
                    }
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

        sf::Text scoreText(font);
        scoreText.setString("Score: " + std::to_string(snake.size() - INITIAL_SNAKE_LENGTH));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        
		sf::Text highScoreText(font);
        highScoreText.setString("High Scores:\n1. " + std::to_string(highScores[0]) +
            "\n2. " + std::to_string(highScores[1]) +
            "\n3. " + std::to_string(highScores[2]) +
            "\n4. " + std::to_string(highScores[3]) +
			"\n5. " + std::to_string(highScores[4]));
		highScoreText.setCharacterSize(24);
		highScoreText.setFillColor(sf::Color::White);

        if (!gameOver)
        {
            // Draw score
            scoreText.setPosition({ 10.f, 10.f });
            window.draw(scoreText);
		}

        // Game over overlay
        if (gameOver)
        {
            // Draw a translucent rectangle over window as a death indicator
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 160));
            window.draw(overlay);

			sf::Text gameOverText(font);
			gameOverText.setString("Game Over!\nPress R to Restart");
            gameOverText.setCharacterSize(48);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setPosition({ 100.f, 100.f });
			window.draw(gameOverText);

            scoreText.setPosition({ 100.f, 400.f });
            window.draw(scoreText);
            window.draw(highScoreText);

			// Check if score is a high score
			int score = snake.size() - INITIAL_SNAKE_LENGTH;

            for (int i = 0; i < 5; i++)
            {
                if (score > highScores[i])
                {
                    for (int j = 4; j > i; j--)
                    {
                        highScores[j] = highScores[j - 1];
                    }

                    highScores[i] = score;   // insert the new high score
                    
                }
                break;
                

            }

            ofstream outFile;
            outFile.open("highscore.txt");

            if (outFile.is_open())
            {
                for (int i = 0; i < 5; ++i)
                {
                    outFile << highScores[i] << endl;
                }
                outFile.close();
            }
        }

        window.display();
    }

    return 0;
}