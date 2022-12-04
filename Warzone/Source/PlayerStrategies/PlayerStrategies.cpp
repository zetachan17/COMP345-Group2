#include "PlayerStrategies/PlayerStrategies.h"
#include "Player/Player.h"
#include "Map/Map.h"
#include "Orders/Orders.h"
#include "Cards/Cards.h"
#include "GameEngine/GameEngine.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::to_string;
#include <iomanip>
#include <vector>
using std::vector;
#include <unordered_set>
using std::unordered_set;
#include <algorithm>
using std::find;
using std::sort;
#include <utility>
using std::pair;

PlayerStrategy::PlayerStrategy() : player(nullptr) {}

PlayerStrategy::PlayerStrategy(Player *pl) : player(pl) {}

PlayerStrategy::~PlayerStrategy() {}

void PlayerStrategy::setPlayer(Player *pl)
{
    player = pl;
}

bool PlayerStrategy::hasUnitsOnMap() const
{
    for (Territory *territory : toDefend())
        if (territory->getArmyUnits() != 0)
            return true;

    return false;
}

HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}

HumanPlayerStrategy::HumanPlayerStrategy(Player *pl) : PlayerStrategy(pl) {}

HumanPlayerStrategy::~HumanPlayerStrategy() {}

void HumanPlayerStrategy::issueOrder()
{
    while (player->getHand()->hasReinforcement())
        player->getHand()->playCard(player, "Reinforcement");

    if (player->isFinishedIssuingOrders())
        cout << player->getPlayerName() << " has no more orders to issue.\n";
    else if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        issueDeployOrder();
    else
    {
        const string selected = selectOrder();

        if (selected == "Pass")
            player->setIsFinishedIssuingOrders(true);
        else if (selected == "Advance")
            issueAdvanceOrder();
        else
            player->getHand()->playCard(player, selected);
    }
}

void HumanPlayerStrategy::issueAirliftOrder()
{
    cout << "*Issuing Airlift Order*\n\n";

    printTerritoriesAndUnits(toDefend(), "Controlled");
    Territory *source = selectTerritory(toDefend(), "source");

    while (source->getArmyUnits() == 0)
    {
        invalidInput(false, source->getTerritoryName() + " has no army units to airlift");
        source = selectTerritory(toDefend(), "source");
    }

    Territory *target = selectTerritory(toDefend(), "target");

    while (target == source)
    {
        invalidInput(false, "Unauthorized to airlift units to and from the same territory");
        target = selectTerritory(toDefend(), "target");
    }

    int armyUnits = selectArmyUnits(source, "airlift from ");

    player->addToOrdersList(new Airlift(player, armyUnits, source, target));
}

void HumanPlayerStrategy::issueBlockadeOrder()
{
    cout << "*Issuing Blockade Order*\n\n";

    printTerritoriesAndUnits(toDefend(), "Controlled");
    Territory *target = selectTerritory(toDefend(), "target");

    player->addToOrdersList(new Blockade(player, target));
}

void HumanPlayerStrategy::issueBombOrder()
{
    cout << "*Issuing Bomb Order*\n\n";

    vector<Territory *> enemyTerritories(toAttack());

    printTerritoriesAndUnits(enemyTerritories, "Adjacent Enemy");
    Territory *target = selectTerritory(enemyTerritories, "target");

    player->addToOrdersList(new Bomb(player, target));
}

void HumanPlayerStrategy::issueNegotiateOrder()
{
    cout << "*Issuing Negotiate Order*\n"
         << "\nEnemy Players:\n\n";

    vector<Player *> enemyPlayers;
    for (int i = 0; i < GameEngine::getPlayers().size(); i++)
        if (GameEngine::getPlayers()[i] != player && !inNegotiations(GameEngine::getPlayers()[i]))
        {
            enemyPlayers.push_back(GameEngine::getPlayers()[i]);
            cout << (i + 1) << ". " << GameEngine::getPlayers()[i]->getPlayerName() << endl;
        }

    int playerInput;
    cout << "\nSelect an enemy player (1-" << enemyPlayers.size() << ") : ";
    cin >> playerInput;

    while (playerInput > enemyPlayers.size() || playerInput < 1)
    {
        invalidInput(true, to_string(enemyPlayers.size()));
        cin >> playerInput;
    }

    player->addToOrdersList(new Negotiate(player, enemyPlayers[--playerInput]));
}

