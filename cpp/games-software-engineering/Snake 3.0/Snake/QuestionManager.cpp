#include "QuestionManager.h"
#include <cstdlib>
#include <algorithm>
#include <random>

// RNG Engine
static std::mt19937& rng()
{
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

// Small helper to produce a random int in [lo, hi]
static int randRange(int lo, int hi)
{
    return lo + std::uniform_int_distribution<int>(0, hi - lo)(rng());
}

// Constructor
QuestionManager::QuestionManager(SessionType type, SessionLength length, BlankDifficulty blanks)
    : currentIndex(0), answered(0),
    sessionType(type), sessionLength(length), blankDifficulty(blanks),
    endless(length == SessionLength::ENDLESS),
    numDifficultyLevel(blanks == BlankDifficulty::EASY ? 1 :
        blanks == BlankDifficulty::HARD ? 3 : 2)
{
    int count = endless ? 15 : static_cast<int>(length);
    buildQuestions(count);
}

// Build a set of questions using the blank difficulty rules:
//   EASY - all single-blank
//   MEDIUM - first half single-blank, second half double-blank
//   HARD - all double-blank
void QuestionManager::buildQuestions(int count)
{
    for (int i = 0; i < count; i++)
    {
        bool forceDouble = false;

        if (blankDifficulty == BlankDifficulty::HARD)
            forceDouble = true;
        else if (blankDifficulty == BlankDifficulty::MEDIUM)
            forceDouble = (i >= count / 2);   // second half switches to double-blank

        questions.push_back(makeQuestion(forceDouble));
    }
}

// Dispatch to the right question factory
Question QuestionManager::makeQuestion(bool forceDouble) const
{
    switch (sessionType)
    {
    case SessionType::TIMES_TABLE: return makeTimesTableQuestion(forceDouble);
    case SessionType::ARITHMETIC: return makeArithmeticQuestion();    // no double-blank
    case SessionType::DIVISION: return makeDivisionQuestion();       // no double-blank
    default: return makeTimesTableQuestion(forceDouble);
    }
}

// Times table question.
// Uses the 5x table (table picker is a future menu step).
// Difficulty affects the factor range:
//   Easy: factors 1-5   (small, familiar products)
//   Medium: factors 1-10
//   Hard: factors 1-12  (full table)
// forceDouble hides both operands (used by HARD blank difficulty).
Question QuestionManager::makeTimesTableQuestion(bool forceDouble) const
{
    const int tableOf = 5;

    int maxFactor = (numDifficultyLevel == 1) ? 5 :
        (numDifficultyLevel == 3) ? 12 : 10;

    int factor = randRange(1, maxFactor);

    if (forceDouble)
        return Question::makeDouble(tableOf, factor);
    else
        return Question::makeSingle(tableOf, factor, QuestionOp::MULTIPLY);
}

// Arithmetic question (addition and subtraction, mixed).
// Number range scales with numDifficultyLevel.
// For harder difficulty the number ranges are wider instead.
Question QuestionManager::makeArithmeticQuestion() const
{
    int maxVal = (numDifficultyLevel == 1) ? 10 :
        (numDifficultyLevel == 3) ? 50 : 20;

    int a = randRange(1, maxVal);
    int b = randRange(1, maxVal);

    bool doAdd = (answered % 2 == 0) || (std::rand() % 2 == 0);

    if (doAdd)
        return Question::makeSingle(a, b, QuestionOp::ADD);
    else
    {
        if (a < b) std::swap(a, b);   // keep result non-negative
        return Question::makeSingle(a, b, QuestionOp::SUBTRACT);
    }
}

// Division question.
// Always generates exact integer division (a = b * c).
// No double-blank - uses wider number ranges for harder difficulty.
Question QuestionManager::makeDivisionQuestion() const
{
    int maxDiv = (numDifficultyLevel == 1) ? 5 :
        (numDifficultyLevel == 3) ? 12 : 10;
    int maxQuotient = (numDifficultyLevel == 1) ? 10 :
        (numDifficultyLevel == 3) ? 15 : 12;

    int b = randRange(2, maxDiv);
    int c = randRange(1, maxQuotient);
    int a = b * c;   // ensures exact division

    return Question::makeSingle(a, b, QuestionOp::DIVIDE);
}

// Accessors
const Question& QuestionManager::current() const
{
    int safe = std::min(currentIndex, (int)questions.size() - 1);
    return questions[safe];
}

bool QuestionManager::submit(const std::vector<int>& collectedAnswers)
{
    if (questions[currentIndex].validate(collectedAnswers))
    {
        answered++;
        currentIndex++;

        // Endless: append a new question when approaching the end
        if (endless && currentIndex >= (int)questions.size() - 1)
            appendNextQuestion();

        return true;
    }
    else
    {
        reshuffleCurrentQuestion();
        return false;
    }
}

bool QuestionManager::isComplete() const
{
    if (endless) return false;
    return currentIndex >= (int)questions.size();
}

int QuestionManager::questionsAnswered() const { return answered; }
int QuestionManager::totalQuestions() const { return endless ? 0 : (int)questions.size(); }
SessionType QuestionManager::getSessionType() const { return sessionType; }
SessionLength QuestionManager::getSessionLength() const { return sessionLength; }
BlankDifficulty QuestionManager::getBlankDifficulty() const { return blankDifficulty; }

int QuestionManager::currentBlankCount() const
{
    if (currentIndex >= (int)questions.size()) return 0;
    return questions[currentIndex].blankCount();
}

// Append one more question (endless mode)
void QuestionManager::appendNextQuestion()
{
    bool forceDouble = (blankDifficulty == BlankDifficulty::HARD);
    questions.push_back(makeQuestion(forceDouble));
}

// Reshuffle: for single-blank questions, pick a new blank slot.
// Double-blank stays fixed since there's only one possible arrangement.
void QuestionManager::reshuffleCurrentQuestion()
{
    Question& q = questions[currentIndex];
    if (q.blankCount() == 1)
    {
        int slot = std::rand() % 3;
        q.blankA = (slot == 0);
        q.blankB = (slot == 1);
        q.blankC = (slot == 2);
    }
}