#pragma once
#include <vector>
#include "Orders/Orders.h"

class Cards
{

public:
	// constructors
	Cards();
	Cards(const Cards &card);
	Cards &operator=(const Cards &card);

	// card type: 0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
	int type;

	void play(Cards* card, OrdersList* orders);
	void createOrder(Cards *card, OrdersList* orders);
};

class Deck : public Cards
{

public:
	// constructors
	Deck();
	~Deck();
	Deck(const Deck &card);
	Deck &operator=(const Deck &card);

	void createDeck();
	void printDeck();
	Cards *draw();
	void addToDeck(Cards *card);

private:
	// creation of a vector of card pointers, this will be the deck
	std::vector<Cards *> deckOfCards;
	// temporary card for deck drawing
	Cards *tempDrawCard;
};

class Hand : public Cards
{

public:
	// constructors
	Hand();
	~Hand();
	Hand(const Hand &card);
	Hand &operator=(const Hand &card);

	void addToHand(Cards *card);
	void printHand();
	Cards *playCard(Deck *deck);

	vector<Cards*> getCards();

private:
	// creation of a vector of card pointers, this will be the player hand
	std::vector<Cards *> cardsInHand;
	// temporary card for playing from hand
	Cards *tempPlayCard;
};
