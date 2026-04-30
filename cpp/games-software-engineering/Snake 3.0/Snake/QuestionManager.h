#pragma once
#include "Question.h"
#include <vector>

// Type of maths session
enum class SessionType { TIMES_TABLE, ARITHMETIC, DIVISION };

// How many questions in a session (0 = endless)
enum class SessionLength { ROUNDS_10 = 10, ROUNDS_15 = 15, ROUNDS_20 = 20, ROUNDS_25 = 25, ENDLESS = 0 };

// How many blanks per question
// EASY - always 1 blank
// MEDIUM - first half 1 blank, second half 2 blanks
// HARD - always 2 blanks (for maths types that support it)
enum class BlankDifficulty { EASY, MEDIUM, HARD };

class QuestionManager
{
public:
    QuestionManager(SessionType type,
        SessionLength length = SessionLength::ROUNDS_10,
        BlankDifficulty blanks = BlankDifficulty::EASY);

    const Question& current() const;

    // Submit collected answers. Returns true if correct (advances), false if wrong.
    bool submit(const std::vector<int>& collectedAnswers);

    bool isComplete() const;
    int questionsAnswered() const;
    int totalQuestions() const;   // 0 = endless
    int currentBlankCount() const;

    SessionType getSessionType() const;
    SessionLength getSessionLength() const;
    BlankDifficulty getBlankDifficulty() const;

    // Append one more question (used by endless mode)
    void appendNextQuestion();

private:
    std::vector<Question> questions;
    int currentIndex;
    int answered;
    SessionType sessionType;
    SessionLength sessionLength;
    BlankDifficulty blankDifficulty;
    bool endless;

    // Number difficulty ranges derived from session type
    int numDifficultyLevel;   // 1-3, maps from BlankDifficulty for numeric range

    void buildQuestions(int count);

    // Make a single question appropriate for the session type and difficulty
    Question makeQuestion(bool forceDouble) const;
    Question makeTimesTableQuestion(bool forceDouble) const;
    Question makeArithmeticQuestion() const;
    Question makeDivisionQuestion() const;

    void reshuffleCurrentQuestion();
};