vector<Territory *> HumanPlayerStrategy::toDefend() const
{
    return player->getTerritories();
}

vector<Territory *> HumanPlayerStrategy::toAttack() const
{
    unordered_set<Territory *> adjacentTerritories;

    for (Territory *territory : toDefend())
    {
        vector<Territory *> currentAdjacent = territory->getAdjacentTerritories();
        adjacentTerritories.insert(currentAdjacent.begin(), currentAdjacent.end());
    }
    for (Territory *territory : toDefend())
        adjacentTerritories.erase(territory);

    return vector<Territory *>(adjacentTerritories.begin(), adjacentTerritories.end());
}

string HumanPlayerStrategy::getStrategyType() const
{
    return "Human";
}

void HumanPlayerStrategy::issueDeployOrder()
{
    int reinforcementPool = player->getReinforcementPool();
    int deployedThisTurn = player->getArmiesDeployedThisTurn();

    cout << "*Issuing Deploy Order*\n"
         << player->getPlayerName() << ": " << deployedThisTurn << "/" << reinforcementPool
         << " army units deployed.\n\n";

    Territory *target = toDefend()[0];
    int armyUnits;

    if (toDefend().size() == 1)
    {
        cout << "You only control one territory: " << target->getTerritoryName()
             << "\nDeploying all " << reinforcementPool << " army units to "
             << target->getTerritoryName() << endl;
        armyUnits = reinforcementPool;
    }
    else
    {
        player->printIssuedOrders();
        cout << endl;
        printTerritoriesAndUnits(toDefend(), "Controlled");
        target = selectTerritory(toDefend(), "target");
        armyUnits = selectArmyUnits(target, "deploy to ", (reinforcementPool - deployedThisTurn));
    }
    player->setArmiesDeployedThisTurn(deployedThisTurn + armyUnits);

    player->addToOrdersList(new Deploy(player, armyUnits, target));
}

void HumanPlayerStrategy::issueAdvanceOrder()
{
    cout << "*Issuing Advance Order*\n\n";

    printTerritoriesAndUnits(toDefend(), "Controlled");
    Territory *source = selectTerritory(toDefend(), "source");

    while (source->getArmyUnits() == 0)
    {
        invalidInput(false, source->getTerritoryName() + " has no army units to advance");
        source = selectTerritory(toDefend(), "source");
    }

    string descriptor = "\nAdvancing from " + source->getTerritoryName() + ", Adjacent";
    printTerritoriesAndUnits(source->getAdjacentTerritories(), descriptor);
    Territory *target = selectTerritory(source->getAdjacentTerritories(), "target");

    descriptor = (target->getOwner() == player) ? "transfer" : "attack";
    int armyUnits = selectArmyUnits(source, descriptor + " from ");

    player->addToOrdersList(new Advance(player, armyUnits, source, target));
}

const string HumanPlayerStrategy::selectOrder()
{
    vector<string> ordersAvailable = getOrdersAvailable();

    if (ordersAvailable.size() == 1)
        return ordersAvailable[0];

    cout << *player
         << "ORDERS AVAILABLE TO ISSUE:\n\n";
    for (int i = 0; i < ordersAvailable.size(); i++)
        cout << (i + 1) << ". " << ordersAvailable[i] << endl;

    int playerInput;
    cout << "\nSelect an order (1-" << ordersAvailable.size() << ") : ";
    cin >> playerInput;

    while (playerInput > ordersAvailable.size() || playerInput < 1)
    {
        invalidInput(true, to_string(ordersAvailable.size()));
        cin >> playerInput;
    }

    cout << endl;
    return (ordersAvailable[playerInput - 1] == "Negotiate") ? "Diplomacy"
                                                             : ordersAvailable[playerInput - 1];
}

