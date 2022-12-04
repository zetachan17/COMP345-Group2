#pragma once

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::ostream;
#include <array>
using std::array;

class Player;

class Card
{
public:
	// constructors
	Card();
	Card(string type);
	Card(const Card &card);

	// destructor
	virtual ~Card();

	// assignment operator
	Card &operator=(const Card &card);

	// output stream operator
	friend ostream &operator<<(ostream &output, const Card &card);

	// REQUIRED - plays the card, each subclass overrides this to issue the appropriate Order through the Player class
	virtual void play(Player *player) = 0;

	virtual Card *clone() const = 0;

	// returns the name of the card type
	const string cardType() const;

protected:
	string type;
};

/// Card subclass representing an Airlift card
class AirliftCard : public Card
{
public:
	AirliftCard();
	~AirliftCard();
	void play(Player *player) override;
	Card *clone() const override;
};

/// Card subclass representing a Blockade card
class BlockadeCard : public Card
{
public:
	BlockadeCard();
	~BlockadeCard();
	void play(Player *player) override;
	Card *clone() const override;
};

/// Card subclass representing a Bomb card
class BombCard : public Card
{
public:
	BombCard();
	~BombCard();
	void play(Player *player) override;
	Card *clone() const override;
};

/// Card subclass representing a Negotiate card
class DiplomacyCard : public Card
{
public:
	DiplomacyCard();
	~DiplomacyCard();
	void play(Player *player) override;
	Card *clone() const override;
};

/// Card subclass representing a Reinforcement card
class ReinforcementCard : public Card
{
public:
	ReinforcementCard();
	~ReinforcementCard();
	void play(Player *player) override;
	Card *clone() const override;
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
	friend ostream &operator<<(ostream &output, const Deck &deck);

	// REQUIRED - draws a card at random from the deck
	Card *draw();

	// instantiates the cards in the deck
	void createDeck();

	// adds a card to the deck
	void addToDeck(Card *card);

private:
	vector<Card *> cardsInDeck;

	void clearDeck();
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
	friend ostream &operator<<(ostream &output, const Hand &hand);

	// adds a card to the hand
	void addToHand(Card *card);

	// plays card from hand and adds it back to the deck
	void playCard(Player *player, Deck *deck);

	// plays card from hand and adds it back to the deck
	void playCard(Player *player, const string &typeToPlay);

	// returns number of cards in hand
	int getHandSize() const;

	// getters
	const vector<Card *> getCards() const;

	void returnCardsToDeck(Deck* deck);

	bool hasAirlift();
	bool hasBlockade();
	bool hasBomb();
	bool hasDiplomacy();
	bool hasReinforcement();

private:
	vector<Card *> cardsInHand;
};
