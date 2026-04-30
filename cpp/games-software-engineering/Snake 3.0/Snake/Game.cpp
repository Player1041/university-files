#include "Game.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <sstream>

// HUD strip at the top of maths/english/random modes
static const int HUD_HEIGHT = 64;
static const int HUD_ROWS = HUD_HEIGHT / CELL_SIZE;  // = 2
static const float WRONG_FLASH_DURATION = 0.8f;
static const int GRID_ITEM_COUNT = 6;
static const int MODE_MENU_COUNT = 5; // Maths, English, Puzzle, Random, High Scores
static const int MAX_NAME_LENGTH = 8;

// Constructor
Game::Game()
    : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Edu Snake"),
    inGame(false),
    selectedMode(NONE),
    selectedMathsType(NO_MATHS_TYPE),
    selectedEnglishType(NO_ENGLISH_TYPE),
    selectedPuzzleType(NO_PUZZLE_TYPE),
    selectedLength(SessionLength::ROUNDS_10),
    selectedDifficulty(BlankDifficulty::EASY),
    menuState(0),
    cursorIndex(0),
    gameOver(false),
    sessionComplete(false),
    scoreSaved(false),
    foodPos(5, 5),
    score(0),
    qm(nullptr),
    wrongAnswer(false),
    questionsScore(0),
    em(nullptr),
    wrongLetter(false),
    rm(nullptr),
    pendingScore(0),
    pendingTime(0.f),
    hsModeIndex(0),
    hsLengthIndex(0)
{
    window.setFramerateLimit(60);
    if (!font.openFromFile("font.otf")) {}
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    highScores.load("highscores.txt");

    auto loadSound = [](sf::SoundBuffer& buf, std::optional<sf::Sound>& snd, const std::string& file)
        {
            if (buf.loadFromFile(file))
                snd.emplace(buf);
        };

    loadSound(sbChangeDir, sndChangeDir, "changedir.wav");
    loadSound(sbClassicEat, sndClassicEat, "correct.wav");
    loadSound(sbCorrect, sndCorrect, "correct.wav");
    loadSound(sbWrong, sndWrong, "wrong.wav");
    loadSound(sbDeath, sndDeath, "death.wav");
}

// Play a sound if it loaded successfully (optional has a value)
void Game::playSound(std::optional<sf::Sound>& snd)
{
    if (snd.has_value())
        snd->setVolume(2.0f); // Set volume since these sounds become loud
        snd->play();
}

// Main loop
void Game::run()
{
    while (window.isOpen())
    {
        handleEvents();
        update();
        render();
    }
}

// Event handling
void Game::handleEvents()
{
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (menuState == 6)
        {
            if (const auto* te = event->getIf<sf::Event::TextEntered>())
            {
                uint32_t ch = te->unicode;
                if (ch == 8 && !pendingName.empty())
                    pendingName.pop_back();   // backspace
                else if (ch >= 32 && ch < 127 && (int)pendingName.size() < MAX_NAME_LENGTH)
                    pendingName += static_cast<char>(std::toupper(ch));
            }
            if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Enter && !pendingName.empty())
                {
                    commitScore();
                    // Return to the game over / session complete overlay.
                    // inGame stays false; gameOver/sessionComplete flags stay set.
                    // The player can now press R to restart or ESC for the menu.
                    menuState = -1;
                }
            }
            continue;
        }

        if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
        {
            sf::Keyboard::Key code = kp->code;

            if (inGame || menuState == -1) { handleGameEvents(code); return; }

            // High scores screen (state 5)
            if (menuState == 5)
            {
                int modeCount = (int)ScoreMode::TYPES.size();
                int lengthCount = (int)ScoreMode::LENGTHS.size();

                // Left/right changes game type
                if (code == sf::Keyboard::Key::Left)
                    hsModeIndex = (hsModeIndex - 1 + modeCount) % modeCount;
                if (code == sf::Keyboard::Key::Right)
                    hsModeIndex = (hsModeIndex + 1) % modeCount;

                // Up/down changes length tier.
                // Classic only has one valid length so skip navigation for it.
                if (ScoreMode::TYPES[hsModeIndex] != "Classic")
                {
                    if (code == sf::Keyboard::Key::Up)
                        hsLengthIndex = (hsLengthIndex - 1 + lengthCount) % lengthCount;
                    if (code == sf::Keyboard::Key::Down)
                        hsLengthIndex = (hsLengthIndex + 1) % lengthCount;
                }
                else
                {
                    hsLengthIndex = 0;   // pin to first slot for Classic
                }

                if (code == sf::Keyboard::Key::Escape)
                {
                    menuState = 0; cursorIndex = 0;
                }
                return;
            }

            // Mode select (state 0)
            if (menuState == 0)
            {
                if (code == sf::Keyboard::Key::Up)
                    cursorIndex = (cursorIndex - 1 + MODE_MENU_COUNT) % MODE_MENU_COUNT;
                if (code == sf::Keyboard::Key::Down)
                    cursorIndex = (cursorIndex + 1) % MODE_MENU_COUNT;
                if (code == sf::Keyboard::Key::Enter)
                {
                    if (cursorIndex == 4) { menuState = 5; return; }   // High Scores
                    selectedMode = static_cast<GAMEMODE>(cursorIndex);
                    // Random has no subtypes - go straight to length selection
                    if (selectedMode == RANDOM)
                    {
                        menuState = 2; cursorIndex = 0;
                    }
                    else
                    {
                        menuState = 1; cursorIndex = 0;
                    }
                }
            }

            // Subtype select (state 1)
            else if (menuState == 1)
            {
                int sub = getSubMenuCount();
                if (code == sf::Keyboard::Key::Up)
                    cursorIndex = (cursorIndex - 1 + sub) % sub;
                if (code == sf::Keyboard::Key::Down)
                    cursorIndex = (cursorIndex + 1) % sub;
                if (code == sf::Keyboard::Key::Enter)
                {
                    // Silently ignore if this subtype has no gameplay yet
                    if (!isSubtypeImplemented(cursorIndex)) return;

                    if (selectedMode == MATHS)
                        selectedMathsType = static_cast<MATHS_GAME_TYPES>(cursorIndex);
                    else if (selectedMode == ENGLISH)
                        selectedEnglishType = static_cast<ENGLISH_GAME_TYPES>(cursorIndex);
                    else if (selectedMode == PUZZLE)
                        selectedPuzzleType = static_cast<PUZZLE_GAME_TYPES>(cursorIndex);

                    // Classic puzzle skips length/difficulty screens
                    if (!modeHasLengthMenu())
                        menuState = 4;
                    else
                    {
                        menuState = 2;
                        cursorIndex = 0;
                    }
                }
                if (code == sf::Keyboard::Key::Escape)
                {
                    menuState = 0; cursorIndex = 0; selectedMode = NONE;
                }
            }

            // Length select (state 2)
            else if (menuState == 2)
            {
                static const int LENGTH_COUNT = 5;   // Endless, 10, 15, 20, 25
                if (code == sf::Keyboard::Key::Up)
                    cursorIndex = (cursorIndex - 1 + LENGTH_COUNT) % LENGTH_COUNT;
                if (code == sf::Keyboard::Key::Down)
                    cursorIndex = (cursorIndex + 1) % LENGTH_COUNT;
                if (code == sf::Keyboard::Key::Enter)
                {
                    // Map cursor to SessionLength
                    static const SessionLength lengths[] = {
                        SessionLength::ENDLESS,
                        SessionLength::ROUNDS_10,
                        SessionLength::ROUNDS_15,
                        SessionLength::ROUNDS_20,
                        SessionLength::ROUNDS_25
                    };
                    selectedLength = lengths[cursorIndex];
                    menuState = 3;
                    cursorIndex = 0;
                }
                if (code == sf::Keyboard::Key::Escape)
                {
                    // Random skips the subtype screen so ESC goes back to mode select
                    if (selectedMode == RANDOM)
                    {
                        menuState = 0; cursorIndex = 0; selectedMode = NONE;
                    }
                    else
                    {
                        menuState = 1; cursorIndex = 0;
                    }
                }
            }

            // Difficulty select (state 3)
            else if (menuState == 3)
            {
                static const int DIFF_COUNT = 3;
                if (code == sf::Keyboard::Key::Up)
                    cursorIndex = (cursorIndex - 1 + DIFF_COUNT) % DIFF_COUNT;
                if (code == sf::Keyboard::Key::Down)
                    cursorIndex = (cursorIndex + 1) % DIFF_COUNT;
                if (code == sf::Keyboard::Key::Enter)
                {
                    static const BlankDifficulty diffs[] = {
                        BlankDifficulty::EASY,
                        BlankDifficulty::MEDIUM,
                        BlankDifficulty::HARD
                    };
                    selectedDifficulty = diffs[cursorIndex];
                    menuState = 4;
                    cursorIndex = 0;
                }
                if (code == sf::Keyboard::Key::Escape)
                {
                    menuState = 2; cursorIndex = 0;
                }
            }

            // Ready screen (state 4)
            else if (menuState == 4)
            {
                if (code == sf::Keyboard::Key::Enter && isImplemented()) startGame();
                if (code == sf::Keyboard::Key::Escape)
                {
                    if (modeHasDifficultyMenu())
                        menuState = 3;
                    else
                        menuState = 1;
                    cursorIndex = 0;
                }
            }
        }
    }
}

