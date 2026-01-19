#pragma once
#include <vector>
#include <ostream>
#include "PlayingCard.h"
class Hand
{
private:
	std::vector<PlayingCard> m_cards;
public:
	void Add(PlayingCard card);
	int Value();
};