vector<string> HumanPlayerStrategy::getOrdersAvailable() const
{
    vector<string> ordersAvailable;

    if (hasUnitsOnMap())
        ordersAvailable.push_back("Advance");
    if (player->getHand()->getHandSize() != 0)
    {
        if (player->getHand()->hasAirlift() && hasUnitsOnMap())
            ordersAvailable.push_back("Airlift");
        if (player->getHand()->hasBlockade())
            ordersAvailable.push_back("Blockade");
        if (player->getHand()->hasBomb())
            ordersAvailable.push_back("Bomb");
        if (player->getHand()->hasNegotiate())
            ordersAvailable.push_back("Negotiate");
    }
    ordersAvailable.push_back("Pass");

    return ordersAvailable;
}

Territory *HumanPlayerStrategy::selectTerritory(vector<Territory *> territories,
                                                string label) const
{
    int playerInput;
    cout << "Select a " << label << " territory (1-" << territories.size() << ") : ";
    cin >> playerInput;

    while (playerInput > territories.size() || playerInput < 1)
    {
        invalidInput(true, to_string(territories.size()));
        cin >> playerInput;
    }
    return territories[playerInput - 1];
}

int HumanPlayerStrategy::selectArmyUnits(Territory *territory, string descriptor, int max) const
{
    int playerInput;
    max = (max == 0) ? territory->getArmyUnits() : max;

    cout << "Select army units to " << descriptor << territory->getTerritoryName()
         << " (1-" << max << ") : ";
    cin >> playerInput;

    while (playerInput > max || playerInput < 1)
    {
        invalidInput(true, to_string(max));
        cin >> playerInput;
    }

    return playerInput;
}

void HumanPlayerStrategy::printTerritoriesAndUnits(vector<Territory *> territories,
                                                   string label) const
{
    cout << label << " Territories:\n\n";

    for (int i = 0; i < territories.size(); i++)
    {
        cout << "    " << (i + 1) << ". " << std::left << std::setw(13)
             << territories[i]->getTerritoryName() << " : "
             << territories[i]->getArmyUnits() << " army units";

        if (territories[i]->getOwner() != player)
            cout << ", controlled by " << territories[i]->getOwner()->getPlayerName();
        cout << endl;
    }

    cout << std::endl;
}

void HumanPlayerStrategy::invalidInput(bool outOfBounds, string message) const
{
    if (outOfBounds)
        cout << "Invalid choice. Please select a valid option (1-" << message << ") : ";
    else
        cout << message << "; please select another option.\n";
}

bool HumanPlayerStrategy::inNegotiations(Player *target) const
{
    for (auto pair : *(Order::negotiations()))
        if ((pair.first == player && pair.second == target) ||
            (pair.first == target && pair.second == player))
            return true;

    return false;
}

AggressivePlayerStrategy::AggressivePlayerStrategy()
    : PlayerStrategy(), strongest(nullptr), firstOrder(true) {}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *pl)
    : PlayerStrategy(pl), strongest(nullptr), firstOrder(true) {}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

void AggressivePlayerStrategy::issueOrder()
{
    while (player->getHand()->hasReinforcement())
        player->getHand()->playCard(player, "Reinforcement");

    if (player->isFinishedIssuingOrders())
        cout << player->getPlayerName() << " has no more orders to issue.\n";
    else if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        issueDeployOrder();
    else
    {
        const string selected = selectOrder();

        if (selected == "Pass")
        {
            player->setIsFinishedIssuingOrders(true);
            firstOrder = true;
        }
        else if (selected == "Advance")
            issueAdvanceOrder();
        else
            player->getHand()->playCard(player, selected);
    }
}