// In-game input
void Game::handleGameEvents(sf::Keyboard::Key code)
{
    if (gameOver || sessionComplete)
    {
        if (code == sf::Keyboard::Key::R)      startGame();
        if (code == sf::Keyboard::Key::Escape) stopGame();
        return;
    }

    switch (code)
    {
    case sf::Keyboard::Key::Up:
        snake.setDirection(UP);
        playSound(sndChangeDir);
        break;
    case sf::Keyboard::Key::Down:
        snake.setDirection(DOWN);
        playSound(sndChangeDir);
        break;
    case sf::Keyboard::Key::Left:
        snake.setDirection(LEFT);
        playSound(sndChangeDir);
        break;
    case sf::Keyboard::Key::Right:
        snake.setDirection(RIGHT);
        playSound(sndChangeDir);
        break;
    case sf::Keyboard::Key::Escape: stopGame(); break;
    default: break;
    }
}

// Update
void Game::update()
{
    if (!inGame || gameOver || sessionComplete) return;

    if (moveClock.getElapsedTime().asSeconds() >= MOVE_INTERVAL)
    {
        moveClock.restart();

        if (selectedMode == PUZZLE && selectedPuzzleType == CLASSIC)
            updateClassic();
        else if (selectedMode == MATHS)
            updateMathsSnake();
        else if (selectedMode == ENGLISH)
            updateEnglishSnake();
        else if (selectedMode == RANDOM)
            updateRandomSnake();
    }
}

// Classic snake step
void Game::updateClassic()
{
    if (!snake.move())
    {
        gameOver = true;
        playSound(sndDeath);
        if (!scoreSaved)
        {
            triggerNameEntry(currentScoreModeKey(), score, 0.f);
            scoreSaved = true;
        }
        return;
    }

    if (snake.getHead() == foodPos)
    {
        snake.grow();
        score++;
        spawnFood();
        playSound(sndClassicEat);
    }
}

// Maths snake step
void Game::updateMathsSnake()
{
    if (!snake.move())
    {
        gameOver = true;
        playSound(sndDeath);
        if (!scoreSaved)
        {
            float elapsed = sessionClock.getElapsedTime().asSeconds();
            triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
            scoreSaved = true;
        }
        return;
    }
    checkGridCollection();
}

// English snake step
void Game::updateEnglishSnake()
{
    if (!snake.move())
    {
        gameOver = true;
        playSound(sndDeath);
        if (!scoreSaved)
        {
            float elapsed = sessionClock.getElapsedTime().asSeconds();
            triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
            scoreSaved = true;
        }
        return;
    }
    checkLetterCollection();
}

// Check if the snake head is on a maths grid item.
// Collects it, validates, and either advances or respawns.
void Game::checkGridCollection()
{
    if (qm->isComplete()) return;

    sf::Vector2i head = snake.getHead();

    for (auto& item : gridItems)
    {
        if (item.collected || item.cell != head) continue;

        item.collected = true;
        collectedAnswers.push_back(item.value);
        snake.grow();

        int blanksNeeded = qm->current().blankCount();

        if ((int)collectedAnswers.size() >= blanksNeeded)
        {
            if (qm->submit(collectedAnswers))
            {
                questionsScore++;
                wrongAnswer = false;
                playSound(sndCorrect);

                if (qm->isComplete())
                {
                    float elapsed = sessionClock.getElapsedTime().asSeconds();
                    triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
                    scoreSaved = true;
                    sessionComplete = true;
                }
                else
                {
                    collectedAnswers.clear();
                    spawnGridItems();
                }
            }
            else
            {
                // Wrong answer - flash the question red and respawn the grid
                wrongAnswer = true;
                wrongAnswerClock.restart();
                collectedAnswers.clear();
                spawnGridItems();
                playSound(sndWrong);
            }
        }

        break;   // only one item per cell
    }

    // Clear wrong-answer flash after the display duration
    if (wrongAnswer && wrongAnswerClock.getElapsedTime().asSeconds() > WRONG_FLASH_DURATION)
        wrongAnswer = false;
}

