// DeckOfCards.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Deck.h"
// todo: shuffle
// todo: deal

int main()
{
    Deck deck;
    std::cout << deck;
    std::cout << "Hello World!\n";
    for (int i = 0; i < 52; i++) {
		deck.Deal();
    }
}