vector<Territory *> AggressivePlayerStrategy::enemyBorders(Territory *territory) const
{
    vector<Territory *> enemyBorders;
    vector<Territory *> territories(toDefend());

    for (Territory *adjacent : territory->getAdjacentTerritories())
        if (find(territories.begin(), territories.end(), adjacent) == territories.end())
            enemyBorders.push_back(adjacent);

    return enemyBorders;
}

Territory *AggressivePlayerStrategy::setStrongestTerritory()
{
    strongest = toDefend()[0];

    if (toDefend().size() == 1)
        return strongest;

    int currentArmySize, strongestArmySize = strongest->getArmyUnits();
    for (int i = 1; i < toDefend().size(); i++)
    {
        currentArmySize = toDefend()[i]->getArmyUnits();

        if ((currentArmySize > strongestArmySize) ||
            (currentArmySize == strongestArmySize &&
             (enemyBorders(toDefend()[i]).size() > enemyBorders(strongest).size())))
        {
            strongest = toDefend()[i];
            strongestArmySize = currentArmySize;
        }
    }

    if (toAttack().empty())
    {
        unitsToAdvanceFromStrongest.push_back(strongest->getArmyUnits());
    }
    else
    {
        unitsToAdvanceFromStrongest = vector<int>(toAttack().size(), 0);
        for (int i = 0; i < strongest->getArmyUnits(); i++)
            unitsToAdvanceFromStrongest[i % unitsToAdvanceFromStrongest.size()]++;
    }

    return strongest;
}

void AggressivePlayerStrategy::setTerritoriesToAdvanceFrom()
{
    setStrongestTerritory();

    for (Territory *territory : toDefend())
        if (territory->getArmyUnits() != 0 && territory != strongest)
        {

            Territory *target = toAttack().empty() ? pathToStrongest(territory, pathToEnemy())
                                                   : pathToStrongest(territory, strongest);
            if (target)
            {
                toAdvanceFrom.push_back(territory);
                toAdvanceTo.push_back(target);
            }
        }

    firstOrder = false;
}

void AggressivePlayerStrategy::issueDeployOrder()
{
    if (firstOrder)
        setTerritoriesToAdvanceFrom();

    int reinforcementPool = player->getReinforcementPool();
    int deployedThisTurn = player->getArmiesDeployedThisTurn();

    cout << "*Issuing Deploy Order*\n"
         << player->getPlayerName() << ": " << deployedThisTurn << "/" << reinforcementPool
         << " army units deployed.\n\n";

    player->setArmiesDeployedThisTurn(player->getReinforcementPool());
    player->addToOrdersList(new Deploy(player, player->getReinforcementPool(), strongest));
}

void AggressivePlayerStrategy::issueAdvanceOrder()
{
    cout << "*Issuing Advance Order*\n\n";

    int unitsToAdvance;
    Territory *source = strongest;
    Territory *target = nullptr;

    if (!toAttack().empty() && !unitsToAdvanceFromStrongest.empty())
    {
        unitsToAdvance = unitsToAdvanceFromStrongest[unitsToAdvanceFromStrongest.size() - 1];
        target = toAttack()[unitsToAdvanceFromStrongest.size() - 1];
        unitsToAdvanceFromStrongest.pop_back();
    }
    else if (toAttack().empty() && !unitsToAdvanceFromStrongest.empty())
    {
        target = pathToEnemy();
        unitsToAdvance = strongest->getArmyUnits();
        unitsToAdvanceFromStrongest.pop_back();
    }
    else if (!toAdvanceFrom.empty())
    {
        source = toAdvanceFrom.back();
        target = toAdvanceTo.back();
        toAdvanceFrom.pop_back();
        toAdvanceTo.pop_back();
        unitsToAdvance = source->getArmyUnits();
    }

    player->addToOrdersList(new Advance(player, unitsToAdvance, source, target));
}

