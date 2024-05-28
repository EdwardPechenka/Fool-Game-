#include <iostream>
#include "SFML/Graphics.hpp"
#include "Card.h"
#include "Player.h"
#include "Array.h"
#include "Button.h"

#define _BOARD_MIN_X 330
#define _BOARD_MAX_X 930
#define _BOARD_MIN_Y 275
#define _BOARD_MAX_Y 470
#define _CARD_HEIGHT 110
#define _CARD_WIDTH 70
#define _DECK_LENGTH 52
#define _CARD_OFFSET_X 30
#define _OVERLAP_OFFSET _CARD_WIDTH / 4 

using namespace std;
using namespace sf;

void takeCardsFromDeck(Player& player, Array<Card*>& deck) {
	if (deck.get_size() == 0) {
		return;
	}

	if (player.getCards().get_size() >= 6) {
		return;
	}
	
	Card* card;
	int size = 6 - player.getCards().get_size();

	for (int i = 0; i < size; i++) {
		card = nullptr;

		int index = rand() % deck.get_size();
		card = deck[index];
		deck.erase(index);

		player.getCards().push_back(card);
	}
}

void setInitPosition(Player& player) {
	for (int i = 0; i < player.getCards().get_size(); i++) {
		player.getCards()[i]->getSprite()->setPosition(player.x + i * (_CARD_WIDTH + _CARD_OFFSET_X) - ((player.getCards().get_size() - 6) * 50), player.y);
	}
}

bool rankExistOnBoard(Array<Card*>& cardsOnBoard, Rank rank) {
	for (int i = 0; i < cardsOnBoard.get_size(); i++) {
		if (cardsOnBoard[i]->getRank() == rank) {
			return true;
		}
	}
	return false;
}

void findAvailableCards(Player& player, Array<Card*>& cardsOnBoard, Array<Card*>& availableCards) {
	for (int i = 0; i < player.getCards().get_size(); i++) {
		if (rankExistOnBoard(cardsOnBoard, player.getCards()[i]->getRank())) {
			availableCards.push_back(player.getCards()[i]);
		}
	}
}

Card* findMinCard(Array<Card*>& cards) {
	if (cards.get_size() == 0) {
		return nullptr;
	}
	Card* minCard = cards[0];
	for (int i = 0; i < cards.get_size(); i++) {
		if (minCard->getRank() > cards[i]->getRank()) {
			minCard = cards[i];
		}
	}
	return minCard;
}

Card* findCorrectCard(Player& player, Array<Card*>& cardsOnBoard) {
	if (cardsOnBoard.get_size() > 0) {
		Array<Card*> availableCards;
		findAvailableCards(player, cardsOnBoard, availableCards);
		return findMinCard(availableCards);
	}
	return findMinCard(player.getCards());
}

void takeCards(Player& player, Array<Card*>& cardsOnBoard) {
	for (int i = 0; i < cardsOnBoard.get_size(); i++) {
		cardsOnBoard[i]->setIsInAction(false);
		cardsOnBoard[i]->setIsHitted(false);
		player.getCards().push_back(cardsOnBoard[i]);
	}
	cardsOnBoard.clear();
}

//Card position on board, when even (парне) numbers attacker, odd (непарне) number defender
void setCardOnBoardPosition(Array<Card*>& cardsOnBoard) {
	for (int i = 0, j = 0; i < cardsOnBoard.get_size(); i++) {
		int x = _BOARD_MIN_X + j * (_CARD_WIDTH + _CARD_OFFSET_X) + (i % 2 ? _OVERLAP_OFFSET : 0);
		int y = _BOARD_MIN_Y + (i % 2 ? _OVERLAP_OFFSET : 0);
		cardsOnBoard[i]->getSprite()->setPosition(x, y);
		if (i % 2 != 0) {
			j++;
		}
	}
}

bool addCardOnBoard(Player& player, Card* card, Array<Card*>& cardsOnBoard) {
	if (card == nullptr) {
		return false;
	}

	for (int i = 0; i < player.getCards().get_size(); i++) {
		if (player.getCards()[i] == card) {
			player.getCards().erase(i);
			break;
		}
	}

	cardsOnBoard.push_back(card);
	setCardOnBoardPosition(cardsOnBoard);
}

bool defend(Player& player, Array<Card*>& cardsOnBoard) {
	Card* card = cardsOnBoard[cardsOnBoard.get_size() - 1];
	for (int i = 0; i < player.getCards().get_size(); i++) {
		if (
			!player.getCards()[i]->getIsInAction() &&
			card->getRank() < player.getCards()[i]->getRank() &&
			card->getSuit() == player.getCards()[i]->getSuit()
			) {
			card->setIsHitted(true);
			player.getCards()[i]->setIsInAction(true);
			addCardOnBoard(player, player.getCards()[i], cardsOnBoard);
			return true;
		}
	}
	return false;
}

