#include "Player/Player.h"
#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Orders/Orders.h"

#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;

Player::Player(const string &name) : playerName(name), hand(new Hand), ordersList(new OrdersList), reinforcementPool(0) {}

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
	cout << "Player " << getPlayerName() << " received " << units << " armies." << endl;
	reinforcementPool += units;
	cout << "Now they have " << getReinforcementPool() << " armies" << endl;
}

int Player::getReinforcementPool() const
{
	return reinforcementPool;
}

const string &Player::getPlayerName() const
{
	return playerName;
}

Player::Player() : playerName(""), hand(new Hand), ordersList(new OrdersList), reinforcementPool(0) {}

Player::Player(const Player &otherPlayer)
{
	playerName = otherPlayer.playerName;
	reinforcementPool = otherPlayer.reinforcementPool;
	hand = new Hand(*otherPlayer.hand);
	ordersList = new OrdersList(*otherPlayer.ordersList);

	for (Territory *territory : otherPlayer.territories)
	{
		territories.push_back(new Territory(*territory));
		territories.back()->setOwner(this);
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
	playerName = otherPlayer.playerName;
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
	output << "Player: " << player.playerName << endl
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
	for (int i = 0; i < allAdjacentTerritories.size(); i++)
	{

		for (int j = i + 1; j < allAdjacentTerritories.size(); j++)
		{
			if (allAdjacentTerritories[i]->getTerritoryName() == allAdjacentTerritories[j]->getTerritoryName())
			{
				allAdjacentTerritories.erase(allAdjacentTerritories.begin() + j);
				continue;
			}
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

Hand *Player::getHand()
{
	return hand;
}
