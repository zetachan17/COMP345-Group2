#pragma once

#include "Orders/Orders.h"

#include <vector>
#include <string>
#include <iostream>
using std::ostream;
using std::string;
using std::vector; 

class Card
{
public:
	// card type: 0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
	int type;

	// constructors
	Card();
	Card(const int type);
	Card(const Card &card);

	// destructor
	~Card();

	// assignment operator
	Card& operator=(const Card &card);
	
	// output stream operator
	friend ostream& operator<<(ostream &output, const Card &card); 

	// REQUIRED - plays the card, creates the corresponding order and adds it to the orderlist
	void play(Card* card, OrdersList *orders);

	// creates an order from a card
	Order* createOrder(Card *card);
};

class Deck
{

public:
	// constructors
	Deck();
	Deck(const Deck &deck);

	// destructor
	~Deck();
	
	// assignment operator
	Deck &operator=(const Deck &deck);

	// output stream operator
	friend ostream& operator<<(ostream& output, const Deck& deck);
	
	// REQUIRED - draws a card at random from the deck
	Card* draw();

	// instantiates the cards in the deck
	void createDeck();

	// adds a card to the deck
	void addToDeck(Card* card);

private:
	vector<Card *> cardsInDeck;
};

class Hand
{

public:
	// constructors
	Hand();
	Hand(const Hand &hand);
	
	// destructor
	~Hand();

	// assignment operator
	Hand &operator=(const Hand &card);

	// output stream operator
	friend ostream& operator<<(ostream& output, const Hand& hand);

	// adds a card to the hand
	void addToHand(Card *card);

	// plays card from hand and adds it back to the deck
	Card *playCard(Deck *deck);

	// getters
	vector<Card*> getCards();

private:
	vector<Card*> cardsInHand;
};
