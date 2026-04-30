#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <string>
#include <vector>
#include "Snake.h"
#include "QuestionManager.h"
#include "EnglishManager.h"
#include "RandomManager.h"
#include "HighScores.h"

class Game
{
public:
    enum GAMEMODE { MATHS, ENGLISH, PUZZLE, RANDOM, NONE };

    enum MATHS_GAME_TYPES { TIMES_TABLE, ARITHMETIC, DIVISION, NO_MATHS_TYPE };
    enum ENGLISH_GAME_TYPES { ALPHABET, SPELLING, NO_ENGLISH_TYPE };
    enum PUZZLE_GAME_TYPES { MIRRORS, CLASSIC, BOXED, MAZE, NO_PUZZLE_TYPE };

    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::Font font;

    // Sound buffers and sounds.
    sf::SoundBuffer sbChangeDir;   // changedir.wav  - direction key pressed
    sf::SoundBuffer sbClassicEat;  // classiceat.wav - food eaten in classic mode -- changed to correct.wav since i couldnt make a good sound in rfxgen lol
    sf::SoundBuffer sbCorrect;     // correct.wav    - correct answer collected
    sf::SoundBuffer sbWrong;       // wrong.wav      - wrong answer collected
    sf::SoundBuffer sbDeath;       // death.wav      - snake dies

    std::optional<sf::Sound> sndChangeDir;
    std::optional<sf::Sound> sndClassicEat;
    std::optional<sf::Sound> sndCorrect;
    std::optional<sf::Sound> sndWrong;
    std::optional<sf::Sound> sndDeath;

    // Plays a sound if it loaded successfully (optional has a value)
    void playSound(std::optional<sf::Sound>& snd);

    // Menu state machine
    // 0 = mode select
    // 1 = subtype select
    // 2 = length select 
    // 3 = difficulty select
    // 4 = ready screen
    // 5 = high scores
    // 6 = name entry
    // -1 = post-game-over (name entered, waiting for R or ESC)
    bool inGame;
    GAMEMODE selectedMode;
    MATHS_GAME_TYPES  selectedMathsType;
    ENGLISH_GAME_TYPES selectedEnglishType;
    PUZZLE_GAME_TYPES selectedPuzzleType;
    SessionLength selectedLength;
    BlankDifficulty selectedDifficulty;
    int menuState;
    int cursorIndex;

    // Shared gameplay
    Snake snake;
    sf::Clock moveClock;
    bool gameOver;
    bool sessionComplete;
    bool scoreSaved;

    // Classic snake
    sf::Vector2i foodPos;
    int score;
    void spawnFood();

    // Maths snake
    struct GridItem {
        sf::Vector2i cell;
        int value;
        bool collected;
    };

    QuestionManager* qm;
    std::vector<GridItem> gridItems;
    std::vector<int>  collectedAnswers;
    bool wrongAnswer;
    sf::Clock wrongAnswerClock;
    int questionsScore;
    sf::Clock sessionClock;

    void spawnGridItems();
    void checkGridCollection();

    // English snake
    struct LetterItem {
        sf::Vector2i cell;
        char letter;
        bool collected;
    };

    EnglishManager* em;
    std::vector<LetterItem> letterItems;
    bool  wrongLetter;
    sf::Clock wrongLetterClock;

    void spawnLetterItems();
    void checkLetterCollection();

    // Random mode
    RandomManager* rm;

    void updateRandomSnake();
    void renderRandomSnake();
    void checkRandomCollection();

    // Name entry
    std::string pendingName;
    std::string pendingScoreMode;
    int pendingScore;
    float pendingTime;

    void triggerNameEntry(const std::string& modeKey, int finalScore, float finalTime);
    void commitScore();

    // High scores screen
    HighScores highScores;
    int hsModeIndex;
    int hsLengthIndex;

    void renderHighScores();

    // Core loop
    void handleEvents();
    void update();
    void render();

    // Menu rendering
    void renderModeMenu();
    void renderSubMenu();
    void renderLengthMenu();
    void renderDifficultyMenu();
    void renderReadyScreen();
    void renderNameEntry();

    // Gameplay
    void handleGameEvents(sf::Keyboard::Key code);

    void updateClassic();
    void renderClassicSnake();

    void updateMathsSnake();
    void renderMathsSnake();

    void updateEnglishSnake();
    void renderEnglishSnake();

    void renderGameOver();
    void renderSessionComplete();

    // Game control
    void startGame();
    void stopGame();
    void loadLevel();
    void unloadLevel();
    void pauseGame();

    // Helpers
    std::string currentScoreModeKey() const;
    bool modeHasLengthMenu() const;
    bool modeHasDifficultyMenu() const;
    bool isImplemented() const;
    bool isSubtypeImplemented(int index) const;

    std::string getModeName(GAMEMODE mode) const;
    std::string getMathsTypeName(MATHS_GAME_TYPES t) const;
    std::string getEnglishTypeName(ENGLISH_GAME_TYPES t) const;
    std::string getPuzzleTypeName(PUZZLE_GAME_TYPES t) const;
    std::string getModeDescription(GAMEMODE mode) const;
    std::string getLengthLabel(SessionLength l) const;
    std::string getDifficultyLabel(BlankDifficulty d) const;
    std::string getDifficultyDescription(BlankDifficulty d) const;
    int getSubMenuCount() const;
    std::string getSubMenuOption(int index) const;
    std::string formatTime(float seconds) const;

    void drawCentredText(const std::string& str, unsigned int size, float y, sf::Color colour);
};