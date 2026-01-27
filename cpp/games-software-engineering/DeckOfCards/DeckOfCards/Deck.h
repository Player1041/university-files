#pragma once
#include <vector>
#include <ostream>
#include "PlayingCard.h"

class Deck {
private:
    int m_nextCard = 0;
    std::vector<PlayingCard> cards;

public:
    Deck();
    void Build();
    void Shuffle();
    PlayingCard Deal();

    friend inline std::ostream& operator<<(std::ostream& os, const Deck& deck)
    {
        for (const auto& card : deck.cards)
        {
            os << card << std::endl;
        }
        return os;
    }
};