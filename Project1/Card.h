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
    Card(Sprite* sprite, Suit suit, Rank rank) : sprite(sprite), suit(suit), rank(rank) {}
    ~Card() {}

    Card() {}

    Sprite* getSprite() const {
        return sprite;
    }

    Suit getSuit() const {
        return suit;
    }
    Rank getRank() const {
        return rank;
    }

    bool getIsInAction() const {
        return isInAction;
    }
    void setIsInAction(bool value) {
        isInAction = value;
    }

    bool getIsHitted() const {
        return isHitted;
    }
    void setIsHitted(bool value) {
        isHitted = value;
    }

    bool operator == (const Card& other) const {
        return suit == other.suit && rank == other.rank;
    }

private:
    Sprite* sprite;
    Suit suit;
    Rank rank;
    bool isInAction = false;
    bool isHitted = false;
};

