#include "Player/Player.h"
#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Orders/Orders.h"
#include "GameEngine/GameEngine.h"
#include "PlayerStrategies/PlayerStrategies.h"

#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;

Player::Player(const string &name) : playerName(name), hand(new Hand), ordersList(new OrdersList),
									 reinforcementPool(0), armiesDeployedThisTurn(0),
									 finishedIssuingOrders(false), strategy(nullptr) {}

Player::Player(PlayerStrategy *pStrategy) : Player(pStrategy->getStrategyType() + " Player")
{
	strategy = pStrategy;
}

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
				   reinforcementPool(0), armiesDeployedThisTurn(0), finishedIssuingOrders(false) {}

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
	delete strategy;
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
	if (strategy != nullptr)
		return strategy->toDefend();

	return territories;
}

vector<Territory *> Player::toAttack()
{
	if (strategy != nullptr)
		return strategy->toAttack();

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
	if (strategy != nullptr)
		return strategy->issueOrder();

	cout << *this << endl;

	if (armiesDeployedThisTurn < reinforcementPool)
	{
		this->issueDeployOrder();
	}
	else
	{

		int randomChoice = rand() % (100);

		if (randomChoice <= 25)
		{
			this->issueAdvanceOrder();
		}
		else if (randomChoice <= 75)
		{
			if (hand->getCards().size() != 0)
			{
				this->playCard();
			}
			else
			{
				this->issueAdvanceOrder();
			}
		}
		else
		{
			this->finishedIssuingOrders = true;
		}
	}
}

void Player::issueDeployOrder()
{
	cout << "Issuing a Deploy order." << endl;
	int remainingArmiesToDeploy = reinforcementPool - armiesDeployedThisTurn;
	cout << remainingArmiesToDeploy << " armies remain available in their reinforcement pool." << endl;

	int armiesToDeploy = remainingArmiesToDeploy;

	if (remainingArmiesToDeploy > 10)
	{
		int maximumArmies = std::min(remainingArmiesToDeploy, 30);
		int minimumArmies = 10;
		armiesToDeploy = rand() % (maximumArmies - minimumArmies + 1) + minimumArmies;
	}

	if (toDefend().size() == 0)
	{
		return;
	}

	Territory *randomTerritoryToDefend = toDefend()[rand() % (toDefend().size())];
	cout << "Deploying " << armiesToDeploy << " to " << randomTerritoryToDefend->getTerritoryName() << endl;

	ordersList->addOrder(new Deploy(this, armiesToDeploy, randomTerritoryToDefend));

	armiesDeployedThisTurn += armiesToDeploy;
}

void Player::issueAdvanceOrder()
{
	cout << "Issuing an Advance order." << endl;

	if (toAttack().size() == 0)
	{
		return;
	}

	// get random target territory from toAttack
	Territory *targetTerritory = toAttack()[rand() % (toAttack().size())];
	Territory *sourceTerritory;

	// find a territory adjacent to the target that's in toDefend
	for (Territory *territoryAdjacentToTarget : targetTerritory->getAdjacentTerritories())
	{
		vector<Territory *> territoriesToDefend = toDefend();
		if (find_if(territoriesToDefend.begin(), territoriesToDefend.end(), [territoryAdjacentToTarget](Territory *territory)
					{ return territory->getTerritoryName() == territoryAdjacentToTarget->getTerritoryName(); }) == territoriesToDefend.end())
		{
			continue;
		}
		sourceTerritory = territoryAdjacentToTarget;
		break;
	}

	// determine how many armies
	int units = sourceTerritory->getArmyUnits();

	cout << "Advancing " << units << " to " << targetTerritory->getTerritoryName() << " from " << sourceTerritory->getTerritoryName() << endl;

	ordersList->addOrder(new Advance(this, units, sourceTerritory, targetTerritory));
}

