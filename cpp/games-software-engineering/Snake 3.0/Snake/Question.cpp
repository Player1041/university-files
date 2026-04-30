#include "Question.h"
#include <cstdlib>
#include <algorithm>
#include <random>
#include <sstream>
#include <cmath>

// Shared RNG
static std::mt19937& rng()
{
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

// Operand symbol helper
std::string Question::opSymbol() const
{
    switch (op)
    {
    case QuestionOp::MULTIPLY: return "x";
    case QuestionOp::ADD: return "+";
    case QuestionOp::SUBTRACT: return "-";
    case QuestionOp::DIVIDE: return "/";
    default: return "?";
    }
}

// Single-blank factory
// Computes c from a OP b, then randomly hides one slot.
Question Question::makeSingle(int aVal, int bVal, QuestionOp operation)
{
    Question q;
    q.a = aVal;
    q.b = bVal;
    q.op = operation;
    q.blankA = false;
    q.blankB = false;
    q.blankC = false;

    switch (operation)
    {
    case QuestionOp::MULTIPLY: q.c = aVal * bVal; break;
    case QuestionOp::ADD: q.c = aVal + bVal; break;
    case QuestionOp::SUBTRACT: q.c = aVal - bVal; break;
    case QuestionOp::DIVIDE:
        // Caller must ensure bVal divides aVal exactly
        q.c = aVal / bVal;
        break;
    }

    int slot = std::rand() % 3;
    if (slot == 0) q.blankA = true;
    else if (slot == 1) q.blankB = true;
    else q.blankC = true;

    return q;
}

// Double-blank factory (MULTIPLY only)
Question Question::makeDouble(int aVal, int bVal)
{
    Question q;
    q.a = aVal;
    q.b = bVal;
    q.c = aVal * bVal;
    q.op = QuestionOp::MULTIPLY;
    q.blankA = true;
    q.blankB = true;
    q.blankC = false;
    return q;
}

// HUD display string
std::string Question::getDisplayString() const
{
    auto slot = [](bool blank, int val) -> std::string {
        return blank ? "__" : std::to_string(val);
        };
    return slot(blankA, a) + " " + opSymbol() + " " + slot(blankB, b) + " = " + slot(blankC, c);
}

int Question::blankCount() const
{
    return (blankA ? 1 : 0) + (blankB ? 1 : 0) + (blankC ? 1 : 0);
}

// Validate
bool Question::validate(const std::vector<int>& answers) const
{
    if (blankCount() == 1)
    {
        if (answers.empty()) return false;
        int got = answers[0];
        if (blankA) return got == a;
        if (blankB) return got == b;
        if (blankC) return got == c;
    }

    if (blankCount() == 2)
    {
        // Double-blank only exists for MULTIPLY
        if (answers.size() < 2) return false;
        int x = answers[0], y = answers[1];
        if (x * y != c) return false;
        return (x == a && y == b) || (x == b && y == a);
    }

    return false;
}

// Generate grid numbers - operation-aware distractors
std::vector<int> Question::generateGridNumbers(int gridSize) const
{
    std::vector<int> numbers;

    // Correct answers first
    if (blankA) numbers.push_back(a);
    if (blankB) numbers.push_back(b);
    if (blankC) numbers.push_back(c);

    std::vector<int> pool;

    switch (op)
    {
    case QuestionOp::MULTIPLY:
        // Nearby multiples of the same table
        for (int d = -3; d <= 3; d++) { int v = a * (b + d); if (v > 0) pool.push_back(v); }
        for (int d = -3; d <= 3; d++) { int v = (a + d) * b; if (v > 0) pool.push_back(v); }
        // Small factors of c
        if (c > 1) for (int f = 2; f <= c / 2; f++) if (c % f == 0) pool.push_back(f);
        break;

    case QuestionOp::ADD:
        // Nearby sums
        for (int d = -4; d <= 4; d++) { int v = a + (b + d); if (v >= 0) pool.push_back(v); }
        for (int d = -4; d <= 4; d++) { int v = (a + d) + b; if (v >= 0) pool.push_back(v); }
        // Nearby operands
        for (int d = -3; d <= 3; d++) { if (a + d > 0) pool.push_back(a + d); }
        for (int d = -3; d <= 3; d++) { if (b + d > 0) pool.push_back(b + d); }
        break;

    case QuestionOp::SUBTRACT:
        // Nearby results
        for (int d = -4; d <= 4; d++) { int v = c + d; if (v >= 0) pool.push_back(v); }
        // Nearby operands
        for (int d = -3; d <= 3; d++) { if (a + d > 0) pool.push_back(a + d); }
        for (int d = -3; d <= 3; d++) { if (b + d > 0) pool.push_back(b + d); }
        break;

    case QuestionOp::DIVIDE:
        // Other quotients of a by nearby divisors
        for (int d = 1; d <= a; d++) if (a % d == 0) pool.push_back(a / d);
        // Nearby multiples that would give similar answers
        for (int d = -3; d <= 3; d++) { int v = c + d; if (v > 0) pool.push_back(v); }
        for (int d = -3; d <= 3; d++) { int v = b + d; if (v > 0) pool.push_back(v); }
        break;
    }

    // Pad with a range of nearby values
    int lo = std::max(0, std::min({ a, b, c }) - 5);
    int hi = std::max({ a, b, c }) + 8;
    for (int v = lo; v <= hi; v++) pool.push_back(v);

    // Shuffle and pick non-duplicate distractors up to gridSize
    std::shuffle(pool.begin(), pool.end(), rng());
    for (int candidate : pool)
    {
        if ((int)numbers.size() >= gridSize) break;
        if (std::find(numbers.begin(), numbers.end(), candidate) == numbers.end())
            numbers.push_back(candidate);
    }

    std::shuffle(numbers.begin(), numbers.end(), rng());
    return numbers;
}