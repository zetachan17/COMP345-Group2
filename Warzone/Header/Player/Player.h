#pragma once

#include <vector>
#include <string>
#include <iostream>
using std::ostream;
using std::string;
using std::vector;

class Territory;
class Continent;
class Map;
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

	// Used to test specific orders
	void issueOrder(Order *order);


	// REQUIRED - the player issues an order by adding it to their orderslist
	void issueOrder();
	
	// methods for creating and issuing a specific order
	void issueDeployOrder();
	void issueAdvanceOrder();
	void issueAirliftOrder();
	void issueBlockadeOrder();
	void issueBombOrder();
	void issueNegotiateOrder();

	// returns pointer to player's next order
	Order* nextOrder(bool deployOnly = false);

	string getPlayerName();
	
	vector<Territory*> getTerritories() const;

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

	// returns whether the player has finished issuing orders
	bool isFinishedIssuingOrders() const;

	// resets the player to not have finished issuing orders
	void resetIsFinishedIssuingOrders();

	// calculates the reinforcements for a player based on owned territories and continents
	int calculateReinforcements(Map* const map);
private:
	vector<Territory *> territories;
	Hand *hand;
	OrdersList *ordersList;
	string playerName;
	int reinforcementPool;
	bool finishedIssuingOrders;
	int armiesDeployedThisTurn;
	
	// plays a random card from the players hand
	void playCard();

	// helper method for calculating reinforcements
	int calculateContinentBonuses(Map* const map);
	bool ownsContinent(Continent* continent);
};
