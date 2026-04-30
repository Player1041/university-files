#pragma once
#include "QuestionManager.h"
#include "EnglishManager.h"
#include <vector>
#include <random>

class RandomManager
{
public:
    // questionCount: 0 = endless, otherwise fixed round count
    RandomManager(int questionCount);
    ~RandomManager();

    // True if the current question comes from a QuestionManager (maths)
    // False means it comes from an EnglishManager
    bool activeIsMaths() const;

    // Typed accessors
    QuestionManager* mathsManager() const;
    EnglishManager* englishManager() const;

    // Advance to the next randomly selected question.
    // Called by Game after a correct answer so it can respawn tiles.
    void advance();

    bool isComplete() const;
    int questionsAnswered() const;
    int totalQuestions() const;   // 0 = endless

private:
    // One manager per implemented type
    QuestionManager* qmTimesTable;
    QuestionManager* qmArithmetic;
    QuestionManager* qmDivision;
    EnglishManager* emAlphabet;
    EnglishManager* emSpelling;

    // Which manager is currently active (index into the arrays below)
    int activeMathsIdx;    // -1 if current is english
    int activeEnglishIdx;  // -1 if current is maths

    int answered;
    int total; // 0 = endless
    bool endless;

    std::mt19937 rng;

    // Flat list of all managers for random selection
    std::vector<QuestionManager*> mathsManagers;
    std::vector<EnglishManager*>  englishManagers;

    // Pick the next active manager at random
    void pickNext();
};