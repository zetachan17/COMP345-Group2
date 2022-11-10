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
void Player::addTerritory(Territory *territory)
{
	territories.push_back(territory);
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

int Player::getReinforcementPool()
{
	return reinforcementPool;
}

//
// original definitions from assignment 1
//
Player::Player()
{
	this->hand = new Hand();
	this->ordersList = new OrdersList();
}

Player::Player(const Player &otherPlayer)
{
	this->hand = new Hand(*otherPlayer.hand);

	this->ordersList = new OrdersList(*otherPlayer.ordersList);

	for (Territory *territory : otherPlayer.territories)
	{
		this->territories.push_back(new Territory(*territory));
	}
}

Player::~Player()
{
	territories.clear();
	delete hand;
	delete ordersList;
}

Player &Player::operator=(const Player &otherPlayer)
{
	this->hand = otherPlayer.hand;
	this->ordersList = otherPlayer.ordersList;
	this->hand = otherPlayer.hand;

	vector<Territory *> territories;
	for (Territory *territory : otherPlayer.territories)
	{
		territories.push_back(new Territory(*territory));
	}
	this->territories = territories;

	return *this;
}

ostream &operator<<(ostream &output, const Player &player)
{
	output << "Player owned territories:" << endl;
	for (Territory *territory : player.territories)
	{
		output << "\t" << *territory << endl;
	}

	output << "Player cards in hand:" << endl;
	output << *player.hand << endl;

	output << "Player orders list:" << endl;
	output << *player.ordersList << endl;

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
