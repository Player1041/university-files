#include "HighScores.h"
#include <sstream>

HighScores::HighScores() {}

void HighScores::load(const std::string& path)
{
    filePath = path;
    tables.clear();

    std::ifstream f(path);
    if (!f.is_open()) return;

    std::string line;
    while (std::getline(f, line))
    {
        if (line.empty()) continue;

        // Find the last three pipe positions to split: key | name | score | time
        size_t p3 = line.rfind('|'); // rightmost: before time
        if (p3 == std::string::npos) continue;
        size_t p2 = line.rfind('|', p3 - 1); // before score
        if (p2 == std::string::npos) continue;
        size_t p1 = line.rfind('|', p2 - 1); // before playerName
        if (p1 == std::string::npos) continue;

        std::string tableKey = line.substr(0, p1);
        std::string playerName = line.substr(p1 + 1, p2 - p1 - 1);
        int score = std::stoi(line.substr(p2 + 1, p3 - p2 - 1));
        float time = std::stof(line.substr(p3 + 1));

        tables[tableKey].push_back({ playerName, score, time });
    }

    // Sort each table after loading
    for (auto& kv : tables)
        sortAndTrim(kv.second);
}

// Save all tables to file.
// Format: tableKey|playerName|score|timeSeconds
void HighScores::save() const
{
    std::ofstream f(filePath);
    for (const auto& kv : tables)
        for (const auto& e : kv.second)
            f << kv.first << "|" << e.playerName << "|" << e.score << "|" << e.timeSeconds << "\n";
}

void HighScores::add(const std::string& tableKey, const ScoreEntry& entry)
{
    tables[tableKey].push_back(entry);
    sortAndTrim(tables[tableKey]);
    save();
}

const std::vector<ScoreEntry>& HighScores::getEntries(const std::string& tableKey) const
{
    auto it = tables.find(tableKey);
    if (it == tables.end()) return emptyTable;
    return it->second;
}

void HighScores::sortAndTrim(std::vector<ScoreEntry>& table) const
{
    std::sort(table.begin(), table.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        if (a.score != b.score) return a.score > b.score;
        if (a.timeSeconds > 0.f && b.timeSeconds > 0.f)
            return a.timeSeconds < b.timeSeconds;
        return false;
        });

    if ((int)table.size() > MAX_PER_TABLE)
        table.resize(MAX_PER_TABLE);
}