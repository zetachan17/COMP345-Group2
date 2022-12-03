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

PlayerStrategy::PlayerStrategy() : player(nullptr) {}

PlayerStrategy::PlayerStrategy(Player *pl) : player(pl) {}

PlayerStrategy::~PlayerStrategy() {}

void PlayerStrategy::setPlayer(Player *pl)
{
    player = pl;
}

HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}

HumanPlayerStrategy::HumanPlayerStrategy(Player *pl) : PlayerStrategy(pl) {}

HumanPlayerStrategy::~HumanPlayerStrategy() {}

void HumanPlayerStrategy::issueOrder()
{
    if (player->isFinishedIssuingOrders())
        cout << player->getPlayerName() << " has no more orders to issue.\n";
    else if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        issueDeployOrder(player->getArmiesDeployedThisTurn(), player->getReinforcementPool());
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

    printTerritoriesAndUnits(player->getTerritories(), "Controlled");
    Territory *source = selectTerritory(player->getTerritories(), "source");

    while (source->getArmyUnits() == 0)
    {
        invalidInput(false, source->getTerritoryName() + " has no army units to airlift");
        source = selectTerritory(player->getTerritories(), "source");
    }

    Territory *target = selectTerritory(player->getTerritories(), "target");

    while (target == source)
    {
        invalidInput(false, "Unauthorized to airlift units to and from the same territory");
        target = selectTerritory(player->getTerritories(), "target");
    }

    int armyUnits = selectArmyUnits(source, "airlift from ");

    player->addToOrdersList(new Airlift(player, armyUnits, source, target));
}

void HumanPlayerStrategy::issueBlockadeOrder()
{
    cout << "*Issuing Blockade Order*\n\n";

    printTerritoriesAndUnits(player->getTerritories(), "Controlled");
    Territory *target = selectTerritory(player->getTerritories(), "target");

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

    for (Territory *territory : player->getTerritories())
    {
        vector<Territory *> currentAdjacent = territory->getAdjacentTerritories();
        adjacentTerritories.insert(currentAdjacent.begin(), currentAdjacent.end());
    }
    for (Territory *territory : player->getTerritories())
        adjacentTerritories.erase(territory);

    return vector<Territory *>(adjacentTerritories.begin(), adjacentTerritories.end());
}

string HumanPlayerStrategy::getStrategyType() const
{
    return "Human";
}

void HumanPlayerStrategy::issueDeployOrder(const int deployedThisTurn, const int reinforcementPool)
{
    cout << "*Issuing Deploy Order*\n"
         << player->getPlayerName() << ": " << deployedThisTurn << "/" << reinforcementPool
         << " army units deployed.\n\n";

    Territory *target = player->getTerritories()[0];
    int armyUnits;

    if (player->getTerritories().size() == 1)
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
        printTerritoriesAndUnits(player->getTerritories(), "Controlled");
        target = selectTerritory(player->getTerritories(), "target");
        armyUnits = selectArmyUnits(target, "deploy to ", (reinforcementPool - deployedThisTurn));
    }
    player->setArmiesDeployedThisTurn(deployedThisTurn + armyUnits);

    player->addToOrdersList(new Deploy(player, armyUnits, target));
}

