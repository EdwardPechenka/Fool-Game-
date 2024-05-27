#include <iostream>
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Button.h"
#include "Player.h"
#define _BOARD_MIN_X 330
#define _BOARD_MAX_X 930 
#define _BOARD_MAX_Y 470
#define _BOARD_MIN_Y 275
#define _CARD_HEIGHT 110
#define _CARD_WIDTH 70
#define _DECK_LENGHT 52

using namespace sf;
using namespace std;


int getCountofHittedCards(vector<Card*>& vector) {
    int counter = 0;
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i]->getIsHitted()) {
            counter++;
        }
    }
    return counter;
}

Card* getCardFromDeck(Card** deck) {
    Card* card = nullptr;
    do {
        card = deck[rand() % 52];
    } while (card->getIsGiven());
    card->setIsGiven(true);
    return card;
}

int getCountofFreeCards(vector<Card*>& vector) {
    int counter = 0;
    for (int i = 0; i < vector.size(); i++) {
        if (!vector[i]->getIsInAction()) {
            counter++;
        }
    }
    return counter;
}

void takeCards(Player & attacker, Player & defender, Card** deck) {
    for (int i = 0; i < attacker.getCards().size(); i++) {
        if (attacker.getCards()[i]->getIsInAction()) {
            defender.getCards().push_back(attacker.getCards()[i]);
            attacker.getCards()[i] = getCardFromDeck(deck);
            attacker.getCards()[i]->getSprite()->setPosition(attacker.x + i * 100, attacker.y);

        }
    }
    for (int i = 0; i < defender.getCards().size(); i++) {
        defender.getCards()[i]->getSprite()->setPosition(defender.x - ((defender.getCards().size() - 6) * 50) + i * 100, defender.y);
        defender.getCards()[i]->setIsInAction(false);
    }
}

void nextCards(vector<Card*>& attackerCards, vector<Card*>& defenderCards, Card** deck) {
    if (getCountofFreeCards(attackerCards) < 6) {
        for (int i = 0; i < attackerCards.size(); i++) {
            if (attackerCards[i]->getIsInAction() ) {
                attackerCards[i] = getCardFromDeck(deck);
                attackerCards[i]->getSprite()->setPosition(350 - ((attackerCards.size() - 6) * 50) + i * 100, 500);
            }
        }

    }
    if (getCountofFreeCards(defenderCards) < 6) {
        for (int i = 0; i < defenderCards.size(); i++) {
            if (defenderCards[i]->getIsInAction()) {
                defenderCards[i] = getCardFromDeck(deck);
                defenderCards[i]->getSprite()->setPosition(350 - ((defenderCards.size() - 6) * 50) + i * 100, 150);
            }
        }
    }
}

void botDefend(Player & bot, Player & user, Card* attackerCard, Card** deck) {

    bool isHitted = false;

    for (int i = 0; i < bot.getCards().size(); i++) {
        if (!bot.getCards()[i]->getIsInAction() && attackerCard->getSuit() == bot.getCards()[i]->getSuit() && attackerCard->getRank() < bot.getCards()[i]->getRank()) {
            bot.getCards()[i]->getSprite()->setPosition(attackerCard->getSprite()->getPosition().x + _CARD_WIDTH / 6, attackerCard->getSprite()->getPosition().y + _CARD_HEIGHT / 6);
            attackerCard->setIsHitted(true);
            bot.getCards()[i]->setIsInAction(true);
            isHitted = true;
            break;
        }
    }
    if (!isHitted) {
        takeCards(user, bot, deck);
    }
}

void botAttack(vector <Card*>& attackerCards , vector <Card*>& defenderCards) {
    Card* minCard = attackerCards[0];
    for (int i = 0; i < attackerCards.size(); i++) {
        if (minCard->getRank() > attackerCards[i]->getRank()) {
            minCard = attackerCards[i];
        }
    }
    minCard->setIsInAction(true);
    minCard->getSprite()->setPosition(330, 275);
}




