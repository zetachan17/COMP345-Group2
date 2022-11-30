#include "PlayerStrategies/PlayerStrategiesDriver.h"
#include "PlayerStrategies/PlayerStrategies.h"
#include "Map/Map.h"
#include "GameEngine/GameEngine.h"
#include "Player/Player.h"

#include <iostream>
using std::cout;
using std::endl;

void testPlayerStrategies()
{
    cout << "-----------------------------------------------------------\n"
         << "\t** PART 1: PLAYER STRATEGY PATTERN **\n\n";

    // setup game engine (just to test access to the deck), territories, and players to test
    GameEngine *game = new GameEngine();

    Territory *territories[7] = {new Territory(0, "Villeray", 4), new Territory(1, "Little Italy", 4),
                                 new Territory(2, "Mile End", 3), new Territory(3, "Plateau", 3),
                                 new Territory(4, "Mount Royal", 2), new Territory(5, "Downtown", 1),
                                 new Territory(6, "The Village", 1)};

    territories[0]->addAdjacentTerritory(territories[1]);
    territories[1]->addAdjacentTerritory(territories[0]);
    territories[1]->addAdjacentTerritory(territories[2]);
    territories[2]->addAdjacentTerritory(territories[1]);
    territories[2]->addAdjacentTerritory(territories[3]);
    territories[3]->addAdjacentTerritory(territories[2]);
    territories[3]->addAdjacentTerritory(territories[4]);
    territories[4]->addAdjacentTerritory(territories[3]);
    territories[3]->addAdjacentTerritory(territories[6]);
    territories[6]->addAdjacentTerritory(territories[3]);
    territories[4]->addAdjacentTerritory(territories[5]);
    territories[5]->addAdjacentTerritory(territories[4]);
    territories[5]->addAdjacentTerritory(territories[6]);
    territories[6]->addAdjacentTerritory(territories[5]);
    territories[2]->addAdjacentTerritory(territories[4]);
    territories[4]->addAdjacentTerritory(territories[2]);

    cout << "-----------------------------------------------------------\n"
         << "*CREATED TEST TERRITORIES*\n\n";
    for (Territory *territory : territories)
        cout << *territory;
    cout << "-----------------------------------------------------------\n";

    Player *tina = new Player("Tina", new HumanPlayerStrategy);
    Player *eugene = new Player("Eugene", new HumanPlayerStrategy);
    Player *louise = new Player("Louise", new HumanPlayerStrategy);

    // assign territories for testing
    tina->addTerritory(territories[4]);
    tina->addTerritory(territories[5]);
    tina->addTerritory(territories[6]);
    eugene->addTerritory(territories[0]);
    eugene->addTerritory(territories[3]);
    louise->addTerritory(territories[1]);
    louise->addTerritory(territories[2]);

    tina->addReinforcements(50);
    eugene->addReinforcements(50);
    louise->addReinforcements(50);

    // issue test deploy orders
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
    tina->issueOrder();
    louise->issueOrder();
    eugene->issueOrder();
}
