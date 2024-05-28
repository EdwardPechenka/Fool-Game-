#pragma once

#include "Card.h"
#include "Array.h"

using namespace std;


class Player
{
public:                    
	//Coordinate user
	Player(int x, int y) : x(x), y(y) {}
	~Player() {}

	Array<Card*>& getCards() {
		return cards;
	}

	int x;
	int y;


private:
	Array<Card*> cards;
};