Territory *AggressivePlayerStrategy::pathToEnemy() const
{
    vector<Territory *> currentLevel{strongest};
    vector<Territory *> visited{strongest};
    vector<Territory *> previous{nullptr};

    while (!currentLevel.empty())
    {
        vector<Territory *> nextLevel;
        for (Territory *ter : currentLevel)
        {
            for (Territory *adj : ter->getAdjacentTerritories())
                if (find(visited.begin(), visited.end(), adj) == visited.end())
                {
                    previous.push_back(ter);
                    visited.push_back(adj);
                    nextLevel.push_back(adj);
                }
        }
        currentLevel = nextLevel;
    }

    int i = 2;
    Territory *destination = visited[i];
    while (destination->getOwner() == player)
        destination = previous[++i];

    while (previous[--i] != strongest)
        if (visited[i] == destination)
            destination = visited[i];

    return visited[i];
}

Territory *AggressivePlayerStrategy::pathToStrongest(Territory *source, Territory *strongest) const
{
    vector<Territory *> adjacent = source->getAdjacentTerritories();
    if (find(adjacent.begin(), adjacent.end(), strongest) != adjacent.end())
        return strongest;

    vector<Territory *> currentLevel{source};
    vector<Territory *> visited{source};
    vector<Territory *> previous{nullptr};

    // find path from source to strongest using only controlled territories
    while (!currentLevel.empty())
    {
        vector<Territory *> nextLevel;
        for (Territory *ter : currentLevel)
        {
            for (Territory *adj : ter->getAdjacentTerritories())
                if (adj->getOwner() == player && find(visited.begin(), visited.end(), adj) == visited.end())
                {
                    previous.push_back(ter);
                    visited.push_back(adj);
                    nextLevel.push_back(adj);
                }
        }
        currentLevel = nextLevel;
    }

    Territory *destination = strongest;
    int i = visited.size();
    while (previous[--i] != source)
        if (visited[i] == destination)
            destination = previous[i];

    if (destination == strongest)
    {
        currentLevel.clear();
        currentLevel.push_back(source);
        visited.clear();
        visited.push_back(source);
        previous.clear();
        previous.push_back(nullptr);

        while (!currentLevel.empty())
        {
            vector<Territory *> nextLevel;
            for (Territory *ter : currentLevel)
            {
                for (Territory *adj : ter->getAdjacentTerritories())
                    if (find(visited.begin(), visited.end(), adj) == visited.end())
                    {
                        previous.push_back(ter);
                        visited.push_back(adj);
                        nextLevel.push_back(adj);
                    }
            }
            currentLevel = nextLevel;
        }

        Territory *destination = strongest;
        int i = visited.size();
        while (previous[--i] != source)
            if (visited[i] == destination && previous[i]->getOwner() == player)
                destination = previous[i];
    }

    return (destination == strongest) ? nullptr : destination;
}

void AggressivePlayerStrategy::issueAirliftOrder()
{
    cout << "*Issuing Airlift Order*\n\n";

    int targetIndex = 0;
    Territory *source = toAdvanceFrom[targetIndex];
    Territory *target = toAdvanceTo[targetIndex];
    for (int i = 1; i < toAdvanceFrom.size(); i++)
    {
        if (toAdvanceFrom[i]->getArmyUnits() > source->getArmyUnits())
        {
            if (!toAttack().empty() && toAdvanceTo[i] != strongest)
                targetIndex = i;
            if (toAttack().empty() && toAdvanceTo[i] != pathToEnemy())
                targetIndex = i;
        }
    }

    source = toAdvanceFrom[targetIndex];
    target = toAttack().empty() ? pathToEnemy() : strongest;
    toAdvanceFrom.erase(toAdvanceFrom.begin() + targetIndex);
    toAdvanceTo.erase(toAdvanceTo.begin() + targetIndex);

    player->addToOrdersList(new Airlift(player, source->getArmyUnits(), source, target));
}

