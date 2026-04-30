#pragma once
#include <vector>
#include <string>

// The mathematical operation for this question
enum class QuestionOp { MULTIPLY, ADD, SUBTRACT, DIVIDE };

// A single question of the form: a OP b = c
// One slot is blank (the player must collect the answer for it)
// For times tables after question 10, two blanks are used (a and b)

struct Question
{
    int a;
    int b;
    int c;       // always the correct result of a OP b
    QuestionOp op;

    bool blankA;
    bool blankB;
    bool blankC;

    // Single-blank: randomly pick which slot is hidden
    static Question makeSingle(int aVal, int bVal, QuestionOp operation);

    // Double-blank: a and b hidden, c shown (only valid for MULTIPLY)
    static Question makeDouble(int aVal, int bVal);

    // HUD display string e.g. "4 x __ = 20" / "__ + 3 = 7"
    std::string getDisplayString() const;

    int blankCount() const;

    // Validate collected answers (left-to-right blank order).
    // For single-blank: answers[0] must equal the hidden value.
    // For double-blank (MULTIPLY only): answers[0]*answers[1] == c, pair == {a,b}.
    bool validate(const std::vector<int>& answers) const;

    // Generate numbers to scatter on the grid (correct answer always included).
    std::vector<int> generateGridNumbers(int gridSize) const;

private:
    // Returns the op symbol as a string
    std::string opSymbol() const;
};