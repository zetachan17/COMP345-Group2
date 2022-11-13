#include "Map/Map.h"
#include "Player/Player.h"
#include "Orders/Orders.h"
#include "GameEngine/GameEngine.h"
class Territory;

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

void testOrderExecution()
{
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n"
          << "\t** PART 4: ORDER EXECUTION IMPLEMENTATION **\n\n";

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

     cout << "----------------------------------------------------------\n"
          << "*CREATED TEST TERRITORIES*\n\n";
     for (Territory *territory : territories)
          cout << *territory;
     cout << "----------------------------------------------------------\n";

     Player *tina = new Player("Tina");
     Player *eugene = new Player("Eugene");
     Player *louise = new Player("Louise");

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
     tina->issueOrder(new Deploy(tina, 10, territories[4]));
     louise->issueOrder(new Deploy(louise, 15, territories[3]));
     eugene->issueOrder(new Deploy(eugene, 10, territories[0]));
     tina->issueOrder(new Deploy(tina, 30, territories[5]));
     louise->issueOrder(new Deploy(louise, 20, territories[1]));
     eugene->issueOrder(new Deploy(eugene, 40, territories[3]));
     tina->issueOrder(new Deploy(tina, 20, territories[6]));
     louise->issueOrder(new Deploy(louise, 30, territories[2]));

     // issue test advance orders
     eugene->issueOrder(new Advance(eugene, 35, territories[3], territories[4]));
     tina->issueOrder(new Advance(tina, 20, territories[5], territories[6]));
     louise->issueOrder(new Advance(louise, 7, territories[2], territories[4]));
     eugene->issueOrder(new Advance(eugene, 5, territories[4], territories[0]));
     tina->issueOrder(new Advance(tina, 4, territories[4], territories[5]));
     louise->issueOrder(new Advance(louise, 22, territories[2], territories[3]));

     // issue test airlift orders
     eugene->issueOrder(new Airlift(eugene, 6, territories[4], territories[0]));
     tina->issueOrder(new Airlift(tina, 2, territories[4], territories[6]));
     louise->issueOrder(new Airlift(louise, 5, territories[3], territories[5]));

     // issue test bomb orders
     eugene->issueOrder(new Bomb(eugene, territories[2]));
     tina->issueOrder(new Bomb(tina, territories[1]));

     // issue test blockade orders
     louise->issueOrder(new Blockade(louise, territories[3]));
     eugene->issueOrder(new Blockade(eugene, territories[1]));
     tina->issueOrder(new Blockade(tina, territories[5]));

     // issue and test negotiate order
     louise->issueOrder(new Negotiate(louise, eugene));
     eugene->issueOrder(new Advance(eugene, 4, territories[4], territories[2]));
     tina->issueOrder(new Advance(tina, 12, territories[6], territories[3]));
     louise->issueOrder(new Bomb(louise, territories[0]));

     cout << "*CREATED TEST PLAYERS*\n"
          << "*ASSIGNED TERRITORIES*\n"
          << "*ISSUED ORDERS TO TEST*\n\n"
          << *tina
          << "Player's reinforcement pool : " << tina->getReinforcementPool() << endl
          << "----------------------------------------------------------\n"
          << *eugene
          << "Player's reinforcement pool : " << eugene->getReinforcementPool() << endl
          << "----------------------------------------------------------\n"
          << *louise
          << "Player's reinforcement pool : " << louise->getReinforcementPool() << endl
          << "----------------------------------------------------------\n";

     // test execution of deploy orders
     Order *next = tina->nextOrder();
     cout << "\n\t\t**EXECUTE DEPLOY ORDERS**\n\n"
          << "----------------------------------------------------------\n";
     cout << tina->getPlayerName() << "'s next order: " << *next << endl;
     next->execute();
     cout << endl;
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";
     cout << "*PLAYERS' REINFORCEMENT POOL & TERRITORY UNITS AFTER DEPLOYS*\n"
          << tina->getPlayerName()
          << "'s reinforcement pool : " << tina->getReinforcementPool() << endl
          << eugene->getPlayerName()
          << "'s reinforcement pool : " << eugene->getReinforcementPool() << endl
          << louise->getPlayerName()
          << "'s reinforcement pool : " << louise->getReinforcementPool() << endl;
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(12) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test execution of advance orders
     cout << "\n\t\t**EXECUTE ADVANCE ORDERS**\n\n"
          << "----------------------------------------------------------\n";
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";
     cout << "*TERRITORY UNITS AFTER ADVANCE ORDERS*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(12) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << *tina
          << "----------------------------------------------------------\n"
          << *eugene
          << "----------------------------------------------------------\n"
          << *louise
          << "----------------------------------------------------------\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(12) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test airlift orders
     cout << "\n\t\t**EXECUTE AIRLIFT ORDERS**\n\n"
          << "----------------------------------------------------------\n";
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";
     cout << "*TERRITORY UNITS AFTER AIRLIFT ORDERS*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(12) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test bomb orders
     cout << "\n\t\t**EXECUTE BOMB ORDERS**\n\n"
          << "----------------------------------------------------------\n";
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";
     cout << "*TERRITORY UNITS AFTER BOMB ORDERS*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(12) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test blockade orders
     cout << "\n\t\t**EXECUTE BLOCKADE ORDERS**\n\n"
          << "----------------------------------------------------------\n";
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";
     cout << "*TERRITORY UNITS & PLAYERS AFTER BLOCKADE ORDERS*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(12) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << *tina
          << "----------------------------------------------------------\n"
          << *eugene
          << "----------------------------------------------------------\n"
          << *louise
          << "----------------------------------------------------------\n"
          << *Order::neutralPlayer()
          << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test negotiate orders
     cout << "\n\t  **EXECUTE & TEST NEGOTIATE ORDER**\n\n"
          << "----------------------------------------------------------\n";
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
     next->execute();
     cout << endl
          << "*TEST ATTACKING THE NEUTRAL PLAYER*\n";
     cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << louise->getPlayerName() << "'s next order: " << *(next = louise->nextOrder()) << endl;
     next->execute();
     cout << endl;
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";
     cout << "*TERRITORY UNITS & PLAYERS AFTER ALL ORDER EXECUTIONS*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(12) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << *tina
          << "----------------------------------------------------------\n"
          << *eugene
          << "----------------------------------------------------------\n"
          << *louise
          << "----------------------------------------------------------\n"
          << *Order::neutralPlayer()
          << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";
}
