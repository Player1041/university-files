#include "EnglishManager.h"
#include "WordList.h"
#include <cstdlib>
#include <algorithm>
#include <random>
#include <cassert>

// Shared RNG - same pattern used in Question.cpp for consistency
static std::mt19937& rng()
{
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

// Pick a random element from a vector
template<typename T>
static const T& randomPick(const std::vector<T>& v)
{
    return v[std::uniform_int_distribution<int>(0, (int)v.size() - 1)(rng())];
}

// EnglishQuestion methods

// HUD display string
// For ALPHABET: "Which letter comes 2 after D?" / "Which letter is 1 before G?"
// For SPELLING: "Spell: lamp  [_][a][m][p]" - filled squares show progress
std::string EnglishQuestion::getDisplayString() const
{
    if (type == EnglishType::ALPHABET)
    {
        std::string dir = (offset > 0) ? "after" : "before";
        int dist = std::abs(offset);
        std::string places = (dist == 1) ? "1 place" : std::to_string(dist) + " places";
        return "Which letter comes " + places + " " + dir + " " + std::string(1, anchorLetter) + "?";
    }
    else
    {
        // Show the word with only the hidden letter replaced by an underscore.
        // letterIndex is the index of the one missing letter.
        std::string display = "Spell:  ";
        for (int i = 0; i < (int)word.size(); i++)
        {
            if (i == letterIndex)
                display += "_";                    // this is the missing letter
            else
                display += std::toupper(word[i]);  // all other letters are shown
            display += " ";
        }
        return display;
    }
}

// Validate a collected letter against the current expected answer
bool EnglishQuestion::validateLetter(char collected) const
{
    char lc = std::tolower(collected);

    if (type == EnglishType::ALPHABET)
        return lc == std::tolower(correctLetter);
    else
        // Spelling: correctLetter is set to word[letterIndex] in makeSpellingQuestion
        return lc == std::tolower(correctLetter);
}

// True when all letters of a spelling word have been collected
bool EnglishQuestion::isWordComplete() const
{
    if (type == EnglishType::SPELLING)
        return letterIndex >= (int)word.size();
    return true;   // alphabet questions are always single-step
}

// Generate letter tiles for the grid.
// For ALPHABET: the correct letter + nearby alphabet letters as distractors.
// For SPELLING: the next required letter + other letters as distractors.
// Letters are uppercase for display; the snake collects uppercase chars.
std::vector<char> EnglishQuestion::generateGridLetters(int gridSize) const
{
    std::vector<char> tiles;

    // The correct answer is always included first
    char correct = (type == EnglishType::ALPHABET)
        ? correctLetter
        : (letterIndex < (int)word.size() ? word[letterIndex] : '?');

    tiles.push_back(std::toupper(correct));

    std::vector<char> pool;
    for (char c = 'A'; c <= 'Z'; c++)
    {
        char lc = std::tolower(c);
        if (lc != correct) pool.push_back(c);
    }

    // Bias the pool toward nearby letters to make the question meaningful
    // (adds the +-5 neighbourhood letters multiple times so they appear more often).
    // correctLetter is stored uppercase, so convert to lowercase for the arithmetic.
    int base = std::tolower(correct) - 'a';
    for (int d = -5; d <= 5; d++)
    {
        int idx = base + d;
        if (idx >= 0 && idx < 26)
        {
            char nearby = 'A' + idx;
            if (nearby != std::toupper(correct)) pool.push_back(nearby);
        }
    }

    std::shuffle(pool.begin(), pool.end(), rng());

    for (char c : pool)
    {
        if ((int)tiles.size() >= gridSize) break;
        if (std::find(tiles.begin(), tiles.end(), c) == tiles.end())
            tiles.push_back(c);
    }

    std::shuffle(tiles.begin(), tiles.end(), rng());
    return tiles;
}

// EnglishManager
EnglishManager::EnglishManager(EnglishType type, EnglishDifficulty diff, int questionCount)
    : currentIndex(0), answered(0), sessionType(type), difficulty(diff),
    endless(questionCount == 0)
{
    int count = endless ? 15 : questionCount;   // endless: seed with 15, append on demand

    if (type == EnglishType::ALPHABET)
        buildAlphabet(count);
    else
        buildSpelling(count);
}

// Build a batch of alphabet questions.
// Edge cases: cannot ask for a letter before A or after Z.
// Offsets are limited per difficulty:
//   Easy:   +/-1 only
//   Medium: +/-1 or +/-2
//   Hard:   +/-1, +/-2, or +/-3
void EnglishManager::buildAlphabet(int count)
{
    for (int i = 0; i < count; i++)
        questions.push_back(makeAlphabetQuestion());
}

// Build a batch of spelling questions from the appropriate word tier.
void EnglishManager::buildSpelling(int count)
{
    for (int i = 0; i < count; i++)
        questions.push_back(makeSpellingQuestion());
}

// Make one random alphabet question respecting edge cases and difficulty.
EnglishQuestion EnglishManager::makeAlphabetQuestion() const
{
    // Allowed offsets per difficulty
    std::vector<int> offsets;
    if (difficulty == EnglishDifficulty::EASY)
        offsets = { -1, 1 };
    else if (difficulty == EnglishDifficulty::MEDIUM)
        offsets = { -2, -1, 1, 2 };
    else
        offsets = { -3, -2, -1, 1, 2, 3 };

    // Keep trying until we get a valid anchor + offset combination
    // (avoids asking "2 before A" which has no answer)
    char anchor;
    int offset;
    int attempts = 0;
    do {
        anchor = 'a' + std::uniform_int_distribution<int>(0, 25)(rng());
        offset = offsets[std::uniform_int_distribution<int>(0, (int)offsets.size() - 1)(rng())];
        attempts++;
    } while ((anchor + offset < 'a' || anchor + offset > 'z') && attempts < 200);

    EnglishQuestion q;
    q.type = EnglishType::ALPHABET;
    q.anchorLetter = std::toupper(anchor);
    q.offset = offset;
    q.correctLetter = std::toupper(anchor + offset);
    q.letterIndex = 0;
    return q;
}

// Make one random spelling question from the appropriate word tier.
// Tracks used words within this manager instance to avoid repeats.
EnglishQuestion EnglishManager::makeSpellingQuestion() const
{
    const std::vector<std::string>* wordTier = nullptr;

    if (difficulty == EnglishDifficulty::EASY)
        wordTier = &WordList::EASY;
    else if (difficulty == EnglishDifficulty::MEDIUM)
        wordTier = &WordList::MEDIUM;
    else
        wordTier = &WordList::HARD;

    // Build a set of words already used so it doesn't repeat
    std::vector<std::string> usedWords;
    for (const auto& q : questions)
        if (q.type == EnglishType::SPELLING)
            usedWords.push_back(q.word);

    // Pick a random unused word, cycling back if exhausted
    std::vector<std::string> available;
    for (const auto& w : *wordTier)
        if (std::find(usedWords.begin(), usedWords.end(), w) == usedWords.end())
            available.push_back(w);

    if (available.empty()) available = *wordTier;

    std::string chosen = randomPick(available);

    // Pick one random letter index to hide
    int hiddenIndex = std::uniform_int_distribution<int>(0, (int)chosen.size() - 1)(rng());

    EnglishQuestion q;
    q.type = EnglishType::SPELLING;
    q.word = chosen;
    q.letterIndex = hiddenIndex;  // letterIndex now means "which letter is hidden"
    q.correctLetter = std::toupper(chosen[hiddenIndex]);
    q.offset = 0;
    return q;
}

// Accessors
const EnglishQuestion& EnglishManager::current() const
{
    int safe = std::min(currentIndex, (int)questions.size() - 1);
    return questions[safe];
}

LetterResult EnglishManager::submitLetter(char collected)
{
    EnglishQuestion& q = questions[currentIndex];

    if (q.type == EnglishType::SPELLING)
    {
        // Build the word the player has formed by substituting their collected
        // letter into the blank slot at letterIndex
        std::string candidate = q.word;
        candidate[q.letterIndex] = std::tolower(collected);

        // Accept if the candidate is any real word in any of the three tiers,
        // not just the original target. This means picking up 'Y' for HA_
        // gives "hay" or T for "hat" which is also a valid word and should award a point.
        bool isValidWord = false;
        for (const auto& w : WordList::EASY)
            if (w == candidate) { isValidWord = true; break; }
        if (!isValidWord)
            for (const auto& w : WordList::MEDIUM)
                if (w == candidate) { isValidWord = true; break; }
        if (!isValidWord)
            for (const auto& w : WordList::HARD)
                if (w == candidate) { isValidWord = true; break; }

        if (!isValidWord)
            return LetterResult::WRONG;

        // Valid word formed - advance to next question
        answered++;
        currentIndex++;

        if (endless && currentIndex >= (int)questions.size() - 1)
            appendNextQuestion();

        return LetterResult::CORRECT_DONE;
    }

    // Alphabet questions: single letter check as before
    if (!q.validateLetter(collected))
        return LetterResult::WRONG;

    answered++;
    currentIndex++;

    if (endless && currentIndex >= (int)questions.size() - 1)
        appendNextQuestion();

    return LetterResult::CORRECT_DONE;
}

bool EnglishManager::isComplete() const
{
    if (endless) return false;
    return currentIndex >= (int)questions.size();
}

int EnglishManager::questionsAnswered() const { return answered; }
int EnglishManager::totalQuestions()    const { return endless ? 0 : (int)questions.size(); }

// Append one more question (used by endless mode)
void EnglishManager::appendNextQuestion()
{
    if (sessionType == EnglishType::ALPHABET)
        questions.push_back(makeAlphabetQuestion());
    else
        questions.push_back(makeSpellingQuestion());
}