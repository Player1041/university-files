#pragma once
#include <string>
#include <vector>

// The type of English session
enum class EnglishType { ALPHABET, SPELLING };

// Difficulty tier - controls question complexity and word length
enum class EnglishDifficulty { EASY, MEDIUM, HARD };

// For ALPHABET: "Which letter comes N places after/before X?"
// The player collects a single letter tile from the grid.

// For SPELLING: "Spell the word [word]"
// The player collects letter tiles one at a time, left-to-right.
struct EnglishQuestion
{
    EnglishType type;

    char anchorLetter;    // the letter shown in the question (e.g. 'E')
    int offset;          // how many places to move (+ve = after, -ve = before)
    char correctLetter; // the expected answer letter

    std::string word;// the word to spell (e.g. "lamp")
    int letterIndex;// which letter the player must collect next (0 = first)

    std::string getDisplayString() const;

    // For ALPHABET: returns true if the collected letter matches correctLetter.
    // For SPELLING: returns true if the collected letter matches word[letterIndex].
    bool validateLetter(char collected) const;

    // True when the full spelling is complete (letterIndex >= word.length())
    bool isWordComplete() const;

    // Generates letter tiles to scatter on the grid.
    // Always includes the correct next letter; fills the rest with plausible distractors.
    std::vector<char> generateGridLetters(int gridSize) const;
};

// Return value from submitLetter
enum class LetterResult
{
    WRONG, // wrong letter, question stays the same
    CORRECT_MORE, // correct letter, but more letters still needed (spelling only)
    CORRECT_DONE  // correct letter and question fully answered
};

class EnglishManager
{
public:
    // questionCount: how many questions (10/15/20/25 or 0 for endless)
    EnglishManager(EnglishType type, EnglishDifficulty difficulty, int questionCount);

    const EnglishQuestion& current() const;

    // Submit a collected letter and get back a LetterResult.
    // On CORRECT_MORE: letterIndex has advanced, caller should respawn tiles.
    // On CORRECT_DONE: question fully answered, manager has advanced to next.
    // On WRONG: nothing changed, caller should respawn tiles.
    LetterResult submitLetter(char collected);

    bool isComplete() const;
    int questionsAnswered() const;
    int totalQuestions() const;  // 0 = endless

    void appendNextQuestion();

private:
    std::vector<EnglishQuestion> questions;
    int currentIndex;
    int answered;
    EnglishType sessionType;
    EnglishDifficulty difficulty;
    bool endless;

    void buildAlphabet(int count);
    void buildSpelling(int count);

    EnglishQuestion makeAlphabetQuestion() const;
    EnglishQuestion makeSpellingQuestion() const;
};