#include "PlayerStrategies/PlayerStrategies.h"
#include "Player/Player.h"
#include "Map/Map.h"
#include "Orders/Orders.h"
#include "Cards/Cards.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <string>
using std::to_string;

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
            if (player->getHand()->getCards().size() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
}

vector<Territory *> HumanPlayerStrategy::toDefend() const
{
    return player->getTerritories();
}

vector<Territory *> HumanPlayerStrategy::toAttack() const
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

string HumanPlayerStrategy::getStrategyType() const
{
    return "Human";
}

void HumanPlayerStrategy::issueDeployOrder(const int deployedThisTurn, const int reinforcementPool)
{
    vector<Territory *> territories = player->getTerritories();
    Territory *target = territories[0];
    int input, numberOfTerritories = territories.size();

    cout << deployedThisTurn << " / " << reinforcementPool << " army units deployed.\n";

    if (numberOfTerritories == 1)
    {
        cout << "You only control 1 territory: " << target->getTerritoryName()
             << "\nDeploying all " << reinforcementPool << " army units to "
             << target->getTerritoryName() << endl;

        input = reinforcementPool;
    }
    else
    {
        cout << "Where to deploy?\n\n";
        for (int i = 0; i < numberOfTerritories; i++)
            cout << (i + 1) << ". " << territories[i]->getTerritoryName() << endl;

        cout << "\nSelect a territory (1-" << numberOfTerritories << ") : ";
        cin >> input;
        while (input > numberOfTerritories && input < 1)
        {
            cout << "Invalid input. Please select a territory from the list above (1-"
                 << numberOfTerritories << ") : ";
            cin >> input;
        }

        target = territories[input - 1];

        int availableToDeploy = reinforcementPool - deployedThisTurn;
        cout << "Army units to deploy (1-" << availableToDeploy << ") : ";
        cin >> input;
        while (input > availableToDeploy && input < 1)
        {
            cout << "Invalid input. Please select a valid number of army units to deploy (1-"
                 << availableToDeploy << ") : ";
            cin >> input;
        }
    }

    Order *deploy = new Deploy(player, input, target);
    player->addToOrdersList(deploy);
    cout << "Issued Order: " << *deploy;
    player->setArmiesDeployedThisTurn(deployedThisTurn + input);
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
            if (player->getHand()->getCards().size() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
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
            if (player->getHand()->getCards().size() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
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
            if (player->getHand()->getCards().size() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
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
            if (player->getHand()->getCards().size() != 0)
                player->playCard();
            else
                player->issueAdvanceOrder();
        }
        else
            player->setIsFinishedIssuingOrders(true);
    }
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