void AggressivePlayerStrategy::issueBlockadeOrder()
{
    return ;
}

void AggressivePlayerStrategy::issueBombOrder()
{
    cout << "*Issuing Bomb Order*\n\n";

    vector<Territory *> enemyTerritories;

    if (toAttack().empty())
    {
        unordered_set<Territory *> adjacentTerritories;

        for (Territory *territory : toDefend())
        {
            vector<Territory *> currentAdjacent = territory->getAdjacentTerritories();
            adjacentTerritories.insert(currentAdjacent.begin(), currentAdjacent.end());
        }
        for (Territory *territory : toDefend())
            adjacentTerritories.erase(territory);

        enemyTerritories = vector<Territory *>(adjacentTerritories.begin(), adjacentTerritories.end());
    }
    else
        enemyTerritories = toAttack();

    Territory *target = enemyTerritories[0];
    for (int i = 1; i < enemyTerritories.size(); i++)
        if (enemyTerritories[i]->getArmyUnits() > target->getArmyUnits())
            target = enemyTerritories[i];

    player->addToOrdersList(new Bomb(player, target));
}

void AggressivePlayerStrategy::issueNegotiateOrder()
{
    return;
}

vector<Territory *> AggressivePlayerStrategy::toDefend() const
{
    return player->getTerritories();
}

vector<Territory *> AggressivePlayerStrategy::toAttack() const
{
    return enemyBorders(strongest);
}

string AggressivePlayerStrategy::getStrategyType() const
{
    return "Aggressive";
}

const string AggressivePlayerStrategy::selectOrder()
{
    vector<string> ordersAvailable(getOrdersAvailable());

    if (ordersAvailable.size() == 1)
        return ordersAvailable[0];

    cout << *player
         << "ORDERS AVAILABLE TO ISSUE:\n\n";
    for (int i = 0; i < ordersAvailable.size(); i++)
        cout << (i + 1) << ". " << ordersAvailable[i] << endl;
    cout << endl;

    return ordersAvailable[0];
}

vector<string> AggressivePlayerStrategy::getOrdersAvailable() const
{
    vector<string> ordersAvailable;

    if (player->getHand()->getHandSize() != 0)
    {
        if (player->getHand()->hasBomb())
            ordersAvailable.push_back("Bomb");
        if (player->getHand()->hasAirlift() && hasUnitsOnMap() && !toAdvanceFrom.empty())
            ordersAvailable.push_back("Airlift");
    }
    if (hasUnitsOnMap() && (!unitsToAdvanceFromStrongest.empty() || !toAdvanceFrom.empty()))
    {
        ordersAvailable.push_back("Advance");
    }

    ordersAvailable.push_back("Pass");

    return ordersAvailable;
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *pl) : PlayerStrategy(pl) {}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

void BenevolentPlayerStrategy::issueOrder()
{
    cout << *player << endl;

    if (toDefend().size() == 0)
    {
        player->setIsFinishedIssuingOrders(true);
        return;
    }

    if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        issueDeployOrder();
    else if (player->getOrdersList()->size() == 0)
        issueAdvanceOrder();
    else if (hasBenevolentCard())
    {
        cout << "playing a nice card" << endl;
        playBenevolentCard();
        player->setIsFinishedIssuingOrders(true);
    }
    else 
        player->setIsFinishedIssuingOrders(true);
}

bool BenevolentPlayerStrategy::hasBenevolentCard()
{
    return player->getHand()->hasAirlift() || player->getHand()->hasDiplomacy() || player->getHand()->hasReinforcement();
}

void BenevolentPlayerStrategy::playBenevolentCard()
{
    if (player->getHand()->hasAirlift())
        player->getHand()->playCard(player, "Airlift");
    else if (player->getHand()->hasDiplomacy())
        player->getHand()->playCard(player, "Diplomacy");
    else if (player->getHand()->hasReinforcement())
        player->getHand()->playCard(player, "Reinforcement");
}

