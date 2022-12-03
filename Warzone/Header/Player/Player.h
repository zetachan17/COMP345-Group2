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
class PlayerStrategy;

class Player
{
public:
	// constructors
	Player();
	Player(const string &name);
	Player(PlayerStrategy *pStrategy);
	Player(const string &name, PlayerStrategy *pStrategy);
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

	// REQUIRED - the player issues an order by adding it to their orderslist
	void issueOrder();

	// methods for creating and issuing a specific order
	void issueDeployOrder();
	void issueAdvanceOrder();
	void issueAirliftOrder();
	void issueBlockadeOrder();
	void issueBombOrder();
	void issueNegotiateOrder();

	// adds an issued order to the player's orders list
	void addToOrdersList(Order *order);

	// returns pointer to player's next order
	Order *nextOrder(bool deployOnly = false);

	// returns whether the player has finished issuing orders
	bool isFinishedIssuingOrders() const;

	// sets bool signaling if player finished issuing orders this turn
	void setIsFinishedIssuingOrders(bool finishedIssuingOrders);

	// returns pointer to player's hand of cards
	Hand *getHand();

	// plays a random card from the players hand
	void playCard();

	// returns vector of pointers to territories controlled by the player
	vector<Territory *> getTerritories() const;

	// adds a territory to the player and updates the territory's ownership
	void addTerritory(Territory *territory);

	// removes a territory from the player and sets a territory's owner to null
	void removeTerritory(Territory *territory);

	// calculates the reinforcements for a player based on owned territories and continents
	int calculateReinforcements(Map *const map);

	// returns number of army units in the reinforcement pool
	int getReinforcementPool() const;

	// adds army units to a player's reinforcement pool
	void addReinforcements(int units);

	// returns number of army units deployed in the current turn
	int getArmiesDeployedThisTurn() const;

	// sets the number of armies deployed this turn
	void setArmiesDeployedThisTurn(int deployed);

	// returns string of player's name
	const string &getPlayerName() const;

	string getStrategyType() const;

	// sets a new player strategy
	void setPlayerStrategy(PlayerStrategy *pStrategy);

private:
	vector<Territory *> territories;
	string playerName;
	PlayerStrategy *strategy;
	Hand *hand;
	OrdersList *ordersList;
	int reinforcementPool;
	int armiesDeployedThisTurn;
	bool finishedIssuingOrders;

	// helper method for calculating reinforcements
	int calculateContinentBonuses(Map *const map);
	bool ownsContinent(Continent *continent);
};
