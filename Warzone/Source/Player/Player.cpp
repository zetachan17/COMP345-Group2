#include "Player/Player.h"
#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Orders/Orders.h"

#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;

Player::Player(const string &name) : playerName(name), hand(new Hand), ordersList(new OrdersList),
									 reinforcementPool(0), armiesDeployedThisTurn(0) {}

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
		territory->setOwner(nullptr);
	}
}

void Player::issueOrder(Order *order)
{
	ordersList->addOrder(order);
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

Player::Player() : playerName(""), hand(new Hand), ordersList(new OrdersList),
				   reinforcementPool(0) {}

Player::Player(const Player &otherPlayer)
{
	playerName = otherPlayer.playerName;
	reinforcementPool = otherPlayer.reinforcementPool;
	armiesDeployedThisTurn = otherPlayer.armiesDeployedThisTurn;
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
	finishedIssuingOrders = otherPlayer.finishedIssuingOrders;
	armiesDeployedThisTurn = otherPlayer.armiesDeployedThisTurn;

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
		allAdjacentTerritories.insert(allAdjacentTerritories.end(),
									  currentAdjacentTerritories.begin(),
									  currentAdjacentTerritories.end());
	}

	// remove duplicates
	for (int i = 0; i < allAdjacentTerritories.size(); i++)
	{
		for (int j = i + 1; j < allAdjacentTerritories.size();)
		{
			if (allAdjacentTerritories[i]->getTerritoryName() ==
				allAdjacentTerritories[j]->getTerritoryName())
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
			if (territories[i]->getTerritoryName() ==
				allAdjacentTerritories[j]->getTerritoryName())
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

void Player::issueOrder()
{
	if (armiesDeployedThisTurn != reinforcementPool) {
		this->issueDeployOrder();
	}
	else
	{
		int randomChoice = rand() % (15);

		if (randomChoice <= 5)
		{
			this->issueAdvanceOrder();
		}
		else if (randomChoice <= 10)
		{
			this->playCard();
		}
		else
		{
			this->finishedIssuingOrders = true;
		}
	}
}

void Player::issueDeployOrder()
{
	int remainingArmiesToDeploy = reinforcementPool - armiesDeployedThisTurn;
	int armiesToDeploy = remainingArmiesToDeploy;

	if (remainingArmiesToDeploy > 5)
	{
		int maximumArmies = std::min(remainingArmiesToDeploy, 20);
		int minimumArmies = 3;
		armiesToDeploy = rand() % (maximumArmies - minimumArmies + 1) + minimumArmies;
	}
	vector<Territory*> territoriesToDefend = toDefend();

	Territory* randomTerritoryToDefend = territoriesToDefend[rand() % (territoriesToDefend.size() - 1)];

	//TODO: when order constructors are merged
	ordersList->addOrder(new Deploy(this, armiesToDeploy, randomTerritoryToDefend));
}

void Player::issueAdvanceOrder()
{
	
	//get random territory from toAttack and toDefend
	Territory* sourceTerritory;
	Territory* targetTerritory;

	//determine how many armies
	int armiesToAdvance;

	//TODO: when order constructors are merged
	ordersList->addOrder(new Advance(this, armiesToAdvance, sourceTerritory, targetTerritory));
}

void Player::issueAirliftOrder()
{
	//get random territory from toAttack and toDefend
	Territory* sourceTerritory;
	Territory* targetTerritory;

	//determine how many armies
	int armiesToAirlift;

	//TODO: when order constructors are merged
	ordersList->addOrder(new Airlift(this, armiesToAirlift, sourceTerritory, targetTerritory));
}

void Player::issueBombOrder()
{
	Territory* targetTerritory;
	ordersList->addOrder(new Bomb(this, targetTerritory));
}

void Player::issueBlockadeOrder()
{
	Territory* targetTerritory;
	ordersList->addOrder(new Blockade(this, targetTerritory));
}

void Player::issueNegotiateOrder()
{
	Player* targetPlayer;
	ordersList->addOrder(new Negotiate(this, targetPlayer));
}

void Player::playCard()
{
	Card* randomCard = this->hand->getCards()[rand() % (this->hand->getCards().size() - 1)];
	randomCard->play(this);
}

string Player::getPlayerName()
{
	return playerName;
}

void Player::addTerritory(Territory* territory)
{
	territories.push_back(territory);
}

vector<Territory*> Player::getTerritories() const
{
	return territories;
}

bool Player::isFinishedIssuingOrders() const
{
	return finishedIssuingOrders;
}

void Player::resetIsFinishedIssuingOrders()
{
	this->finishedIssuingOrders = false;
}

int Player::calculateReinforcements(Map &map)
{
	int numberOfTerritories = this->getTerritories().size();

	int territoryReinforcements = std::max(numberOfTerritories / 3, 3);

	int continentBonusReinforcements = this->calculateContinentBonuses(map);

	this->addReinforcements(territoryReinforcements + continentBonusReinforcements);
}

int Player::calculateContinentBonuses(Map &map)
{
	int totalBonus = 0;
	for (Continent* continent : map.getContinents())
	{
		if (this->ownsContinent(continent))
		{
			totalBonus += continent->getBonus();
		}
	}

	return totalBonus;

}

bool Player::ownsContinent(Continent* continent)
{
	for (auto territory : continent->getTerritories()) {
		if (std::find(this->territories.begin(), this->territories.end(), territory) == this->territories.end()) {
			return false;
		}
		return true;
	}
}

Order* Player::nextOrder(bool deployOnly = false)
{
	return ordersList->nextOrder(deployOnly);
}
