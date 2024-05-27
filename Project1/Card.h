#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

enum class Suit {
    Spades, Hearts, Diamonds, Clubs
};

enum class Rank {
   Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King,
};

class Card
{
public:
	Card(Sprite* sprite, Suit suit, Rank rank);
	~Card();

	Card();

	Sprite* getSprite() const;

	bool getIsGiven() const;
	void setIsGiven(bool value);

	Suit getSuit() const;
	Rank getRank() const;

	bool getIsInAction() const;
	void setIsInAction(bool value);

	bool getIsHitted() const;
	void setIsHitted(bool value);

private:
	Sprite* sprite;
	Suit suit;
	Rank rank;
	bool isGiven = false;
	bool isInAction = false;
	bool isHitted = false;



};