void BenevolentPlayerStrategy::issueDeployOrder()
{
    const int deployedThisTurn = player->getArmiesDeployedThisTurn();
    const int reinforcementPool = player->getReinforcementPool();

    cout << "Issuing a Deploy order." << endl;
	int remainingUnitsToDeploy = reinforcementPool - deployedThisTurn;
	cout << remainingUnitsToDeploy << " armies remain available in their reinforcement pool." << endl;

	int armiesToDeploy = remainingUnitsToDeploy;

    // deploy half the remaining units unless there are less than 10 remaining
	if (remainingUnitsToDeploy > 10)
	{
		armiesToDeploy = remainingUnitsToDeploy / 2;
	}

    //target territory based on how many orders have been issued already
	Territory *territoryToDefend = toDefend()[player->getOrdersList()->size()];
	cout << "Deploying " << armiesToDeploy << " to " << territoryToDefend->getTerritoryName() << endl;

    player->setArmiesDeployedThisTurn(deployedThisTurn + armiesToDeploy);
    player->addToOrdersList(new Deploy(player, armiesToDeploy, territoryToDefend));
}

void BenevolentPlayerStrategy::issueAdvanceOrder()
{
    cout << "Issuing an Advance order." << endl;

	Territory *targetTerritory = nullptr;
	Territory *sourceTerritory = nullptr;

    for (int i = 0; i < toDefend().size(); i++)
    {
        targetTerritory = toDefend()[i];

        vector<Territory*> adjacentOwnedTerritories = {};
        
        // find a territory adjacent to the target that's owned by the player
        for (Territory *territoryAdjacentToTarget : targetTerritory->getAdjacentTerritories())
        {
            vector<Territory *> territoriesToDefend = toDefend();
            if (find_if(territoriesToDefend.begin(), territoriesToDefend.end(), [territoryAdjacentToTarget](Territory *territory)
                        { return territory->getTerritoryName() == territoryAdjacentToTarget->getTerritoryName(); }) == territoriesToDefend.end())
            {
                continue;
            }
            adjacentOwnedTerritories.push_back(territoryAdjacentToTarget);
        }

        if (adjacentOwnedTerritories.size() == 0)
        {
            break;
        }

        // of the owned adjacent territories, sort to find the one with the most armies
        sort(adjacentOwnedTerritories.begin(), adjacentOwnedTerritories.end(),
        [](const Territory *x, const Territory *y) {
            return x->getArmyUnits() > y->getArmyUnits();
        });

        sourceTerritory = adjacentOwnedTerritories[0];
    }

	// balance the number of armies between the two territories
	int units = (sourceTerritory->getArmyUnits() - targetTerritory->getArmyUnits()) / 2;

	cout << "Advancing " << units << " to " << targetTerritory->getTerritoryName() << " from " << sourceTerritory->getTerritoryName() << endl;

	player->addToOrdersList(new Advance(player, units, sourceTerritory, targetTerritory));
}

void BenevolentPlayerStrategy::issueAirliftOrder()
{
	cout << "Issuing an Airlift order." << endl;

	// get random territory from toAttack and toDefend
	Territory *sourceTerritory = toDefend()[toDefend().size()-1];
	Territory *targetTerritory = toDefend()[0];

	// determine how many armies
	int units = (sourceTerritory->getArmyUnits() - targetTerritory->getArmyUnits()) / 2;

	cout << "Airlifting " << units << " from " << sourceTerritory->getTerritoryName() << " to " << targetTerritory->getTerritoryName() << endl;

	player->addToOrdersList(new Airlift(player, units, sourceTerritory, targetTerritory));
}
void BenevolentPlayerStrategy::issueBlockadeOrder()
{
    //no-op
}
void BenevolentPlayerStrategy::issueBombOrder()
{
    //no-op
}
void BenevolentPlayerStrategy::issueNegotiateOrder()
{
	cout << "Issuing a Negotiate order." << endl;

	Player *targetPlayer = nullptr;

	for (Player *otherPlayer : GameEngine::getPlayers())
	{
		if (otherPlayer->getPlayerName() != player->getPlayerName())
		{
			targetPlayer = player;
			break;
		}
	}

	cout << "Negotiating with " << targetPlayer->getPlayerName() << endl;

	player->addToOrdersList(new Negotiate(player, targetPlayer));
}

