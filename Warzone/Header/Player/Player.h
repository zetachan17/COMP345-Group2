#pragma once

#include <vector>
#include <string>
#include <iostream>
using std::ostream;	
using std::string;
using std::vector;

class Territory;
class Hand;
class Order;
class OrdersList;

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

	// returns pointer to player's next order
	Order *nextOrder();

	// adds a territory to the player and updates the territory's ownership
	void addTerritory(Territory *territory);

	// removes a territory from the player and sets a territory's owner to null
	void removeTerritory(Territory *territory);

	// adds army units to a player's reinforcement pool
	void addReinforcements(int units);

	// returns number of army units in the reinforcement pool
	int getReinforcementPool() const;

	// returns string of player's name
	const string &getPlayerName() const;

	// returns pointer to player's hand of cards
	Hand *getHand();

private:
	vector<Territory *> territories;
	Hand *hand;
	OrdersList *ordersList;
	string name;
	string playerName;
	int reinforcementPool;
};
