#include "Card.h"
#include <SFML/Graphics.hpp>
using namespace sf;

Card::Card(Sprite* sprite, Suit suit, Rank rank)
{
	this->sprite = sprite;
	this->rank = rank;
	this->suit = suit;
}

Card::~Card()
{
}

Card::Card()
{
}

Sprite* Card::getSprite() const
{
	return this->sprite;
}

Suit Card::getSuit() const
{
	return this->suit;
}

Rank Card::getRank() const
{
	return this->rank;
}

bool Card::getIsGiven() const
{
	return this->isGiven;
}

void Card::setIsGiven(bool value)
{
	this->isGiven = value;
}

bool Card::getIsInAction() const
{
	return this->isInAction;
}

void Card::setIsInAction(bool value)
{
	this->isInAction = value;
}

bool Card::getIsHitted() const 
{
	return this->isHitted;
}

void Card::setIsHitted(bool value) 
{
	this->isHitted = value;
}