void HumanPlayerStrategy::issueAdvanceOrder()
{
    cout << "*Issuing Advance Order*\n\n";

    printTerritoriesAndUnits(player->getTerritories(), "Controlled");
    Territory *source = selectTerritory(player->getTerritories(), "source");

    while (source->getArmyUnits() == 0)
    {
        invalidInput(false, source->getTerritoryName() + " has no army units to advance");
        source = selectTerritory(player->getTerritories(), "source");
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
    const vector<string> ordersAvailable = getOrdersAvailable();

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

const vector<string> HumanPlayerStrategy::getOrdersAvailable() const
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

bool HumanPlayerStrategy::hasUnitsOnMap() const
{
    for (Territory *territory : player->getTerritories())
        if (territory->getArmyUnits() != 0)
            return true;

    return false;
}

bool HumanPlayerStrategy::inNegotiations(Player *target) const
{
    for (auto pair : *(Order::negotiations()))
        if ((pair.first == player && pair.second == target) ||
            (pair.first == target && pair.second == player))
            return true;

    return false;
}

AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy() {}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *pl) : PlayerStrategy(pl) {}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

void AggressivePlayerStrategy::issueOrder()
{
    cout << *player << endl;

    if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        player->issueDeployOrder();
    else
    {
        int randomChoice = rand() % (100);

        if (randomChoice <= 25)
            player->issueAdvanceOrder();
        else if (randomChoice <= 75)
        {
            if (player->getHand()->getHandSize() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
}

void AggressivePlayerStrategy::issueAirliftOrder()
{
    player->issueAirliftOrder();
}
void AggressivePlayerStrategy::issueBlockadeOrder()
{
    player->issueBlockadeOrder();
}
void AggressivePlayerStrategy::issueBombOrder()
{
    player->issueBombOrder();
}
void AggressivePlayerStrategy::issueNegotiateOrder()
{
    player->issueNegotiateOrder();
}

vector<Territory *> AggressivePlayerStrategy::toDefend() const
{
    return player->getTerritories();
}

vector<Territory *> AggressivePlayerStrategy::toAttack() const
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

string AggressivePlayerStrategy::getStrategyType() const
{
    return "Aggressive";
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *pl) : PlayerStrategy(pl) {}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

void BenevolentPlayerStrategy::issueOrder()
{
    cout << *player << endl;

    if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        player->issueDeployOrder();
    else
    {
        int randomChoice = rand() % (100);

        if (randomChoice <= 25)
            player->issueAdvanceOrder();
        else if (randomChoice <= 75)
        {
            if (player->getHand()->getHandSize() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
}

void BenevolentPlayerStrategy::issueAirliftOrder()
{
    player->issueAirliftOrder();
}
void BenevolentPlayerStrategy::issueBlockadeOrder()
{
    player->issueBlockadeOrder();
}
void BenevolentPlayerStrategy::issueBombOrder()
{
    player->issueBombOrder();
}
void BenevolentPlayerStrategy::issueNegotiateOrder()
{
    player->issueNegotiateOrder();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend() const
{
    return player->getTerritories();
}

vector<Territory *> BenevolentPlayerStrategy::toAttack() const
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

    if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        player->issueDeployOrder();
    else
    {
        int randomChoice = rand() % (100);

        if (randomChoice <= 25)
            player->issueAdvanceOrder();
        else if (randomChoice <= 75)
        {
            if (player->getHand()->getHandSize() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
}

void NeutralPlayerStrategy::issueAirliftOrder()
{
    player->issueAirliftOrder();
}
void NeutralPlayerStrategy::issueBlockadeOrder()
{
    player->issueBlockadeOrder();
}
void NeutralPlayerStrategy::issueBombOrder()
{
    player->issueBombOrder();
}
void NeutralPlayerStrategy::issueNegotiateOrder()
{
    player->issueNegotiateOrder();
}

vector<Territory *> NeutralPlayerStrategy::toDefend() const
{
    return player->getTerritories();
}

vector<Territory *> NeutralPlayerStrategy::toAttack() const
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

    if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        player->issueDeployOrder();
    else
    {
        int randomChoice = rand() % (100);

        if (randomChoice <= 25)
            player->issueAdvanceOrder();
        else if (randomChoice <= 75)
        {
            if (player->getHand()->getHandSize() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
}

void CheaterPlayerStrategy::issueAirliftOrder()
{
    player->issueAirliftOrder();
}
void CheaterPlayerStrategy::issueBlockadeOrder()
{
    player->issueBlockadeOrder();
}
void CheaterPlayerStrategy::issueBombOrder()
{
    player->issueBombOrder();
}
void CheaterPlayerStrategy::issueNegotiateOrder()
{
    player->issueNegotiateOrder();
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
