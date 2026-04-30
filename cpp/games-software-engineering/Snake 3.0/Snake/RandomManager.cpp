#include "RandomManager.h"

// Constructor
RandomManager::RandomManager(int questionCount)
    : activeMathsIdx(-1), activeEnglishIdx(-1),
    answered(0), total(questionCount),
    endless(questionCount == 0),
    rng(std::random_device{}())
{
    // Create one of each maths type - endless so they never run dry
    qmTimesTable = new QuestionManager(SessionType::TIMES_TABLE,
        SessionLength::ENDLESS,
        BlankDifficulty::MEDIUM);
    qmArithmetic = new QuestionManager(SessionType::ARITHMETIC,
        SessionLength::ENDLESS,
        BlankDifficulty::MEDIUM);
    qmDivision = new QuestionManager(SessionType::DIVISION,
        SessionLength::ENDLESS,
        BlankDifficulty::MEDIUM);

    // Create one of each english type - endless, medium difficulty
    emAlphabet = new EnglishManager(EnglishType::ALPHABET,
        EnglishDifficulty::MEDIUM, 0);
    emSpelling = new EnglishManager(EnglishType::SPELLING,
        EnglishDifficulty::MEDIUM, 0);

    // Register managers in the flat lists for random selection
    mathsManagers = { qmTimesTable, qmArithmetic, qmDivision };
    englishManagers = { emAlphabet, emSpelling };

    // Pick the first question
    pickNext();
}

RandomManager::~RandomManager()
{
    delete qmTimesTable;
    delete qmArithmetic;
    delete qmDivision;
    delete emAlphabet;
    delete emSpelling;
}

// Randomly choose the next active manager.
// Gives equal weight to all 5 types (3 maths + 2 english).
void RandomManager::pickNext()
{
    int total_types = (int)mathsManagers.size() + (int)englishManagers.size();
    int pick = std::uniform_int_distribution<int>(0, total_types - 1)(rng);

    if (pick < (int)mathsManagers.size())
    {
        activeMathsIdx = pick;
        activeEnglishIdx = -1;
    }
    else
    {
        activeMathsIdx = -1;
        activeEnglishIdx = pick - (int)mathsManagers.size();
    }
}

bool RandomManager::activeIsMaths() const
{
    return activeMathsIdx >= 0;
}

QuestionManager* RandomManager::mathsManager() const
{
    if (activeMathsIdx >= 0)
        return mathsManagers[activeMathsIdx];
    return nullptr;
}

EnglishManager* RandomManager::englishManager() const
{
    if (activeEnglishIdx >= 0)
        return englishManagers[activeEnglishIdx];
    return nullptr;
}

// Advance to the next question after a correct answer.
void RandomManager::advance()
{
    answered++;
    pickNext();
}

bool RandomManager::isComplete() const
{
    if (endless) return false;
    return answered >= total;
}

int RandomManager::questionsAnswered() const { return answered; }
int RandomManager::totalQuestions() const { return endless ? 0 : total; }