int main()
{
    RenderWindow window(VideoMode(1280, 720), "Game cards");

    Card** deck = new Card* [_DECK_LENGHT];

    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/cards.png");
    t2.loadFromFile("images/table.png");
    t3.loadFromFile("images/coloda.png");
    t4.loadFromFile("images/coloda.png");

    Sprite s(t1);
    Sprite stable(t2);
    Sprite usedDeck(t3);
    Sprite aviableDeck(t4);

    usedDeck.setPosition(100, 300);
    aviableDeck.setPosition(1100, 300);

    /*vector<Card*> player1Cards;
    vector<Card*> player2Cards;*/

    Player user(350, 150);
    Player bot(350, 500);

    bool isBotMove = false;

    srand(time(0));

    Button buttonNext(1000, 580, 200, 60, "next");
    Button buttonTake(70, 580, 200, 60, "take");


    // Сдесь создание кода колоды карт
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < _DECK_LENGHT / 4; i++) {
            deck[i + j * 13] = new Card(new Sprite(), (Suit)j, (Rank)i);
            deck[i + j * 13]->getSprite()->setTexture(t1);
            deck[i + j * 13]->getSprite()->setTextureRect(IntRect(_CARD_WIDTH * i, _CARD_HEIGHT * j, _CARD_WIDTH, _CARD_HEIGHT));
        }
    }

    //Выдаёт карты

    for (int i = 0; i < 6; ++i) {
        Card* card = nullptr;
        do {
            card = deck[rand() % 52];
        } while (card->getIsGiven());

        card->setIsGiven(true);
        user.getCards().push_back(card);

        card = nullptr;
        do {
            card = deck[rand() % 52];
        } while (card->getIsGiven());

        card->setIsGiven(true);
        bot.getCards().push_back(card);

        user.getCards()[i]->getSprite()->setPosition(350 + i * 100, 500);
        bot.getCards()[i]->getSprite()->setPosition(350 + i * 100, 150);
    }

    int inittop = NULL, initleft = NULL;
    Card* draggedCard = nullptr;
    Vector2f offset;

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event))
        { 
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    if (buttonTake.isClicked(Mouse::getPosition(window))) {
                        
                    }
                    if (buttonNext.isClicked(Mouse::getPosition(window))) {
                        
                        break;
                    }
                    for (int i = 0; i < user.getCards().size(); ++i)
                    {
                        if (user.getCards()[i]->getSprite()->getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            draggedCard = user.getCards()[i];
                            offset = draggedCard->getSprite()->getPosition() - window.mapPixelToCoords(mousePos);
                            inittop = draggedCard->getSprite()->getPosition().y;
                            initleft = draggedCard->getSprite()->getPosition().x;
                            break;
                        }
                    }
                }
            }
            else if (draggedCard != nullptr && event.type == Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == Mouse::Left) {
                    int top = draggedCard->getSprite()->getPosition().y;
                    int left = draggedCard->getSprite()->getPosition().x;
                    if (top > _BOARD_MAX_Y - _CARD_HEIGHT || top < _BOARD_MIN_Y || left < _BOARD_MIN_X || left > _BOARD_MAX_X - _CARD_WIDTH) {
                        draggedCard->getSprite()->setPosition(initleft, inittop);
                        break;
                    }

                    draggedCard->setIsInAction(true);

                    if (isBotMove) {
                        
                    }
                    else {
                        for (int i = 0; i < user.getCards().size(); i++) {
                            if (user.getCards()[i]->getIsInAction() && !user.getCards()[i]->getIsHitted()) {
                                user.getCards()[i]->getSprite()->setPosition(330 + getCountofHittedCards(user.getCards()) * 100, 275);
                                botDefend(bot, user, user.getCards()[i], deck);
                                break;
                            }
                        }
                    }



                 
                }
                draggedCard = nullptr;
            }

        }

        if (draggedCard)
        {
            draggedCard->getSprite()->setPosition(window.mapPixelToCoords(mousePos) + offset);
        }

        window.clear();
        window.draw(stable);
        window.draw(usedDeck);
        window.draw(aviableDeck);
        buttonNext.draw(window);
        buttonTake.draw(window);

        for (int i = 0; i < user.getCards().size(); i++) {
            window.draw(*user.getCards()[i]->getSprite());
        }
        
        for (int i = 0; i < bot.getCards().size(); i++) {
            window.draw(*bot.getCards()[i]->getSprite());
        }

        for (int i = 0; i < user.getCards().size(); ++i) {
            for (int j = 0; j < bot.getCards().size(); j++) {
                if (bot.getCards()[j]->getSprite()->getGlobalBounds().intersects(user.getCards()[i]->getSprite()->getGlobalBounds())) {
                    if (!isBotMove) {
                        window.draw(*user.getCards()[i]->getSprite());
                        window.draw(*bot.getCards()[j]->getSprite());
                    }
                    else {
                        window.draw(*bot.getCards()[j]->getSprite());
                        window.draw(*user.getCards()[i]->getSprite());
                    }
                }
            }


        }

        window.display();
    }

    return 0;
}