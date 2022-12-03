#include "Player/Player.h"
#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Orders/Orders.h"
#include "GameEngine/GameEngine.h"
#include "PlayerStrategies/PlayerStrategies.h"

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>

Player::Player() : playerName(""), strategy(nullptr), hand(new Hand), ordersList(new OrdersList),
				   reinforcementPool(0), armiesDeployedThisTurn(0), finishedIssuingOrders(false)
{
}

Player::Player(const string &name) : playerName(name), strategy(nullptr), hand(new Hand),
									 ordersList(new OrdersList), reinforcementPool(0),
									 armiesDeployedThisTurn(0), finishedIssuingOrders(false) {}

Player::Player(const string &name, PlayerStrategy *pStrategy)
	: playerName(name), strategy(pStrategy), hand(new Hand), ordersList(new OrdersList),
	  reinforcementPool(0), armiesDeployedThisTurn(0), finishedIssuingOrders(false)
{
	strategy->setPlayer(this);
}

Player::Player(PlayerStrategy *pStrategy)
	: Player((pStrategy->getStrategyType() + " Player"), pStrategy) {}

Player::Player(const Player &otherPlayer)
{
	playerName = otherPlayer.playerName;
	strategy = otherPlayer.strategy;
	hand = new Hand(*otherPlayer.hand);
	ordersList = new OrdersList(*otherPlayer.ordersList);
	reinforcementPool = otherPlayer.reinforcementPool;
	armiesDeployedThisTurn = otherPlayer.armiesDeployedThisTurn;
	finishedIssuingOrders = otherPlayer.finishedIssuingOrders;

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
	strategy = otherPlayer.strategy;
	hand = otherPlayer.hand;
	ordersList = otherPlayer.ordersList;
	reinforcementPool = otherPlayer.reinforcementPool;
	armiesDeployedThisTurn = otherPlayer.armiesDeployedThisTurn;
	finishedIssuingOrders = otherPlayer.finishedIssuingOrders;

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
		output << "    " << std::left << std::setw(12) << territory->getTerritoryName()
			   << " : " << territory->getArmyUnits() << " army units\n";

	output << "\nCards in hand:" << endl
		   << *player.hand << endl;

	output << "Issued orders:" << endl
		   << *player.ordersList << endl;

	return output;
}

vector<Territory *> Player::toDefend()
{
	return strategy->toDefend();
}

vector<Territory *> Player::toAttack()
{
	return strategy->toAttack();
}

void Player::issueOrder()
{
	strategy->issueOrder();
}

void Player::issueDeployOrder()
{
	cout << "Issuing a Deploy order." << endl;
	int remainingUnitsToDeploy = reinforcementPool - armiesDeployedThisTurn;
	cout << remainingUnitsToDeploy << " armies remain available in their reinforcement pool." << endl;

	int armiesToDeploy = remainingUnitsToDeploy;

	if (remainingUnitsToDeploy > 10)
	{
		int maximumArmies = std::min(remainingUnitsToDeploy, 30);
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
	if (strategy->getStrategyType() == "Human")
	{
		strategy->issueAirliftOrder();
		return;
	}
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
	if (strategy->getStrategyType() == "Human")
	{
		strategy->issueBombOrder();
		return;
	}
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
	if (strategy->getStrategyType() == "Human")
	{
		strategy->issueBlockadeOrder();
		return;
	}

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
	if (strategy->getStrategyType() == "Human")
	{
		strategy->issueNegotiateOrder();
		return;
	}

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

void Player::addToOrdersList(Order *order)
{
	ordersList->addOrder(order);
	cout << "\nIssued Order: " << *order << endl;
}

Order *Player::nextOrder(bool deployOnly)
{
	return ordersList->nextOrder(deployOnly);
}

void Player::printIssuedOrders()
{
	cout << "Issued Orders:\n"
		 << *ordersList;
}

bool Player::isFinishedIssuingOrders() const
{
	return finishedIssuingOrders;
}

void Player::setIsFinishedIssuingOrders(bool finishedIssuingOrders)
{
	this->finishedIssuingOrders = finishedIssuingOrders;
	cout << getPlayerName() << " has no more orders to issue.\n";
}

Hand *Player::getHand()
{
	return hand;
}

void Player::playCard()
{
	hand->playCard(this, GameEngine::getDeck());
}

vector<Territory *> Player::getTerritories() const
{
	return territories;
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

int Player::calculateReinforcements(Map *const map)
{
	cout << "\n\t-----Calculating reinforcements for player " << playerName << endl;

	int numberOfTerritories = this->getTerritories().size();

	int territoryReinforcements = std::max(numberOfTerritories / 3, 3);

	cout << "Player " << playerName << " owns " << numberOfTerritories
		 << " territories and receives " << territoryReinforcements
		 << " reinforcement army units from territory ownership." << endl;

	int continentBonusReinforcements = this->calculateContinentBonuses(map);

	cout << "Player " << playerName << " receives " << continentBonusReinforcements
		 << " reinforcement army units in continent control bonuses." << endl;

	return territoryReinforcements + continentBonusReinforcements;
}

int Player::getReinforcementPool() const
{
	return reinforcementPool;
}

void Player::addReinforcements(int units)
{
	cout << "Player " << getPlayerName() << " received " << units << " armies." << endl;
	reinforcementPool += units;
	cout << "Now they have " << getReinforcementPool() << " armies" << endl;
}

int Player::getArmiesDeployedThisTurn() const
{
	return armiesDeployedThisTurn;
}

void Player::setArmiesDeployedThisTurn(int deployed)
{
	armiesDeployedThisTurn = deployed;
}

const string &Player::getPlayerName() const
{
	return playerName;
}

string Player::getStrategyType() const
{
	return strategy->getStrategyType();
}

void Player::setPlayerStrategy(PlayerStrategy *pStrategy)
{
	strategy = pStrategy;
}

int Player::calculateContinentBonuses(Map *const map)
{
	int totalBonus = 0;
	for (Continent *continent : map->getContinents())
	{
		if (this->ownsContinent(continent))
		{
			cout << "Player " << playerName << " owns " << continent->getContinentName()
				 << " and will receive a " << continent->getBonus() << " continent control bonus.\n";
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