// Check if the snake head is on a letter tile.
// Uses LetterResult to cleanly distinguish wrong / partial / complete.
void Game::checkLetterCollection()
{
    if (em->isComplete()) return;

    sf::Vector2i head = snake.getHead();

    for (auto& item : letterItems)
    {
        if (item.collected || item.cell != head) continue;

        item.collected = true;
        LetterResult result = em->submitLetter(std::tolower(item.letter));

        if (result == LetterResult::CORRECT_MORE)
        {
            // Right letter but more still needed - grow and respawn with next letter
            snake.grow();
            wrongLetter = false;
            playSound(sndCorrect);
            spawnLetterItems();
        }
        else if (result == LetterResult::CORRECT_DONE)
        {
            // Question fully answered
            snake.grow();
            wrongLetter = false;
            questionsScore++;
            playSound(sndCorrect);

            if (em->isComplete())
            {
                float elapsed = sessionClock.getElapsedTime().asSeconds();
                triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
                scoreSaved = true;
                sessionComplete = true;
            }
            else
            {
                spawnLetterItems();
            }
        }
        else   // LetterResult::WRONG
        {
            // Wrong letter - flash the question red and respawn
            wrongLetter = true;
            wrongLetterClock.restart();
            playSound(sndWrong);
            spawnLetterItems();
        }

        break;   // only one item per cell
    }

    // Clear wrong-letter flash after the display duration
    if (wrongLetter && wrongLetterClock.getElapsedTime().asSeconds() > WRONG_FLASH_DURATION)
        wrongLetter = false;
}

// Random snake step - delegates to whichever manager is active
void Game::updateRandomSnake()
{
    if (!snake.move())
    {
        gameOver = true;
        playSound(sndDeath);
        if (!scoreSaved)
        {
            float elapsed = sessionClock.getElapsedTime().asSeconds();
            triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
            scoreSaved = true;
        }
        return;
    }
    checkRandomCollection();
}

// Random collection - routes to maths or english collection
// depending on what the RandomManager currently has active.
void Game::checkRandomCollection()
{
    if (rm->isComplete())
    {
        float elapsed = sessionClock.getElapsedTime().asSeconds();
        triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
        scoreSaved = true;
        sessionComplete = true;
        return;
    }

    if (rm->activeIsMaths())
    {
        // Maths question active
        sf::Vector2i head = snake.getHead();

        for (auto& item : gridItems)
        {
            if (item.collected || item.cell != head) continue;

            item.collected = true;
            collectedAnswers.push_back(item.value);
            snake.grow();

            QuestionManager* active = rm->mathsManager();
            int blanksNeeded = active->current().blankCount();

            if ((int)collectedAnswers.size() >= blanksNeeded)
            {
                if (active->submit(collectedAnswers))
                {
                    questionsScore++;
                    wrongAnswer = false;
                    playSound(sndCorrect);
                    collectedAnswers.clear();
                    rm->advance();

                    if (rm->isComplete())
                    {
                        float elapsed = sessionClock.getElapsedTime().asSeconds();
                        triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
                        scoreSaved = true;
                        sessionComplete = true;
                    }
                    else
                    {
                        // Respawn appropriate tile type for next question
                        if (rm->activeIsMaths()) spawnGridItems();
                        else                     spawnLetterItems();
                    }
                }
                else
                {
                    wrongAnswer = true;
                    wrongAnswerClock.restart();
                    collectedAnswers.clear();
                    playSound(sndWrong);
                    spawnGridItems();
                }
            }
            break;
        }

        if (wrongAnswer && wrongAnswerClock.getElapsedTime().asSeconds() > WRONG_FLASH_DURATION)
            wrongAnswer = false;
    }
    else
    {
        // English question active
        sf::Vector2i head = snake.getHead();

        for (auto& item : letterItems)
        {
            if (item.collected || item.cell != head) continue;

            item.collected = true;
            EnglishManager* active = rm->englishManager();
            LetterResult result = active->submitLetter(std::tolower(item.letter));

            if (result == LetterResult::CORRECT_MORE)
            {
                snake.grow();
                wrongLetter = false;
                playSound(sndCorrect);
                spawnLetterItems();
            }
            else if (result == LetterResult::CORRECT_DONE)
            {
                snake.grow();
                wrongLetter = false;
                questionsScore++;
                playSound(sndCorrect);
                rm->advance();

                if (rm->isComplete())
                {
                    float elapsed = sessionClock.getElapsedTime().asSeconds();
                    triggerNameEntry(currentScoreModeKey(), questionsScore, elapsed);
                    scoreSaved = true;
                    sessionComplete = true;
                }
                else
                {
                    if (rm->activeIsMaths()) spawnGridItems();
                    else                     spawnLetterItems();
                }
            }
            else   // WRONG
            {
                wrongLetter = true;
                wrongLetterClock.restart();
                playSound(sndWrong);
                spawnLetterItems();
            }
            break;
        }

        if (wrongLetter && wrongLetterClock.getElapsedTime().asSeconds() > WRONG_FLASH_DURATION)
            wrongLetter = false;
    }
}

// Render: random snake - delegates to maths or english render
// depending on the active question type, so the HUD always
// shows the right question format.
void Game::renderRandomSnake()
{
    if (!rm) return;

    if (rm->activeIsMaths())
    {
        QuestionManager* saved = qm;
        qm = rm->mathsManager();
        renderMathsSnake();
        qm = saved;
    }
    else
    {
        EnglishManager* saved = em;
        em = rm->englishManager();
        renderEnglishSnake();
        em = saved;
    }
}

// Spawn food for classic mode (avoids snake body)
void Game::spawnFood()
{
    sf::Vector2i pos;
    do {
        pos.x = std::rand() % GRID_COLS;
        pos.y = std::rand() % GRID_ROWS;
    } while (snake.occupies(pos));
    foodPos = pos;
}

// Spawn number tiles for the current maths question.
// For Random mode, temporarily borrows the active manager from rm.
void Game::spawnGridItems()
{
    gridItems.clear();

    // In random mode qm is null - borrow the active maths manager from rm
    QuestionManager* source = qm ? qm : (rm ? rm->mathsManager() : nullptr);
    if (!source) return;

    std::vector<int> numbers = source->current().generateGridNumbers(GRID_ITEM_COUNT);

    for (int value : numbers)
    {
        sf::Vector2i cell;
        int attempts = 0;
        do {
            cell.x = std::rand() % GRID_COLS;
            cell.y = HUD_ROWS + (std::rand() % (GRID_ROWS - HUD_ROWS));
            attempts++;
        } while (attempts < 100 && (snake.occupies(cell) || [&]() {
            for (auto& it : gridItems) if (it.cell == cell) return true;
            return false;
            }()));

        gridItems.push_back({ cell, value, false });
    }
}

// Spawn letter tiles for the current English question.
// For Random mode, temporarily borrows the active manager from rm.
void Game::spawnLetterItems()
{
    letterItems.clear();

    // In random mode em is null - borrow the active english manager from rm
    EnglishManager* source = em ? em : (rm ? rm->englishManager() : nullptr);
    if (!source) return;

    std::vector<char> letters = source->current().generateGridLetters(GRID_ITEM_COUNT);

    for (char c : letters)
    {
        sf::Vector2i cell;
        int attempts = 0;
        do {
            cell.x = std::rand() % GRID_COLS;
            cell.y = HUD_ROWS + (std::rand() % (GRID_ROWS - HUD_ROWS));
            attempts++;
        } while (attempts < 100 && (snake.occupies(cell) || [&]() {
            for (auto& it : letterItems) if (it.cell == cell) return true;
            return false;
            }()));

        letterItems.push_back({ cell, c, false });
    }
}

