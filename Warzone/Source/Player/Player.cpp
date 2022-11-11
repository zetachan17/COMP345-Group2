#include "Player/Player.h"
#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Orders/Orders.h"

#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;

//
// added by raf to implement order execution
//
Player::Player(const string &name) : name(name), hand(new Hand), ordersList(new OrdersList) {}

void Player::addTerritory(Territory *territory)
{
	territories.push_back(territory);
	territory->setOwner(this);
}

void Player::removeTerritory(Territory *territory)
{
	vector<Territory *>::iterator i = find(territories.begin(), territories.end(), territory);
	if (i != territories.end())
	{
		territories.erase(i);
		territory->setOwner(NULL);
	}
}

void Player::issueOrder(Order *order)
{
	ordersList->addOrder(order);
}

Order *Player::nextOrder()
{
	return ordersList->nextOrder();
}

void Player::addReinforcements(int units)
{
	reinforcementPool += units;
}

int Player::getReinforcementPool() const
{
	return reinforcementPool;
}

const string &Player::getName() const
{
	return name;
}

//
// original definitions from assignment 1
//
Player::Player() : name(""), hand(new Hand), ordersList(new OrdersList), reinforcementPool(0) {}

Player::Player(const Player &otherPlayer)
{
	name = otherPlayer.name;
	reinforcementPool = otherPlayer.reinforcementPool;
	hand = new Hand(*otherPlayer.hand);
	ordersList = new OrdersList(*otherPlayer.ordersList);

	for (Territory *territory : otherPlayer.territories)
	{
		territories.push_back(new Territory(*territory));
		territories.back()->setOwner(this);
	}

	this->playerName = otherPlayer.playerName;
	this->numArmies = otherPlayer.numArmies;
}

Player::~Player()
{
	territories.clear();
	delete hand;
	delete ordersList;
}

Player &Player::operator=(const Player &otherPlayer)
{
	name = otherPlayer.name;
	hand = otherPlayer.hand;
	ordersList = otherPlayer.ordersList;
	reinforcementPool = otherPlayer.reinforcementPool;

	for (Territory *territory : otherPlayer.territories)
	{
		territories.push_back(new Territory(*territory));
		territories.back()->setOwner(this);
	}

	return *this;
}

ostream &operator<<(ostream &output, const Player &player)
{
	output << "Player: " << player.name << endl
		   << "\nOwned territories: " << endl;
	for (Territory *territory : player.territories)
		output << "    " << territory->getTerritoryName() << endl;

	output << "\nCards in hand:" << endl
		   << *player.hand << endl;

	output << "Orders list:" << endl
		   << *player.ordersList << endl;

	return output;
}

vector<Territory *> Player::toDefend()
{
	return territories;
}

vector<Territory *> Player::toAttack()
{
	vector<Territory *> allAdjacentTerritories;

	for (Territory *territory : territories)
	{
		vector<Territory *> currentAdjacentTerritories = territory->getAdjacentTerritories();
		allAdjacentTerritories.insert(allAdjacentTerritories.end(), currentAdjacentTerritories.begin(), currentAdjacentTerritories.end());
	}

	// remove duplicates
	for (int i = 0; i < allAdjacentTerritories.size();)
	{
		for (int j = i + 1; j < allAdjacentTerritories.size(); j++)
		{
			if (allAdjacentTerritories[i]->getTerritoryName() == allAdjacentTerritories[j]->getTerritoryName())
			{
				allAdjacentTerritories.erase(allAdjacentTerritories.begin() + j);
				continue;
			}
			j++;
		}
	}

	// remove the player's territories
	for (int i = 0; i < territories.size(); i++)
	{
		for (int j = 0; j < allAdjacentTerritories.size();)
		{
			if (territories[i]->getTerritoryName() == allAdjacentTerritories[j]->getTerritoryName())
			{
				allAdjacentTerritories.erase(allAdjacentTerritories.begin() + j);
				continue;
			}
			j++;
		}
	}

	return allAdjacentTerritories;
}

void Player::issueOrder(string orderName)
{
	//to be refactored
	cout << "Handling order: " << orderName << endl;
}

string Player::getPlayerName()
{
	return playerName;
}

void Player::addTerritory(Territory* territory)
{
	territories.push_back(territory);
}

vector<Territory*> Player::getTerritories()
{
	return territories;
}

Hand* Player::getHand()
{
	return hand;
}

int Player::getNumArmies()
{
	return numArmies;
}

void Player::addNumArmies(int newArmies)
{
	cout << "Player " << getPlayerName() << " received " << newArmies << " armies." << endl;
	numArmies += newArmies;
	cout << "Now they have " << getNumArmies() << " armies" << endl;
}




