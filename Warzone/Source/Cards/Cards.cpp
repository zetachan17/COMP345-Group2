#include "Cards/Cards.h"
#include "Orders/Orders.h"
#include "Player/Player.h"

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

Card::Card() : type("") {}

Card::Card(string type) : type(type) {}

Card::Card(const Card &card)
{
	type = card.type;
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
	output << "Card: " << card.type << endl;
	return output;
}

string Card::cardType() const
{
	return type;
}


AirliftCard::AirliftCard() : Card("Airlift") {}

AirliftCard::~AirliftCard() {}

void AirliftCard::play(Player* player)
{
	cout << "Playing a Airlift card" << endl;
	player->issueAirliftOrder();
}

BlockadeCard::BlockadeCard() : Card("Blockade") {}

BlockadeCard::~BlockadeCard() {}

void BlockadeCard::play(Player* player)
{
	cout << "Playing a Blockade card" << endl;
	player->issueBlockadeOrder();
}

BombCard::BombCard() : Card("Bomb") {}

BombCard::~BombCard() {}

void BombCard::play(Player* player)
{
	cout << "Playing a Bomb card" << endl;
	player->issueBombOrder();
}

DiplomacyCard::DiplomacyCard() : Card("Diplomacy") {}

DiplomacyCard::~DiplomacyCard() {}

void DiplomacyCard::play(Player* player)
{
	cout << "Playing a Diplomacy card" << endl;
	player->issueNegotiateOrder();
}

ReinforcementCard::ReinforcementCard() : Card("Reinforcement") {}

ReinforcementCard::~ReinforcementCard() {}

void ReinforcementCard::play(Player* player)
{
	cout << "Playing a Reinforcement card" << endl;
	player->addReinforcements(5);
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

	cout << "Drawn card is " << tempDrawCard->cardType() << endl;

	return tempDrawCard;
}

void Deck::createDeck()
{
	// to determine the type of the card and add it to the deck
	for (int i = 0; i < totalCardsInDeck; i++)
	{
		Card* newCard;
		switch (i % 5)
		{
			case 0:
				newCard = new AirliftCard();
				break;
			case 1:
				newCard = new BlockadeCard();
				break;
			case 2:
				newCard = new BombCard();
				break;
			case 3:
				newCard = new DiplomacyCard();
				break;
			case 4:
				newCard = new ReinforcementCard();
				break;

		}

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

void Hand::playCard(Player *player, Deck *deck)
{
	int randomIndex = rand() % (getCards().size());
	Card* randomCard = getCards()[randomIndex];
	
	cardsInHand.erase(cardsInHand.begin() + randomIndex);

	deck->addToDeck(randomCard);

	randomCard->play(player);
}

void Hand::addToHand(Card *card)
{
	cardsInHand.push_back(card);
	cout << "Card " << card->cardType() << " has been added to the hand." << endl;
}

vector<Card *> Hand::getCards()
{
	return cardsInHand;
}
