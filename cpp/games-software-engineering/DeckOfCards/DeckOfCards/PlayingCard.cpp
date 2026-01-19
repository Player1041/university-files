#include "PlayingCard.h"

PlayingCard::PlayingCard(const std::string& suit, const std::string& value) : m_faceValue(value), m_suit(suit) {}

std::string PlayingCard::GetFaceValue() const {
	return m_faceValue;
}

std::string PlayingCard::GetSuit() const {
	return m_suit;
}