#pragma once

class Territory;
class Hand;
class Order;
class OrdersList;

#include <vector>
#include <string>
#include <iostream>
using std::ostream;
using std::string;
using std::vector;

class Player
{
public:
	// constructors
	Player();
	Player(const string &name);
	Player(const Player &player);

	// destructor
	~Player();

	// assignment operator
	Player &operator=(const Player &player);

	// output stream operator
	friend ostream &operator<<(ostream &output, const Player &player);

	// REQUIRED - lists territories that the player needs to defend
	vector<Territory *> toDefend();

	// REQUIRED - lists territories that the player is able to attack
	vector<Territory *> toAttack();

	// REQUIRED - issues an order by adding it to the player's ordersList.
	// if the order requires a card, the player's cards are checked before issuing the order
	// the card is then removed from the player's cards
	void issueOrder(Order *order);

	Order *nextOrder();

	void addTerritory(Territory *territory);

	void removeTerritory(Territory *territory);

	void addReinforcements(int units);

	int getReinforcementPool() const;

	const string &getPlayerName() const;

	Hand *getHand();

private:
	vector<Territory *> territories;
	Hand *hand;
	OrdersList *ordersList;
	string name;
	string playerName;
	int reinforcementPool;
};
