#include "Cards/Cards.h"
#include "Orders/Orders.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
using std::cout;
using std::endl;
using std::vector;

// the number of cards in the deck defined as the number of cards per type multiplied by the number of types
int cardsPerType = 3;
int totalCardsInDeck = cardsPerType * 5;

// array to convert number of card to its respective type
string cardTypes[5]{
	// 0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
	"Bomb",			 // 0
	"Reinforcement", // 1
	"Blockade",		 // 2
	"Airlift",		 // 3
	"Diplomacy"		 // 4
};

Card::Card()
{
}

Card::Card(const int type)
{
	this->type = type;
}

Card::Card(const Card &card)
{
	this->type = card.type;
}

Card::~Card()
{
}

Card &Card::operator=(const Card &card)
{
	this->type = card.type;
	return *this;
}

ostream &operator<<(ostream &output, const Card &card)
{
	output << "Card: " << cardTypes[card.type] << endl;
	return output;
}

void Card::play(Card *card, OrdersList *orders)
{
	cout << "Played card is " << cardTypes[card->type] << endl;
	Order *newOrder = createOrder(card);

	orders->addOrder(newOrder);
}

Order *Card::createOrder(Card *card)
{
	cout << "Creating order for card " << cardTypes[card->type] << endl;
	switch (card->type)
	{
	case 0:
		return new Bomb();
	case 1:
		// TODO: Reinforcement when it's implemented
		return new Bomb();
	case 2:
		return new Blockade();
	case 3:
		return new Airlift();
	case 4:
		return new Negotiate();
	default:
		return new Bomb(); // TODO: fix default order?
	}
}

Deck::Deck()
{
}

Deck::Deck(const Deck &deck)
{
	this->cardsInDeck = *new vector<Card *>(deck.cardsInDeck);
}

Deck::~Deck()
{
	for (auto &card : cardsInDeck)
		delete card;
	cardsInDeck.clear();
}

Deck &Deck::operator=(const Deck &deck)
{

	this->cardsInDeck = *new vector<Card *>(deck.cardsInDeck);
	return *this;
}

ostream &operator<<(ostream &output, const Deck &deck)
{
	if (deck.cardsInDeck.empty())
	{
		cout << "-DECK EMPTY-" << endl;
		return output;
	}
	else
	{
		int i = 1;
		for (auto &card : deck.cardsInDeck)
			cout << i++ << ". " << *card << endl;
		return output;
	}

	return output;
}

// REQUIRED
Card *Deck::draw()
{
	// shuffle deck before drawing
	std::random_device rd;
	std::mt19937 g(rd());
	shuffle(cardsInDeck.begin(), cardsInDeck.end(), g);

	Card *tempDrawCard = cardsInDeck[0];
	cardsInDeck.erase(cardsInDeck.begin());

	cout << "Drawn card is " << cardTypes[tempDrawCard->type] << endl;

	return tempDrawCard;
}

void Deck::createDeck()
{
	// to determine the type of the card and add it to the deck
	for (int i = 0; i < totalCardsInDeck; i++)
	{
		Card *newCard = new Card(i % 5);
		cardsInDeck.push_back(newCard);
	}
}

void Deck::addToDeck(Card *card)
{
	cardsInDeck.push_back(card);
}

Hand::Hand()
{
}

Hand::Hand(const Hand &hand)
{
	this->cardsInHand = *new vector<Card *>(hand.cardsInHand);
}

Hand::~Hand()
{
}

Hand &Hand::operator=(const Hand &hand)
{
	this->cardsInHand = *new vector<Card *>(hand.cardsInHand);
	return *this;
}

ostream &operator<<(ostream &output, const Hand &hand)
{
	if (hand.cardsInHand.empty())
	{
		cout << "-HAND EMPTY-" << endl;
		return output;
	}
	else
	{
		int i = 1;
		for (auto &card : hand.cardsInHand)
			cout << i++ << ". " << *card << endl;
		return output;
	}

	return output;
}

Card *Hand::playCard(Deck *deck)
{
	Card *tempPlayCard = cardsInHand[0];
	cardsInHand.erase(cardsInHand.begin());

	deck->addToDeck(tempPlayCard);

	return tempPlayCard;
}

void Hand::addToHand(Card *card)
{
	cardsInHand.push_back(card);
	cout << "Card " << cardTypes[card->type] << " has been added to the hand." << endl;
}

vector<Card *> Hand::getCards()
{
	return cardsInHand;
}
