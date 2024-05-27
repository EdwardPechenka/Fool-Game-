#pragma once
#include <vector>
#include "Card.h"

using namespace std;


class Player
{
public:
	Player(int x, int y);
	~Player();

	vector <Card*> & getCards();

	int x;
	int y;


private:
	vector <Card*> cards;
};

Player::Player(int x, int y)
{
	this->x = x;
	this->y = y;
}

Player::~Player()
{
}

vector <Card*> & Player::getCards() {
	return this->cards;
}
