#include "PlayerStrategies/PlayerStrategies.h"
#include "Player/Player.h"
#include "Map/Map.h"
#include "Orders/Orders.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <string>
using std::to_string;

PlayerStrategy::PlayerStrategy() : player(nullptr), strategyType("") {}

PlayerStrategy::PlayerStrategy(Player *pl, const string &type) : player(pl), strategyType(type) {}

PlayerStrategy::~PlayerStrategy() {}

string PlayerStrategy::getStrategyType() const
{
    return strategyType;
}

HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}

HumanPlayerStrategy::HumanPlayerStrategy(Player *pl) : PlayerStrategy(pl, "Human") {}

HumanPlayerStrategy::~HumanPlayerStrategy() {}

void HumanPlayerStrategy::issueOrder()
{
    if (player->getArmiesDeployedThisTurn() < player->getReinforcementPool())
        issueDeployOrder(player->getArmiesDeployedThisTurn(), player->getReinforcementPool());
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

AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy()
{
}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *pl)
    : PlayerStrategy(pl, "Aggressive") {}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *pl)
    : PlayerStrategy(pl, "Benevolent") {}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *pl) : PlayerStrategy(pl, "Neutral") {}

NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

CheaterPlayerStrategy::CheaterPlayerStrategy() : PlayerStrategy() {}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player *pl) : PlayerStrategy(pl, "Cheater") {}

CheaterPlayerStrategy::~CheaterPlayerStrategy() {}