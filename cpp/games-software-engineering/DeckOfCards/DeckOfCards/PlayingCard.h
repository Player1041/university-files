#pragma once
#include <ostream>
#include <string.h>
class PlayingCard {
private:
    std::string m_faceValue;
    std::string m_suit;
public:
    PlayingCard(const std::string& suit, const std::string& value);
    std::string GetFaceValue() const;
    std::string GetSuit() const;
};

// Insertion operator for PlayingCard
inline std::ostream& operator<<(std::ostream& os, const PlayingCard& card)
{
    os << card.GetFaceValue() << " of " << card.GetSuit();
    return os;
}