void Player::issueAirliftOrder()
{
	cout << "Issuing an Airlift order." << endl;

	if (toDefend().size() == 0)
	{
		return;
	}
	// get random territory from toAttack and toDefend
	Territory *sourceTerritory = toDefend()[rand() % (toDefend().size())];
	Territory *targetTerritory = toDefend()[rand() % (toDefend().size())];

	// determine how many armies
	int units = sourceTerritory->getArmyUnits();

	cout << "Airlifting " << units << " from " << sourceTerritory->getTerritoryName() << " to " << targetTerritory->getTerritoryName() << endl;

	ordersList->addOrder(new Airlift(this, units, sourceTerritory, targetTerritory));
}

void Player::issueBombOrder()
{
	cout << "Issuing a Bomb order." << endl;

	if (toAttack().size() == 0)
	{
		return;
	}

	Territory *targetTerritory = toAttack()[rand() % (toAttack().size())];

	cout << "Bombing " << targetTerritory->getTerritoryName() << endl;
	ordersList->addOrder(new Bomb(this, targetTerritory));
}

void Player::issueBlockadeOrder()
{
	cout << "Issuing a Blockade order." << endl;

	if (toDefend().size() == 0)
	{
		return;
	}

	Territory *targetTerritory = toDefend()[rand() % (toDefend().size())];
	cout << "Blockading " << targetTerritory->getTerritoryName() << endl;

	ordersList->addOrder(new Blockade(this, targetTerritory));
}

void Player::issueNegotiateOrder()
{
	cout << "Issuing a Negotiate order." << endl;

	Player *targetPlayer = nullptr;

	for (Player *player : GameEngine::getPlayers())
	{
		if (player->getPlayerName() != this->playerName)
		{
			targetPlayer = player;
			break;
		}
	}

	cout << "Negotiating with " << targetPlayer->getPlayerName() << endl;

	ordersList->addOrder(new Negotiate(this, targetPlayer));
}

void Player::playCard()
{
	hand->playCard(this, GameEngine::getDeck());
}

string Player::getPlayerName()
{
	return playerName;
}

vector<Territory *> Player::getTerritories() const
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

void Player::resetArmiesDeployedThisTurn()
{
	this->armiesDeployedThisTurn = 0;
}

int Player::calculateReinforcements(Map *const map)
{
	cout << "\n\t-----Calculating reinforcements for player " << playerName << endl;

	int numberOfTerritories = this->getTerritories().size();

	int territoryReinforcements = std::max(numberOfTerritories / 3, 3);

	cout << "Player " << playerName << " owns " << numberOfTerritories << " territories and receives " << territoryReinforcements << " reinforcement army units from territory ownership." << endl;

	int continentBonusReinforcements = this->calculateContinentBonuses(map);

	cout << "Player " << playerName << " receives " << continentBonusReinforcements << " reinforcement army units in continent control bonuses." << endl;

	return territoryReinforcements + continentBonusReinforcements;
}

int Player::calculateContinentBonuses(Map *const map)
{
	int totalBonus = 0;
	for (Continent *continent : map->getContinents())
	{
		if (this->ownsContinent(continent))
		{
			cout << "Player " << playerName << " owns " << continent->getContinentName() << " and will receive a " << continent->getBonus() << " continent control bonus." << endl;
			totalBonus += continent->getBonus();
		}
	}

	return totalBonus;
}

bool Player::ownsContinent(Continent *continent)
{
	for (auto territory : continent->getTerritories())
	{
		if (std::find_if(this->territories.begin(), this->territories.end(), [territory](Territory *ownedTerritory)
						 { return territory->getTerritoryName() == ownedTerritory->getTerritoryName(); }) == this->territories.end())
		{
			return false;
		}
		return true;
	}
	return false;
}

Order *Player::nextOrder(bool deployOnly)
{
	return ordersList->nextOrder(deployOnly);
}

void Player::setPlayerStrategy(PlayerStrategy *pStrategy)
{
	strategy = pStrategy;
}