// Store pending score data and switch to name entry state
void Game::triggerNameEntry(const std::string& modeKey, int finalScore, float finalTime)
{
    pendingName = "";
    pendingScoreMode = modeKey;
    pendingScore = finalScore;
    pendingTime = finalTime;
    inGame = false;
    menuState = 6;
}

// Save the entered name and pending score to disk
void Game::commitScore()
{
    ScoreEntry entry;
    entry.playerName = pendingName.empty() ? "ANON" : pendingName;
    entry.score = pendingScore;
    entry.timeSeconds = pendingTime;
    highScores.add(pendingScoreMode, entry);
}

std::string Game::currentScoreModeKey() const
{
    // Determine the type label
    std::string typeLabel;
    if (selectedMode == PUZZLE && selectedPuzzleType == CLASSIC)
        typeLabel = "Classic";
    else if (selectedMode == MATHS)
    {
        if (selectedMathsType == TIMES_TABLE) typeLabel = "Times Tables";
        else if (selectedMathsType == ARITHMETIC) typeLabel = "Arithmetic";
        else typeLabel = "Division";
    }
    else if (selectedMode == ENGLISH)
    {
        typeLabel = (selectedEnglishType == ALPHABET) ? "Alphabet" : "Spelling";
    }
    else if (selectedMode == RANDOM)
    {
        typeLabel = "Random";
    }
    else typeLabel = "Unknown";

    // Determine the length label
    std::string lengthLabel;
    if (selectedMode == PUZZLE && selectedPuzzleType == CLASSIC)
        lengthLabel = ScoreMode::CLASSIC_LENGTH;
    else
    {
        switch (selectedLength)
        {
        case SessionLength::ENDLESS: lengthLabel = "Endless";   break;
        case SessionLength::ROUNDS_10: lengthLabel = "10 Rounds"; break;
        case SessionLength::ROUNDS_15: lengthLabel = "15 Rounds"; break;
        case SessionLength::ROUNDS_20: lengthLabel = "20 Rounds"; break;
        case SessionLength::ROUNDS_25: lengthLabel = "25 Rounds"; break;
        default: lengthLabel = "Unknown";   break;
        }
    }

    return ScoreMode::makeKey(typeLabel, lengthLabel);
}

// Classic puzzle has no length or difficulty screens
bool Game::modeHasLengthMenu() const
{
    return !(selectedMode == PUZZLE && selectedPuzzleType == CLASSIC);
}

bool Game::modeHasDifficultyMenu() const
{
    return !(selectedMode == PUZZLE && selectedPuzzleType == CLASSIC);
}

// Returns false for any mode/subtype that has no gameplay code yet.
// Used to block ENTER on the ready screen and grey out subtype entries.
bool Game::isImplemented() const
{
    if (selectedMode == PUZZLE)
        return selectedPuzzleType == CLASSIC;   // only Classic is built
    // Maths, English, and Random are all implemented
    return true;
}

// Returns false for a specific subtype index within the current mode.
// Used to grey out unimplemented entries in the subtype menu.
bool Game::isSubtypeImplemented(int subtypeIndex) const
{
    if (selectedMode == PUZZLE)
    {
        // 0=Mirrors, 1=Classic, 2=Boxed, 3=Maze - only Classic (index 1) is done
        return subtypeIndex == 1;
    }
    // All Maths and English subtypes are implemented
    return true;
}

// Render dispatcher
void Game::render()
{
    window.clear(sf::Color(15, 15, 20));

    if (inGame)
    {
        if (selectedMode == PUZZLE && selectedPuzzleType == CLASSIC)
            renderClassicSnake();
        else if (selectedMode == MATHS)
            renderMathsSnake();
        else if (selectedMode == ENGLISH)
            renderEnglishSnake();
        else if (selectedMode == RANDOM)
            renderRandomSnake();

        if (gameOver) renderGameOver();
        if (sessionComplete) renderSessionComplete();
    }
    else
    {
        // Name entry overlays on top of the game over or session complete screen
        if (menuState == 6)
        {
            if (selectedMode == PUZZLE && selectedPuzzleType == CLASSIC)
                renderClassicSnake();
            else if (selectedMode == MATHS)
                renderMathsSnake();
            else if (selectedMode == ENGLISH)
                renderEnglishSnake();
            else if (selectedMode == RANDOM)
                renderRandomSnake();

            // Show game over or session complete behind the name entry box
            if (gameOver) renderGameOver();
            if (sessionComplete) renderSessionComplete();

            renderNameEntry();
        }
        else if (menuState == -1)
        {
            // Post name entry: show the game over or session complete screen
            // so the player can press R to restart or ESC for the menu
            if (selectedMode == PUZZLE && selectedPuzzleType == CLASSIC)
                renderClassicSnake();
            else if (selectedMode == MATHS)
                renderMathsSnake();
            else if (selectedMode == ENGLISH)
                renderEnglishSnake();
            else if (selectedMode == RANDOM)
                renderRandomSnake();

            if (gameOver) renderGameOver();
            if (sessionComplete) renderSessionComplete();
        }
        else if (menuState == 0) renderModeMenu();
        else if (menuState == 1) renderSubMenu();
        else if (menuState == 2) renderLengthMenu();
        else if (menuState == 3) renderDifficultyMenu();
        else if (menuState == 4) renderReadyScreen();
        else if (menuState == 5) renderHighScores();
    }

    window.display();
}

