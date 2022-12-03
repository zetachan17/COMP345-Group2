#include "Cards/Cards.h"
#include "Orders/Orders.h"
#include "Player/Player.h"
#include "GameEngine/GameEngine.h"

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <algorithm>
#include <random>
#include <string>
#include <array>
using std::array;

// number of cards in the deck defined as number of cards per type multiplied by number of types
const int CARDS_PER_TYPE = 3;
const int TOTAL_CARDS_IN_DECK = CARDS_PER_TYPE * 5;

Card::Card() : type("") {}

Card::Card(string type) : type(type) {}

Card::Card(const Card &card)
{
	type = card.type;
}

Card::~Card() {}

Card &Card::operator=(const Card &card)
{
	this->type = card.type;
	return *this;
}

ostream &operator<<(ostream &output, const Card &card)
{
	output << card.type << " Card";
	return output;
}

const string Card::cardType() const
{
	return type;
}

AirliftCard::AirliftCard() : Card("Airlift") {}

AirliftCard::~AirliftCard() {}

void AirliftCard::play(Player *player)
{
	cout << "*Playing an Airlift Card*" << endl;
	player->issueAirliftOrder();
}

BlockadeCard::BlockadeCard() : Card("Blockade") {}

BlockadeCard::~BlockadeCard() {}

void BlockadeCard::play(Player *player)
{
	cout << "*Playing a Blockade Card*" << endl;
	player->issueBlockadeOrder();
}

BombCard::BombCard() : Card("Bomb") {}

BombCard::~BombCard() {}

void BombCard::play(Player *player)
{
	cout << "*Playing a Bomb Card*" << endl;
	player->issueBombOrder();
}

DiplomacyCard::DiplomacyCard() : Card("Diplomacy") {}

DiplomacyCard::~DiplomacyCard() {}

void DiplomacyCard::play(Player *player)
{
	cout << "*Playing a Diplomacy Card*" << endl;
	player->issueNegotiateOrder();
}

ReinforcementCard::ReinforcementCard() : Card("Reinforcement") {}

ReinforcementCard::~ReinforcementCard() {}

void ReinforcementCard::play(Player *player)
{
	cout << "*Playing a Reinforcement Card*" << endl;
	player->addReinforcements(5);
}

Deck::Deck()
{
	createDeck();
}

Deck::Deck(const Deck &deck)
{
	for (Card *card : deck.cardsInDeck)
		this->cardsInDeck.push_back(card->clone());
}

Deck::~Deck()
{
	clearDeck();
}

Deck &Deck::operator=(const Deck &deck)
{
	for (Card *card : deck.cardsInDeck)
		this->cardsInDeck.push_back(card->clone());

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
}

// REQUIRED
Card *Deck::draw()
{
	if (cardsInDeck.size() == 0)
		return nullptr;
	if (cardsInDeck.size() == 1)
		return cardsInDeck[0];

	// shuffle deck before drawing
	std::random_device rd;
	std::mt19937 g(rd());
	shuffle(cardsInDeck.begin(), cardsInDeck.end(), g);

	Card *cardDrawn = cardsInDeck[0];
	cardsInDeck.erase(cardsInDeck.begin());

	cout << "Drawn card is " << cardDrawn->cardType() << endl;

	return cardDrawn;
}

void Deck::clearDeck()
{
	for (auto &card : cardsInDeck)
		delete card;
	cardsInDeck.clear();
}

void Deck::createDeck()
{
	if (cardsInDeck.size() != 0)
		clearDeck();

	// to determine the type of the card and add it to the deck
	for (int i = 0; i < TOTAL_CARDS_IN_DECK; i++)
	{
		switch (i % 5)
		{
		case 0:
			cardsInDeck.push_back(new AirliftCard);
			break;
		case 1:
			cardsInDeck.push_back(new BlockadeCard);
			break;
		case 2:
			cardsInDeck.push_back(new BombCard);
			break;
		case 3:
			cardsInDeck.push_back(new DiplomacyCard);
			break;
		case 4:
			cardsInDeck.push_back(new ReinforcementCard);
			break;
		}
	}
}

void Deck::addToDeck(Card *card)
{
	cardsInDeck.push_back(card);
}

Hand::Hand() : cardTypesInHand{0} {}

Hand::Hand(const Hand &hand)
{
	for (Card *card : hand.cardsInHand)
		this->cardsInHand.push_back(card->clone());

	for (int i = 0; i < cardTypesInHand.size(); i++)
		this->cardTypesInHand[i] = hand.cardTypesInHand[i];
}

Hand::~Hand()
{
	for (auto &card : cardsInHand)
		delete card;
	cardsInHand.clear();
}

Hand &Hand::operator=(const Hand &hand)
{
	for (Card *card : hand.cardsInHand)
		this->cardsInHand.push_back(card->clone());

	for (int i = 0; i < cardTypesInHand.size(); i++)
		this->cardTypesInHand[i] = hand.cardTypesInHand[i];

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
			cout << "    " << i++ << ". " << *card << endl;
		return output;
	}
}

void Hand::addToHand(Card *card)
{
	cardsInHand.push_back(card);
	cout << "Card " << card->cardType() << " has been added to the hand." << endl;

	updateCardTypesInHand(card->cardType(), 1);
}

void Hand::playCard(Player *player, Deck *deck)
{
	int randomIndex = rand() % (getCards().size());
	Card *randomCard = getCards()[randomIndex];

	cardsInHand.erase(cardsInHand.begin() + randomIndex);

	deck->addToDeck(randomCard);

	randomCard->play(player);

	updateCardTypesInHand(randomCard->cardType(), -1);
}

void Hand::playCard(Player *player, const string &typeToPlay)
{
	for (int i = 0; i < getHandSize(); i++)
	{
		if (cardsInHand[i]->cardType() == typeToPlay)
		{
			cardsInHand[i]->play(player);
			GameEngine::getDeck()->addToDeck(cardsInHand[i]);
			cardsInHand.erase(cardsInHand.begin() + i);
			break;
		}
	}

	updateCardTypesInHand(typeToPlay, -1);
}

int Hand::getHandSize() const
{
	return cardsInHand.size();
}

const vector<Card *> Hand::getCards() const
{
	return cardsInHand;
}

bool Hand::hasAirlift()
{
	return cardTypesInHand[0];
}

bool Hand::hasBlockade()
{
	return cardTypesInHand[1];
}

bool Hand::hasBomb()
{
	return cardTypesInHand[2];
}

bool Hand::hasNegotiate()
{
	return cardTypesInHand[3];
}

void Hand::updateCardTypesInHand(const string &type, int add)
{
	if (type == "Airlift")
		cardTypesInHand[0] += add;
	else if (type == "Blockade")
		cardTypesInHand[1] += add;
	else if (type == "Bomb")
		cardTypesInHand[2] += add;
	else if (type == "Diplomacy")
		cardTypesInHand[3] += add;
}

void Hand::returnCardsToDeck(Deck *deck)
{
	for (Card *card : cardsInHand)
		deck->addToDeck(card);

	cardsInHand.clear();
	cardTypesInHand = {0};
}

Card *AirliftCard::clone() const
{
	return new AirliftCard(*this);
}

Card *BlockadeCard::clone() const
{
	return new BlockadeCard(*this);
}

Card *BombCard::clone() const
{
	return new BombCard(*this);
}

Card *DiplomacyCard::clone() const
{
	return new DiplomacyCard(*this);
}

Card *ReinforcementCard::clone() const
{
	return new ReinforcementCard(*this);
}