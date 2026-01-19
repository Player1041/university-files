#include "Deck.h"
#include "string.h"
Deck::Deck() {
	Build();
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
