#include "Deck.h"
#include "string.h"
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <random>
#include <ctime>

Deck::Deck() {
	srand(time(NULL));
	Build();
	//Shuffle();
}
	

void Deck::Build() {
	cards.clear(); // empty the vector of any cards

	std::string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
	std::string values[] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };

	for (const auto& suit : suits) {
		for (const auto& value : values) {
			cards.emplace_back(suit, value);
		};
	};
};

void Deck::Shuffle() {
	for (int card = 52 - 1; card > 0; card--) {
		int index = rand() % (card + 1);
		std::swap(cards[card], cards[index]);
	};
}

PlayingCard Deck::Deal() {
	std::cout << "Dealing card: " << cards[m_nextCard] << std::endl;
	if (m_nextCard < 51) {
		m_nextCard++;
		std::cout << "Cards left in deck: " << (52 - m_nextCard) << "\n" << std::endl;
	}
	else {
		std::cout << "Cards left in deck: 0" << "\n" << std::endl;
	}
	
	return cards[m_nextCard];
}
