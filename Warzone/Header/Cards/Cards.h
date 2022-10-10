#pragma once
#include <vector>

class Player
{

public:
};

class Order
{

public:
	int type;
};

class OrdersList
{

public:
	void addToOrders(Order *order);
};

class Cards : public OrdersList
{

public:
	// constructors
	Cards();
	Cards(const Cards &card);
	Cards &operator=(const Cards &card);

	// card type: 0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
	int type;

	void play(Cards *card);
	void createOrder(Cards *card);
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

private:
	// creation of a vector of card pointers, this will be the player hand
	std::vector<Cards *> cardsInHand;
	// temporary card for playing from hand
	Cards *tempPlayCard;
};
