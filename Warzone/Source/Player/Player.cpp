#include "Player/Player.h"

#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;

Player::Player()
{
	this->numArmies = 0;
}

Player::Player(string playerName)
{
	this->playerName = playerName;
	this->numArmies = 0;
}


Player::Player(const Player& otherPlayer)
{
	this->hand = new Hand(*otherPlayer.hand);

	this->ordersList = new OrdersList(*otherPlayer.ordersList);

	for (Territory* territory : otherPlayer.territories)
	{
		this->territories.push_back(new Territory(*territory));
	}

	this->playerName = otherPlayer.playerName;
	this->numArmies = otherPlayer.numArmies;
}

Player::~Player() 
{
	territories.clear();
	hand->~Hand();
	ordersList->~OrdersList();
	delete hand;
	delete ordersList;
}

Player &Player::operator=(const Player &otherPlayer)
{
	this->hand = otherPlayer.hand;
	this->ordersList = otherPlayer.ordersList;
	this->hand = otherPlayer.hand;
	this->playerName = otherPlayer.playerName;

	vector<Territory*> territories;
	for (Territory* territory : otherPlayer.territories)
	{
		territories.push_back(new Territory(*territory));
	}
	this->territories = territories;

	return *this;
}

ostream& operator<<(ostream& output, const Player& player)
{
	output << "Player owned territories:" << endl;
	for (Territory* territory : player.territories)
	{
		output << "\t" << territory << endl;
	}

	output << "Player cards in hand:" << endl;
	for (Cards* card : player.hand->getCards())
	{
		output << "\t" << card << endl;
	}

	output << "Player orders list:" << endl;
	for (Order* order : player.ordersList->getOrders())
	{
		output << "\t" << order << endl;
	}

	output << "Player name: "  + player.playerName << endl;
	
	return output;
}

vector<Territory*> Player::toAttack()
{
	vector<Territory*> allAdjacentTerritories;

	for (Territory* territory : territories)
	{
		vector<Territory*> currentAdjacentTerritories = territory->getAdjacentTerritories();
		allAdjacentTerritories.insert(allAdjacentTerritories.end(), currentAdjacentTerritories.begin(), currentAdjacentTerritories.end());
	}

	// remove duplicates
	for (int i = 0; i < allAdjacentTerritories.size();)
	{
		for (int j = i+1; j < allAdjacentTerritories.size(); j++)
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

vector<Territory*> Player::toDefend()
{
	return territories;
}

vector<string> cardOrders{ "bomb", "blockade", "airlift", "diplomacy" };

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




