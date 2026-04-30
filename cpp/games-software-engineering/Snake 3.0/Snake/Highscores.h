#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

// One entry in a score table
struct ScoreEntry
{
    std::string playerName;   // up to 8 characters
    int score;        // questions correct, or classic food eaten
    float timeSeconds;  // completion time in seconds (0 = not timed / classic)
};

// All displayable game type names, in left-right order on the high scores screen.
// Classic has no length variants so it only appears under "Classic" in the length axis.
namespace ScoreMode
{
    // Game type labels (left/right axis on the high scores screen)
    static const std::vector<std::string> TYPES = {
        "Classic", "Times Tables", "Arithmetic", "Division", "Alphabet", "Spelling", "Random"
    };

    // Length labels (up/down axis on the high scores screen)
    // Classic only ever uses index 0 (the "Classic" entry below)
    static const std::vector<std::string> LENGTHS = {
        "Endless", "10 Rounds", "15 Rounds", "20 Rounds", "25 Rounds"
    };

    // Build the compound key used to look up a specific table.
    // e.g. makeKey("Times Tables", "10 Rounds") -> "Times Tables|10 Rounds"
    // Classic always uses makeKey("Classic", "Classic").
    inline std::string makeKey(const std::string& type, const std::string& length)
    {
        return type + "|" + length;
    }

    // The special length label used for Classic (no rounds concept)
    static const std::string CLASSIC_LENGTH = "Classic";
}

class HighScores
{
public:
    static const int MAX_PER_TABLE = 5;

    HighScores();
    void load(const std::string& path);
    void save() const;

    void add(const std::string& tableKey, const ScoreEntry& entry);

    const std::vector<ScoreEntry>& getEntries(const std::string& tableKey) const;

private:
    std::map<std::string, std::vector<ScoreEntry>> tables;
    std::vector<ScoreEntry> emptyTable;
    std::string filePath;

    void sortAndTrim(std::vector<ScoreEntry>& table) const;
};