int main() {

	srand(time(0));

	RenderWindow window(VideoMode(1280, 720), "Fool Game");

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

	Button buttonNext(1000, 630, 200, 60, "next");
	Button buttonTake(70, 630, 200, 60, "take");

	bool isBotMove = false;
	int inittop = NULL, initleft = NULL;
	Card* draggedCard = nullptr;
	Vector2f offset;
	
	//Coordinate user
	Player user(350, 500);
	Player bot(350, 150);

	Array<Card*> deck;
	Array<Card*> cardsOnBoard;

	//Card deck creation
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < _DECK_LENGTH / 4; i++) {
            deck.push_back(new Card(new Sprite(), (Suit)j, (Rank)i));
            deck[i + j * 13]->getSprite()->setTexture(t1);
            deck[i + j * 13]->getSprite()->setTextureRect(IntRect(_CARD_WIDTH * i, _CARD_HEIGHT * j, _CARD_WIDTH, _CARD_HEIGHT));
        }
    }

	takeCardsFromDeck(user, deck);
	takeCardsFromDeck(bot, deck);

	setInitPosition(user);
	setInitPosition(bot);

	while (window.isOpen()) {
		Vector2i mousePos = Mouse::getPosition(window);
		Event event;
		
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			
			//ButtonPressed
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				if (isBotMove && buttonTake.isClicked(Mouse::getPosition(window))) {
					takeCards(user, cardsOnBoard);
					takeCardsFromDeck(bot, deck);

					setInitPosition(user);
					setInitPosition(bot);

					addCardOnBoard(bot, findCorrectCard(bot, cardsOnBoard), cardsOnBoard);
				}
				
				if (!isBotMove && buttonNext.isClicked(Mouse::getPosition(window))) {
					takeCardsFromDeck(user, deck);
					takeCardsFromDeck(bot, deck);

					setInitPosition(user);
					setInitPosition(bot);

					cardsOnBoard.clear();

					isBotMove = true;

					addCardOnBoard(bot, findCorrectCard(bot, cardsOnBoard), cardsOnBoard);

					break;
				}
					
				if (mousePos.y > 500) {
					for (int i = 0; i < user.getCards().get_size(); ++i)
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
			
			//ButtonReleased
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (draggedCard == nullptr) {
					break;
				}

				int top = draggedCard->getSprite()->getPosition().y;
				int left = draggedCard->getSprite()->getPosition().x;

				if (top > _BOARD_MAX_Y - _CARD_HEIGHT || top < _BOARD_MIN_Y || left < _BOARD_MIN_X || left > _BOARD_MAX_X - _CARD_WIDTH) {
					draggedCard->getSprite()->setPosition(initleft, inittop);
					draggedCard = nullptr;
					break;
				}

				draggedCard->setIsInAction(true);

				if (isBotMove) {
					if (
						cardsOnBoard[cardsOnBoard.get_size() - 1]->getSuit() == draggedCard->getSuit() &&
						draggedCard->getRank() > cardsOnBoard[cardsOnBoard.get_size() - 1]->getRank()
						) {
						cardsOnBoard[cardsOnBoard.get_size() - 1]->setIsHitted(true);

						addCardOnBoard(user, draggedCard, cardsOnBoard);

						if (!addCardOnBoard(bot, findCorrectCard(bot, cardsOnBoard), cardsOnBoard)) {
							isBotMove = false;
							takeCardsFromDeck(bot, deck);
							takeCardsFromDeck(user, deck);

							cardsOnBoard.clear();
						}
						setInitPosition(user);
						setInitPosition(bot);
					}
				}
				else {
					if (cardsOnBoard.get_size() > 0 && !rankExistOnBoard(cardsOnBoard, draggedCard->getRank())) {
						draggedCard->getSprite()->setPosition(initleft, inittop);
						draggedCard = nullptr;
						break;
					}

					addCardOnBoard(user, draggedCard, cardsOnBoard);

					if (!defend(bot, cardsOnBoard)) {
						takeCards(bot, cardsOnBoard);
						takeCardsFromDeck(user, deck);
					}
					setInitPosition(user);
					setInitPosition(bot);
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

		for (int i = 0; i < user.getCards().get_size(); i++) {
			window.draw(*user.getCards()[i]->getSprite());
		}

		for (int i = 0; i < bot.getCards().get_size(); i++) {
			window.draw(*bot.getCards()[i]->getSprite());
		}

		for (int i = 0; i < cardsOnBoard.get_size(); i++) {
			window.draw(*cardsOnBoard[i]->getSprite());
		}
		window.display();

	}
	cin.get();
}