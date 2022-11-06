#include "Cards/Cards.h"
#include "Orders/Orders.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

// order vector
std::vector<Order *> orderVector;

// nbOfCards is the number of cards per type. 5 is the number of types there are so we multiply to get an even distribution of cards
int nbOfCards = 3;
int totalCards = nbOfCards * 5;

// array to convert number of card to its respective type
std::string cardsType[5]{
	// 0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
	"Bomb",			 // 0
	"Reinforcement", // 1
	"Blockade",		 // 2
	"Airlift",		 // 3
	"Diplomacy"		 // 4
};

// Cards constructors
Cards::Cards()
{
}

Cards::Cards(const Cards &card)
{
}

Cards &Cards::operator=(const Cards &card)
{
	this->type = *new int(card.type);
	return *this;
}

// function to play the card from the hand
void Cards::play(Cards *card)
{

	std::cout << "Played card is " << cardsType[card->type] << std::endl;

	createOrder(card);
}

// function to create an order and add it to the players' list of orders
void Cards::createOrder(Cards *card)
{

	std::cout << "Creating order for card " << cardsType[card->type] << std::endl;

	Order order;
	Order *ptrO = &order;
	ptrO->type = card->type;

	addToOrders(ptrO);
}

// function to add the created order to the players' list of orders
void OrdersList::addToOrders(Order *order)
{

	orderVector.push_back(order);
	std::cout << "Order for " << cardsType[order->type] << " has been added to the list." << std::endl;
}

// Deck constructors
Deck::Deck()
{
}

Deck::~Deck()
{
	for (auto &card : deckOfCards)
		delete card;
	deckOfCards.clear();
}

Deck::Deck(const Deck &deck)
{

	this->deckOfCards = *new std::vector<Cards *>(deck.deckOfCards);
	this->tempDrawCard = new Cards(*(deck.tempDrawCard));
}

Deck &Deck::operator=(const Deck &deck)
{

	this->deckOfCards = *new std::vector<Cards *>(deck.deckOfCards);
	this->tempDrawCard = new Cards(*(deck.tempDrawCard));
	return *this;
}

// function to create a deck of cards depending on how many were declared above. also shuffles the deck once it has been created
void Deck::createDeck()
{
	// to determine the type of the card and add it to the deck
	int counter = 0;
	for (int i = 0; i < totalCards; i++)
	{
		switch (counter)
		{
		case 0:
		{
			Cards *ptrC = new Cards();
			ptrC->type = 0;
			deckOfCards.push_back(ptrC);
			counter++;
			break;
		}
		case 1:
		{
			Cards *ptrC = new Cards();
			ptrC->type = 1;
			deckOfCards.push_back(ptrC);
			counter++;
			break;
		}
		case 2:
		{
			Cards *ptrC = new Cards();
			ptrC->type = 2;
			deckOfCards.push_back(ptrC);
			counter++;
			break;
		}
		case 3:
		{
			Cards *ptrC = new Cards();
			ptrC->type = 3;
			deckOfCards.push_back(ptrC);
			counter++;
			break;
		}
		case 4:
		{
			Cards *ptrC = new Cards();
			ptrC->type = 4;
			deckOfCards.push_back(ptrC);
			counter = 0;
			break;
		}
		}
	}

	// shuffle the deck of cards
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(deckOfCards.begin(), deckOfCards.end(), g);
}

// function to print the contents of the deck
void Deck::printDeck()
{
	std::cout << "Content of deck: " << std::endl;
	for (int i = 0; i < deckOfCards.size(); i++)
	{
		std::cout << cardsType[deckOfCards[i]->type] << std::endl;
	}
}

// function to draw a card from the deck, remove it from the deck, and return it
Cards *Deck::draw()
{

	tempDrawCard = deckOfCards[0];
	deckOfCards.erase(deckOfCards.begin());

	std::cout << "Drawn card is " << cardsType[tempDrawCard->type] << std::endl;

	return tempDrawCard;
}

// function to add the card back to the deck after it has been played
void Deck::addToDeck(Cards *card)
{

	deckOfCards.push_back(card);
}

// Hand constructors
Hand::Hand()
{
}

Hand::~Hand()
{
}

Hand::Hand(const Hand &hand)
{

	this->cardsInHand = *new std::vector<Cards *>(hand.cardsInHand);
	this->tempPlayCard = new Cards(*(hand.tempPlayCard));
}

Hand &Hand::operator=(const Hand &hand)
{

	this->cardsInHand = *new std::vector<Cards *>(hand.cardsInHand);
	this->tempPlayCard = new Cards(*(hand.tempPlayCard));
	return *this;
}

// function to print the contents of the hand
void Hand::printHand()
{

	std::cout << "Content of hand: " << std::endl;
	for (int i = 0; i < cardsInHand.size(); i++)
	{
		std::cout << cardsType[cardsInHand[i]->type] << std::endl;
	}
}

// function to add the card that is drawn from the deck, into the hand
void Hand::addToHand(Cards *card)
{

	cardsInHand.push_back(card);
	std::cout << "Card " << cardsType[card->type] << " has been added to the hand." << std::endl;
}

// function to play card from hand and call the function to add it back to the deck
Cards *Hand::playCard(Deck *deck)
{

	tempPlayCard = cardsInHand[0];
	cardsInHand.erase(cardsInHand.begin());

	deck->addToDeck(tempPlayCard);

	return tempPlayCard;
}