// Helper: centred text
void Game::drawCentredText(const std::string& str, unsigned int size, float y, sf::Color colour)
{
    sf::Text text(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(colour);
    float x = (window.getSize().x - text.getLocalBounds().size.x) / 2.f;
    text.setPosition({ x, y });
    window.draw(text);
}

// Render: classic snake
void Game::renderClassicSnake()
{
    // Draw subtle grid lines
    sf::RectangleShape line;
    line.setFillColor(sf::Color(25, 25, 30));
    for (int x = 0; x < GRID_COLS; x++)
    {
        line.setSize({ 1.f, (float)WINDOW_HEIGHT });
        line.setPosition({ x * (float)CELL_SIZE, 0 });
        window.draw(line);
    }
    for (int y = 0; y < GRID_ROWS; y++)
    {
        line.setSize({ (float)WINDOW_WIDTH, 1.f });
        line.setPosition({ 0, y * (float)CELL_SIZE });
        window.draw(line);
    }

    // Food tile
    sf::RectangleShape food(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    food.setFillColor(sf::Color(220, 60, 60));
    food.setPosition({ foodPos.x * (float)CELL_SIZE + 1.f, foodPos.y * (float)CELL_SIZE + 1.f });
    window.draw(food);

    snake.draw(window);

    // Score label top-right
    sf::Text sc(font);
    sc.setString("Score: " + std::to_string(score));
    sc.setCharacterSize(24);
    sc.setFillColor(sf::Color(180, 180, 180));
    sc.setPosition({ WINDOW_WIDTH - sc.getLocalBounds().size.x - 10.f, 6.f });
    window.draw(sc);
}

// Render: maths snake
void Game::renderMathsSnake()
{
    // HUD background strip
    sf::RectangleShape hud(sf::Vector2f((float)WINDOW_WIDTH, (float)HUD_HEIGHT));
    hud.setFillColor(sf::Color(25, 25, 35));
    window.draw(hud);

    // Build the live question string, filling blanks left-to-right as answers are collected
    std::string displayStr;
    if (qm && !qm->isComplete())
    {
        const Question& q = qm->current();
        int ci = 0;   // index into collectedAnswers; increments only for blank slots

        auto slotText = [&](bool isBlank, int val) -> std::string {
            if (!isBlank) return std::to_string(val);
            std::string s = (ci < (int)collectedAnswers.size())
                ? "[" + std::to_string(collectedAnswers[ci]) + "]"
                : "__";
            ci++;
            return s;
            };

        // Build operand symbol
        std::string opSym;
        switch (q.op)
        {
        case QuestionOp::MULTIPLY: opSym = "x"; break;
        case QuestionOp::ADD: opSym = "+"; break;
        case QuestionOp::SUBTRACT: opSym = "-"; break;
        case QuestionOp::DIVIDE: opSym = "/"; break;
        }

        displayStr = slotText(q.blankA, q.a) + " " + opSym + " "
            + slotText(q.blankB, q.b) + " = "
            + slotText(q.blankC, q.c);
    }

    sf::Color qColour = wrongAnswer ? sf::Color(220, 60, 60) : sf::Color(240, 240, 100);
    drawCentredText(displayStr, 22, 10, qColour);

    // Progress counter and elapsed timer - top-right of HUD
    if (qm)
    {
        float elapsed = sessionClock.getElapsedTime().asSeconds();
        std::string total = qm->totalQuestions() > 0
            ? std::to_string(qm->totalQuestions())
            : "inf";
        std::string prog = std::to_string(qm->questionsAnswered()) + "/" + total
            + "  " + formatTime(elapsed);
        sf::Text pt(font);
        pt.setCharacterSize(24);
        pt.setFillColor(sf::Color(140, 140, 140));
        pt.setString(prog);
        pt.setPosition({ WINDOW_WIDTH - pt.getLocalBounds().size.x - 8.f, 8.f });
        window.draw(pt);
    }

    // Solid divider line - visually marks the HUD wall the snake bounces off
    sf::RectangleShape div(sf::Vector2f((float)WINDOW_WIDTH, 3.f));
    div.setFillColor(sf::Color(100, 100, 160));
    div.setPosition({ 0, (float)HUD_HEIGHT - 1.f });
    window.draw(div);

    // Number tiles on the grid
    sf::RectangleShape tile(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    for (const auto& item : gridItems)
    {
        if (item.collected) continue;
        float px = item.cell.x * (float)CELL_SIZE + 1.f;
        float py = item.cell.y * (float)CELL_SIZE + 1.f;

        tile.setFillColor(sf::Color(50, 50, 80));
        tile.setPosition({ px, py });
        window.draw(tile);

        sf::Text lbl(font);
        lbl.setString(std::to_string(item.value));
        lbl.setCharacterSize(14);
        lbl.setFillColor(sf::Color(220, 220, 100));
        lbl.setPosition({
            px + (CELL_SIZE - lbl.getLocalBounds().size.x) / 2.f - 1.f,
            py + (CELL_SIZE - lbl.getLocalBounds().size.y) / 2.f - 4.f
            });
        window.draw(lbl);
    }

    snake.draw(window);
}

// Render: English snake
void Game::renderEnglishSnake()
{
    // HUD background strip
    sf::RectangleShape hud(sf::Vector2f((float)WINDOW_WIDTH, (float)HUD_HEIGHT));
    hud.setFillColor(sf::Color(25, 25, 35));
    window.draw(hud);

    // Question display - red if last answer was wrong
    if (em && !em->isComplete())
    {
        sf::Color qColour = wrongLetter ? sf::Color(220, 60, 60) : sf::Color(240, 240, 100);
        drawCentredText(em->current().getDisplayString(), 20, 12, qColour);
    }

    // Progress + timer
    if (em)
    {
        float elapsed = sessionClock.getElapsedTime().asSeconds();
        std::string total = em->totalQuestions() > 0
            ? std::to_string(em->totalQuestions())
            : "inf";
        std::string prog = std::to_string(em->questionsAnswered()) + "/" + total
            + "  " + formatTime(elapsed);
        sf::Text pt(font);
        pt.setCharacterSize(24);
        pt.setFillColor(sf::Color(140, 140, 140));
        pt.setString(prog);
        pt.setPosition({ WINDOW_WIDTH - pt.getLocalBounds().size.x - 8.f, 8.f });
        window.draw(pt);
    }

    // HUD wall divider
    sf::RectangleShape div(sf::Vector2f((float)WINDOW_WIDTH, 3.f));
    div.setFillColor(sf::Color(100, 100, 160));
    div.setPosition({ 0, (float)HUD_HEIGHT - 1.f });
    window.draw(div);

    // Letter tiles on the grid
    sf::RectangleShape tile(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    for (const auto& item : letterItems)
    {
        if (item.collected) continue;
        float px = item.cell.x * (float)CELL_SIZE + 1.f;
        float py = item.cell.y * (float)CELL_SIZE + 1.f;

        tile.setFillColor(sf::Color(50, 80, 50));
        tile.setPosition({ px, py });
        window.draw(tile);

        sf::Text lbl(font);
        lbl.setString(std::string(1, item.letter));
        lbl.setCharacterSize(16);
        lbl.setFillColor(sf::Color(140, 220, 140));
        lbl.setPosition({
            px + (CELL_SIZE - lbl.getLocalBounds().size.x) / 2.f - 1.f,
            py + (CELL_SIZE - lbl.getLocalBounds().size.y) / 2.f - 4.f
            });
        window.draw(lbl);
    }

    snake.draw(window);
}

// Render: game over overlay
void Game::renderGameOver()
{
    sf::RectangleShape overlay(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    drawCentredText("GAME OVER", 40, 180, sf::Color(220, 60, 60));

    if (selectedMode == MATHS || selectedMode == ENGLISH)
        drawCentredText("Answered: " + std::to_string(questionsScore), 24, 250, sf::Color(210, 210, 210));
    else
        drawCentredText("Score: " + std::to_string(score), 24, 250, sf::Color(210, 210, 210));

    drawCentredText("R - Play again", 18, 310, sf::Color(80, 200, 120));
    drawCentredText("ESC - Main menu", 18, 345, sf::Color(120, 120, 120));
}

// Render: session complete overlay
void Game::renderSessionComplete()
{
    sf::RectangleShape overlay(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    float elapsed = (pendingTime > 0.f) ? pendingTime : sessionClock.getElapsedTime().asSeconds();

    drawCentredText("COMPLETE!", 40, 150, sf::Color(80, 200, 120));

    int total = qm ? qm->totalQuestions() : (em ? em->totalQuestions() : 0);
    if (total > 0)
        drawCentredText("Score: " + std::to_string(questionsScore) + " / " + std::to_string(total),
            24, 215, sf::Color(240, 240, 100));
    else
        drawCentredText("Score: " + std::to_string(questionsScore),
            24, 215, sf::Color(240, 240, 100));

    drawCentredText("Time: " + formatTime(elapsed), 20, 255, sf::Color(200, 200, 200));
}

// Render: name entry overlay (on top of session complete)
void Game::renderNameEntry()
{
    sf::RectangleShape overlay(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(overlay);

    // Input box
    sf::RectangleShape box(sf::Vector2f(360, 180));
    box.setFillColor(sf::Color(30, 30, 45));
    box.setOutlineColor(sf::Color(80, 200, 120));
    box.setOutlineThickness(2);
    box.setPosition({ (WINDOW_WIDTH - 360) / 2.f, 305 });
    window.draw(box);

    drawCentredText("Enter your name (up to 8 chars)", 16, 318, sf::Color(160, 160, 160));
    drawCentredText(pendingName + "_", 28, 352, sf::Color(240, 240, 100));
    drawCentredText("ENTER to confirm", 14, 402, sf::Color(80, 200, 120));
}

// Render: high scores screen
// Left/right = game type, up/down = length tier
void Game::renderHighScores()
{
    drawCentredText("HIGH SCORES", 36, 28, sf::Color(80, 200, 120));

    // Game type row (left/right navigation())
    int modeCount = (int)ScoreMode::TYPES.size();
    const std::string& currentType = ScoreMode::TYPES[hsModeIndex];

    // Left/right arrows
    sf::Text arr(font);
    arr.setCharacterSize(22);
    arr.setFillColor(sf::Color(140, 140, 140));
    arr.setString("<");
    arr.setPosition({ 18, 68 });
    window.draw(arr);
    arr.setString(">");
    arr.setPosition({ WINDOW_WIDTH - 34.f, 68 });
    window.draw(arr);

    // Type name centred
    drawCentredText(currentType, 22, 66, sf::Color(240, 200, 60));

    // Type dot indicators (horizontal strip under the title)
    float typeDotSpacing = 14.f;
    float typeDotsX = (WINDOW_WIDTH - modeCount * typeDotSpacing) / 2.f;
    for (int i = 0; i < modeCount; i++)
    {
        sf::CircleShape dot(3.f);
        dot.setFillColor(i == hsModeIndex ? sf::Color(240, 200, 60) : sf::Color(60, 60, 60));
        dot.setPosition({ typeDotsX + i * typeDotSpacing, 95 });
        window.draw(dot);
    }

    // Length column (up/down navigation)
    // Classic has no length variants so hide the length column for it
    bool isClassic = (currentType == "Classic");

    std::string lengthLabel;
    std::string tableKey;

    if (isClassic)
    {
        lengthLabel = ScoreMode::CLASSIC_LENGTH;
        tableKey = ScoreMode::makeKey(currentType, ScoreMode::CLASSIC_LENGTH);
    }
    else
    {
        int lengthCount = (int)ScoreMode::LENGTHS.size();

        // Up/down arrows on the left side of the scores area
        sf::Text upArr(font);
        upArr.setCharacterSize(18);
        upArr.setFillColor(sf::Color(140, 140, 140));
        upArr.setString("^");
        upArr.setPosition({ 18, 140 });
        window.draw(upArr);
        upArr.setString("v");
        upArr.setPosition({ 18, WINDOW_HEIGHT - 80.f });
        window.draw(upArr);

        // Length labels listed down the left side, current one highlighted
        float ly = 140.f;
        for (int i = 0; i < lengthCount; i++, ly += 26.f)
        {
            sf::Text lt(font);
            lt.setCharacterSize(13);
            lt.setFillColor(i == hsLengthIndex
                ? sf::Color(80, 200, 120)
                : sf::Color(70, 70, 70));
            lt.setString(ScoreMode::LENGTHS[i]);
            lt.setPosition({ 40.f, ly });
            window.draw(lt);
        }

        lengthLabel = ScoreMode::LENGTHS[hsLengthIndex];
        tableKey = ScoreMode::makeKey(currentType, lengthLabel);
    }

    // Score table for the selected type + length
    const auto& entries = highScores.getEntries(tableKey);

    // Divider between length column and score table
    float tableX = isClassic ? 50.f : 145.f;
    float tableW = WINDOW_WIDTH - tableX - 20.f;

    sf::RectangleShape sep(sf::Vector2f(tableW, 1.f));
    sep.setFillColor(sf::Color(50, 50, 60));
    sep.setPosition({ tableX, 138 });
    window.draw(sep);

    if (entries.empty())
    {
        // Centre the "no scores" message in the table area
        sf::Text none(font);
        none.setCharacterSize(15);
        none.setFillColor(sf::Color(100, 100, 100));
        none.setString("No scores yet");
        float nx = tableX + (tableW - none.getLocalBounds().size.x) / 2.f;
        none.setPosition({ nx, 240 });
        window.draw(none);
    }
    else
    {
        // Header
        sf::Text hdr(font);
        hdr.setCharacterSize(13);
        hdr.setFillColor(sf::Color(80, 80, 100));
        hdr.setString("  #  NAME         SCORE  TIME");
        hdr.setPosition({ tableX + 4, 145 });
        window.draw(hdr);

        float y = 168.f;
        for (int i = 0; i < (int)entries.size(); i++)
        {
            const auto& e = entries[i];
            std::ostringstream line;
            line << std::setw(2) << (i + 1) << ".  "
                << std::left << std::setw(10) << e.playerName
                << std::right << std::setw(5) << e.score << "  ";
            line << (e.timeSeconds > 0.f ? formatTime(e.timeSeconds) : "  ---");

            sf::Text row(font);
            row.setCharacterSize(17);
            row.setFillColor(i == 0 ? sf::Color(240, 200, 60) : sf::Color(200, 200, 200));
            row.setString(line.str());
            row.setPosition({ tableX + 4, y });
            window.draw(row);
            y += 28.f;
        }
    }

    drawCentredText("< > type  |  ^ v length  |  ESC back", 12,
        (float)WINDOW_HEIGHT - 24.f, sf::Color(70, 70, 70));
}

// Screen 0: Mode selection
void Game::renderModeMenu()
{
    drawCentredText("EDU SNAKE", 42, 50, sf::Color(80, 200, 120));
    drawCentredText("Select a game mode", 18, 104, sf::Color(160, 160, 160));

    struct MenuItem { std::string label; std::string desc; };
    const MenuItem items[MODE_MENU_COUNT] = {
        { "Maths",       "Answer arithmetic, times tables, or division" },
        { "English",     "Practice the alphabet or test your spelling"  },
        { "Puzzle",      "Navigate mirrors, mazes, and classic puzzles" },
        { "Random",      "Surprise mix - any mode, any question type"   },
        { "High Scores", "Check your best scores at any time"           },
    };

    const sf::Color normal = sf::Color(210, 210, 210);
    const sf::Color sel = sf::Color(80, 200, 120);
    const sf::Color dimDesc = sf::Color(120, 120, 120);
    const sf::Color gold = sf::Color(240, 200, 60);
    const sf::Color unavail = sf::Color(70, 70, 70);

    float y = 158.f;
    for (int i = 0; i < MODE_MENU_COUNT; i++, y += 72.f)
    {
        bool isSel = (cursorIndex == i);
        if (isSel)
        {
            sf::Text arrow(font);
            arrow.setString(">");
            arrow.setCharacterSize(22);
            arrow.setFillColor(sel);
            arrow.setPosition({ 100, y + 4 });
            window.draw(arrow);
        }
        drawCentredText(items[i].label, 24, y, isSel ? sel : (i == 4 ? gold : normal));
        drawCentredText(items[i].desc, 13, y + 28, dimDesc);
    }

    drawCentredText("UP / DOWN  |  ENTER to confirm", 13, WINDOW_HEIGHT - 30, sf::Color(80, 80, 80));
}

// Screen 1: Subtype selection
void Game::renderSubMenu()
{
    drawCentredText(getModeName(selectedMode) + " - Choose type", 30, 70, sf::Color(80, 200, 120));

    int count = getSubMenuCount();
    const sf::Color normal = sf::Color(210, 210, 210);
    const sf::Color sel = sf::Color(80, 200, 120);
    const sf::Color unavail = sf::Color(70, 70, 70);
    float y = 175.f;

    for (int i = 0; i < count; i++, y += 70.f)
    {
        bool isSel = (cursorIndex == i);
        bool isDisabled = !isSubtypeImplemented(i);

        if (isSel && !isDisabled)
        {
            sf::Text arrow(font);
            arrow.setString(">");
            arrow.setCharacterSize(20);
            arrow.setFillColor(sel);
            arrow.setPosition({ 110, y + 4 });
            window.draw(arrow);
        }

        sf::Color col = isDisabled ? unavail : (isSel ? sel : normal);
        drawCentredText(getSubMenuOption(i), 24, y, col);

        // Show a small "coming soon" hint under disabled entries
        if (isDisabled)
            drawCentredText("coming soon", 12, y + 28, unavail);
    }

    drawCentredText("UP / DOWN  |  ENTER  |  ESC to go back", 13, WINDOW_HEIGHT - 36, sf::Color(80, 80, 80));
}

// Screen 2: Session length selection
void Game::renderLengthMenu()
{
    drawCentredText("How many questions?", 30, 70, sf::Color(80, 200, 120));

    struct LenItem { SessionLength val; std::string label; std::string desc; };
    static const LenItem items[] = {
        { SessionLength::ENDLESS,   "Endless",    "Keep going until you crash" },
        { SessionLength::ROUNDS_10, "10 rounds",  "Quick session"              },
        { SessionLength::ROUNDS_15, "15 rounds",  "Standard session"           },
        { SessionLength::ROUNDS_20, "20 rounds",  "Longer practice"            },
        { SessionLength::ROUNDS_25, "25 rounds",  "Full challenge"             },
    };
    static const int COUNT = 5;

    const sf::Color normal = sf::Color(210, 210, 210);
    const sf::Color sel = sf::Color(80, 200, 120);
    const sf::Color dimDesc = sf::Color(120, 120, 120);
    float y = 148.f;

    for (int i = 0; i < COUNT; i++, y += 66.f)
    {
        bool isSel = (cursorIndex == i);
        if (isSel)
        {
            sf::Text arrow(font);
            arrow.setString(">");
            arrow.setCharacterSize(20);
            arrow.setFillColor(sel);
            arrow.setPosition({ 100, y + 4 });
            window.draw(arrow);
        }
        drawCentredText(items[i].label, 22, y, isSel ? sel : normal);
        drawCentredText(items[i].desc, 13, y + 26, dimDesc);
    }

    drawCentredText("UP / DOWN  |  ENTER  |  ESC to go back", 13, WINDOW_HEIGHT - 30, sf::Color(80, 80, 80));
}

// Screen 3: Difficulty selection
void Game::renderDifficultyMenu()
{
    drawCentredText("Choose difficulty", 30, 80, sf::Color(80, 200, 120));

    static const BlankDifficulty diffs[] = {
        BlankDifficulty::EASY, BlankDifficulty::MEDIUM, BlankDifficulty::HARD
    };
    static const int COUNT = 3;

    const sf::Color normal = sf::Color(210, 210, 210);
    const sf::Color sel = sf::Color(80, 200, 120);
    const sf::Color dimDesc = sf::Color(120, 120, 120);
    float y = 190.f;

    for (int i = 0; i < COUNT; i++, y += 90.f)
    {
        bool isSel = (cursorIndex == i);
        if (isSel)
        {
            sf::Text arrow(font);
            arrow.setString(">");
            arrow.setCharacterSize(20);
            arrow.setFillColor(sel);
            arrow.setPosition({ 100, y + 4 });
            window.draw(arrow);
        }
        drawCentredText(getDifficultyLabel(diffs[i]), 22, y, isSel ? sel : normal);
        drawCentredText(getDifficultyDescription(diffs[i]), 13, y + 28, dimDesc);
    }

    drawCentredText("UP / DOWN  |  ENTER  |  ESC to go back", 13, WINDOW_HEIGHT - 30, sf::Color(80, 80, 80));
}

// Screen 4: Ready screen
void Game::renderReadyScreen()
{
    drawCentredText("READY?", 48, 70, sf::Color(80, 200, 120));
    drawCentredText("Mode:  " + getModeName(selectedMode), 20, 175, sf::Color(210, 210, 210));

    if (selectedMode == MATHS)
        drawCentredText("Type:  " + getMathsTypeName(selectedMathsType), 20, 205, sf::Color(210, 210, 210));
    else if (selectedMode == ENGLISH)
        drawCentredText("Type:  " + getEnglishTypeName(selectedEnglishType), 20, 205, sf::Color(210, 210, 210));
    else if (selectedMode == PUZZLE)
        drawCentredText("Type:  " + getPuzzleTypeName(selectedPuzzleType), 20, 205, sf::Color(210, 210, 210));

    if (modeHasLengthMenu())
        drawCentredText("Length:  " + getLengthLabel(selectedLength), 20, 235, sf::Color(210, 210, 210));
    if (modeHasDifficultyMenu())
        drawCentredText("Difficulty:  " + getDifficultyLabel(selectedDifficulty), 20, 265, sf::Color(210, 210, 210));

    sf::RectangleShape sep(sf::Vector2f(300, 2));
    sep.setFillColor(sf::Color(60, 60, 60));
    sep.setPosition({ (WINDOW_WIDTH - 300) / 2.f, 300 });
    window.draw(sep);

    drawCentredText("Press ENTER to start", 20, 318,
        isImplemented() ? sf::Color(80, 200, 120) : sf::Color(70, 70, 70));
    if (!isImplemented())
        drawCentredText("(coming soon)", 15, 344, sf::Color(70, 70, 70));
    drawCentredText("Press ESC to go back", 16, 352, sf::Color(100, 100, 100));
}

// Game control
void Game::startGame()
{
    gameOver = false;
    sessionComplete = false;
    scoreSaved = false;
    score = 0;
    questionsScore = 0;
    pendingTime = 0.f;
    collectedAnswers.clear();
    wrongAnswer = false;
    wrongLetter = false;
    gridItems.clear();
    letterItems.clear();

    delete qm; qm = nullptr;
    delete em; em = nullptr;
    delete rm; rm = nullptr;

    if (selectedMode == PUZZLE && selectedPuzzleType == CLASSIC)
    {
        snake.reset(0);
        spawnFood();
    }
    else if (selectedMode == MATHS)
    {
        snake.reset(HUD_ROWS);
        SessionType st = (selectedMathsType == TIMES_TABLE) ? SessionType::TIMES_TABLE :
            (selectedMathsType == ARITHMETIC) ? SessionType::ARITHMETIC :
            SessionType::DIVISION;
        qm = new QuestionManager(st, selectedLength, selectedDifficulty);
        spawnGridItems();
        sessionClock.restart();
    }
    else if (selectedMode == ENGLISH)
    {
        snake.reset(HUD_ROWS);
        EnglishType et = (selectedEnglishType == ALPHABET) ? EnglishType::ALPHABET
            : EnglishType::SPELLING;
        EnglishDifficulty ed = (selectedDifficulty == BlankDifficulty::EASY) ? EnglishDifficulty::EASY :
            (selectedDifficulty == BlankDifficulty::HARD) ? EnglishDifficulty::HARD :
            EnglishDifficulty::MEDIUM;
        int qCount = (selectedLength == SessionLength::ENDLESS) ? 0
            : static_cast<int>(selectedLength);
        em = new EnglishManager(et, ed, qCount);
        spawnLetterItems();
        sessionClock.restart();
    }
    else if (selectedMode == RANDOM)
    {
        snake.reset(HUD_ROWS);
        int qCount = (selectedLength == SessionLength::ENDLESS) ? 0
            : static_cast<int>(selectedLength);
        rm = new RandomManager(qCount);
        // Spawn the right tile type for the first question
        if (rm->activeIsMaths()) spawnGridItems();
        else                     spawnLetterItems();
        sessionClock.restart();
    }

    inGame = true;
    moveClock.restart();
}

void Game::stopGame()
{
    inGame = false;
    menuState = 0;
    cursorIndex = 0;
    selectedMode = NONE;

    delete qm; qm = nullptr;
    delete em; em = nullptr;
    delete rm; rm = nullptr;
}

void Game::loadLevel() {}
void Game::unloadLevel() {}
void Game::pauseGame() {}

// Format mm:ss
std::string Game::formatTime(float seconds) const
{
    int mins = (int)seconds / 60;
    int secs = (int)seconds % 60;
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << mins << ":"
        << std::setw(2) << std::setfill('0') << secs;
    return ss.str();
}

// Name / description helpers
std::string Game::getModeName(GAMEMODE mode) const
{
    switch (mode)
    {
    case MATHS: return "Maths";
    case ENGLISH: return "English";
    case PUZZLE: return "Puzzle";
    case RANDOM: return "Random";
    default: return "Unknown";
    }
}

std::string Game::getModeDescription(GAMEMODE mode) const
{
    switch (mode)
    {
    case MATHS: return "Answer arithmetic, times tables, or division questions";
    case ENGLISH: return "Practice the alphabet or test your spelling";
    case PUZZLE: return "Navigate mirrors, mazes, and classic puzzles";
    case RANDOM: return "Surprise mix --- any mode, any question type";
    default: return "";
    }
}

std::string Game::getMathsTypeName(MATHS_GAME_TYPES t) const
{
    switch (t)
    {
    case TIMES_TABLE: return "Times Tables";
    case ARITHMETIC: return "Arithmetic";
    case DIVISION: return "Division";
    default: return "Unknown";
    }
}

std::string Game::getEnglishTypeName(ENGLISH_GAME_TYPES t) const
{
    switch (t)
    {
    case ALPHABET: return "Alphabet";
    case SPELLING: return "Spelling";
    default: return "Unknown";
    }
}

std::string Game::getPuzzleTypeName(PUZZLE_GAME_TYPES t) const
{
    switch (t)
    {
    case MIRRORS: return "Mirrors";
    case CLASSIC: return "Classic";
    case BOXED: return "Boxed";
    case MAZE: return "Maze";
    default: return "Unknown";
    }
}

std::string Game::getLengthLabel(SessionLength l) const
{
    switch (l)
    {
    case SessionLength::ENDLESS: return "Endless";
    case SessionLength::ROUNDS_10: return "10 rounds";
    case SessionLength::ROUNDS_15: return "15 rounds";
    case SessionLength::ROUNDS_20: return "20 rounds";
    case SessionLength::ROUNDS_25: return "25 rounds";
    default: return "Unknown";
    }
}

std::string Game::getDifficultyLabel(BlankDifficulty d) const
{
    switch (d)
    {
    case BlankDifficulty::EASY: return "Easy";
    case BlankDifficulty::MEDIUM: return "Medium";
    case BlankDifficulty::HARD: return "Hard";
    default: return "Unknown";
    }
}

std::string Game::getDifficultyDescription(BlankDifficulty d) const
{
    if (selectedMode == ENGLISH)
    {
        switch (d)
        {
        case BlankDifficulty::EASY:
            return (selectedEnglishType == ALPHABET)
                ? "Letters 1 place apart"
                : "Short 3-4 letter words";
        case BlankDifficulty::MEDIUM:
            return (selectedEnglishType == ALPHABET)
                ? "Letters up to 2 places apart"
                : "Medium 4-6 letter words";
        case BlankDifficulty::HARD:
            return (selectedEnglishType == ALPHABET)
                ? "Letters up to 3 places apart"
                : "Longer 5-8 letter words";
        default: return "";
        }
    }

    // Maths descriptions
    switch (d)
    {
    case BlankDifficulty::EASY: return "Always 1 missing number";
    case BlankDifficulty::MEDIUM: return "Starts with 1, moves to 2 missing numbers";
    case BlankDifficulty::HARD: return "Always 2 missing numbers";
    default: return "";
    }
}

int Game::getSubMenuCount() const
{
    if (selectedMode == MATHS) return 3;
    if (selectedMode == ENGLISH) return 2;
    if (selectedMode == PUZZLE) return 4;
    return 0;
}

std::string Game::getSubMenuOption(int index) const
{
    if (selectedMode == MATHS)
        return getMathsTypeName(static_cast<MATHS_GAME_TYPES>(index));
    if (selectedMode == ENGLISH)
        return getEnglishTypeName(static_cast<ENGLISH_GAME_TYPES>(index));
    if (selectedMode == PUZZLE)
        return getPuzzleTypeName(static_cast<PUZZLE_GAME_TYPES>(index));
    return "";
}