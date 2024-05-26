#include <iostream>
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Button.h"
#define _BOARD_MIN_X 330
#define _BOARD_MAX_X 930 
#define _BOARD_MAX_Y 470
#define _BOARD_MIN_Y 275
#define _CARD_HEIGHT 110
#define _CARD_WIDTH 70
#define _DECK_LENGHT 52

using namespace sf;
using namespace std;

Card* getCardFromDeck(Card** deck) {
    Card* card = nullptr;
    do {
        card = deck[rand() % 52];
    } while (card->getIsGiven());
    card->setIsGiven(true);
    return card;
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

    Card* player1Cards[6];
    Card* player2Cards[6];

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
        player1Cards[i] = card;

        card = nullptr;
        do {
            card = deck[rand() % 52];
        } while (card->getIsGiven());

        card->setIsGiven(true);
        player2Cards[i] = card;

        player1Cards[i]->getSprite()->setPosition(350 + i * 100, 500);
        player2Cards[i]->getSprite()->setPosition(350 + i * 100, 150);
    }

    bool isDraging = false;
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
                    if (buttonNext.isClicked(Mouse::getPosition(window))) {
                        for (int i = 0; i < 6; i++) {
                            if (player1Cards[i]->getIsInAction()) {
                                player1Cards[i] = getCardFromDeck(deck);
                                player1Cards[i]->getSprite()->setPosition(350 + i * 100, 500);
                            }
                            if (player2Cards[i]->getIsInAction()) {
                                player2Cards[i] = getCardFromDeck(deck);
                                player2Cards[i]->getSprite()->setPosition(350 + i * 100, 150);
                            }
                        }
                        break;
                    }

                    int indexPlayer1 = -1;

                    for (int i = 0; i < 6; ++i)
                    {
                        if (player1Cards[i]->getSprite()->getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            indexPlayer1 = i;
                            break;
                        }
                    }

                    if (indexPlayer1 != -1) {
                        isDraging = true;
                        draggedCard = player1Cards[indexPlayer1];
                        offset = draggedCard->getSprite()->getPosition() - window.mapPixelToCoords(mousePos);
                        inittop = draggedCard->getSprite()->getPosition().y;
                        initleft = draggedCard->getSprite()->getPosition().x;
                    }

                }
            }
            else if (draggedCard != nullptr && event.type == Event::MouseButtonReleased)
            {
                isDraging = false;
                if (event.mouseButton.button == Mouse::Left) {
                   int top = draggedCard->getSprite()->getPosition().y;
                   int left = draggedCard->getSprite()->getPosition().x;
                   if (top > _BOARD_MAX_Y - _CARD_HEIGHT || top < _BOARD_MIN_Y || left < _BOARD_MIN_X || left > _BOARD_MAX_X - _CARD_WIDTH) {
                       draggedCard->getSprite()->setPosition(initleft, inittop);
                       break;
                   }
                   for (int i = 0; i < 6; i++) {
                       if (!player2Cards[i]->getIsInAction() && draggedCard->getSuit() == player2Cards[i]->getSuit() && draggedCard->getRank() < player2Cards[i]->getRank()) {
                           player2Cards[i]->getSprite()->setPosition(draggedCard->getSprite()->getPosition().x + _CARD_WIDTH / 6, draggedCard->getSprite()->getPosition().y + _CARD_HEIGHT / 6);
                           draggedCard->setIsInAction(true);
                           player2Cards[i]->setIsInAction(true);
                           break;
                       }
                   }
                 
                }
                draggedCard = nullptr;
            }

        }

        if (isDraging && draggedCard)
        {
            draggedCard->getSprite()->setPosition(window.mapPixelToCoords(mousePos) + offset);
        }

        window.clear();
        window.draw(stable);
        window.draw(usedDeck);
        window.draw(aviableDeck);
        buttonNext.draw(window);
        buttonTake.draw(window);

        for (int i = 0; i < 6; ++i) {
            window.draw(*player1Cards[i]->getSprite());
            window.draw(*player2Cards[i]->getSprite());
        }

        window.display();
    }

    return 0;
}