vector<Territory *> BenevolentPlayerStrategy::toDefend() const
{
    vector<Territory*> sortedTerritories = player->getTerritories();

    sort(sortedTerritories.begin(), sortedTerritories.end(),
            [](const Territory *x, const Territory *y) {
                return x->getArmyUnits() < y->getArmyUnits();
            });
    
    return sortedTerritories;
}

vector<Territory *> BenevolentPlayerStrategy::toAttack() const
{
    return {};
}

string BenevolentPlayerStrategy::getStrategyType() const
{
    return "Benevolent";
}

NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *pl) : PlayerStrategy(pl) {}

NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

void NeutralPlayerStrategy::issueOrder()
{
    cout << *player << endl;

    player->setIsFinishedIssuingOrders(true);
}

void NeutralPlayerStrategy::issueDeployOrder()
{
    // no-op
}

void NeutralPlayerStrategy::issueAdvanceOrder()
{
    // no-op
}

void NeutralPlayerStrategy::issueAirliftOrder()
{
    // no-op
}
void NeutralPlayerStrategy::issueBlockadeOrder()
{
    // no-op
}
void NeutralPlayerStrategy::issueBombOrder()
{
    // no-op
}
void NeutralPlayerStrategy::issueNegotiateOrder()
{
    // no-op
}

vector<Territory *> NeutralPlayerStrategy::toDefend() const
{
    return {};
}

vector<Territory *> NeutralPlayerStrategy::toAttack() const
{
    return {};
}

string NeutralPlayerStrategy::getStrategyType() const
{
    return "Neutral";
}

CheaterPlayerStrategy::CheaterPlayerStrategy() : PlayerStrategy() {}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player *pl) : PlayerStrategy(pl) {}

CheaterPlayerStrategy::~CheaterPlayerStrategy() {}

void CheaterPlayerStrategy::issueOrder()
{
    cout << *player << endl;
    
    for(Territory* territory : toAttack())
    {
        territory->getOwner()->removeTerritory(territory);
        player->addTerritory(territory);

        Order::playerEarnedCard(player);
    }

    player->setIsFinishedIssuingOrders(true);
    
}

void CheaterPlayerStrategy::issueDeployOrder()
{
    // no-op
}

void CheaterPlayerStrategy::issueAdvanceOrder()
{
    // no-op
}

void CheaterPlayerStrategy::issueAirliftOrder()
{
    // no-op
}
void CheaterPlayerStrategy::issueBlockadeOrder()
{
    // no-op
}
void CheaterPlayerStrategy::issueBombOrder()
{
    // no-op
}
void CheaterPlayerStrategy::issueNegotiateOrder()
{
    // no-op
}

vector<Territory *> CheaterPlayerStrategy::toDefend() const
{
    return player->getTerritories();
}

vector<Territory *> CheaterPlayerStrategy::toAttack() const
{
    vector<Territory *> allAdjacentTerritories;

    for (Territory *territory : player->getTerritories())
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
    for (int i = 0; i < player->getTerritories().size(); i++)
    {
        for (int j = 0; j < allAdjacentTerritories.size();)
        {
            if (player->getTerritories()[i]->getTerritoryName() ==
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

string CheaterPlayerStrategy::getStrategyType() const
{
    return "Cheater";
}
