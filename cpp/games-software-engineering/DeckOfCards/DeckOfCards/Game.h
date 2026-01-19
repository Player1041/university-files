#pragma once
#include "Deck.h"
#include "Hand.h"

class Game
{
private:
	Hand m_hands;
	Deck m_deck;
public:
	Game();
	void Play();
};

