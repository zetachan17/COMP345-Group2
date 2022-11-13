#include "Map/Map.h"
#include "Player/Player.h"
#include "Orders/Orders.h"
#include "GameEngine/GameEngine.h"
class Territory;

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

// clang++ -std=c++11 test_Orders.cpp OrdersDriver.cpp Map.cpp Player.cpp Orders.cpp Cards.cpp

void testOrderExecution()
{
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n"
          << "** PART 4: ORDER EXECUTION IMPLEMENTATION **\n\n";

     GameEngine *game = new GameEngine();

     Territory *territories[8] = {new Territory(0, "Eritrea", 4), new Territory(1, "Chile", 1),
                                  new Territory(2, "Croatia", 2), new Territory(3, "Australia", 3),
                                  new Territory(4, "Ecuador", 1), new Territory(5, "Palau", 3),
                                  new Territory(6, "Cambodia", 5), new Territory(7, "Guatemala", 1)};

     territories[3]->addAdjacentTerritory(territories[5]);
     territories[5]->addAdjacentTerritory(territories[3]);
     territories[4]->addAdjacentTerritory(territories[7]);
     territories[7]->addAdjacentTerritory(territories[4]);
     territories[4]->addAdjacentTerritory(territories[1]);
     territories[1]->addAdjacentTerritory(territories[4]);
     territories[3]->addAdjacentTerritory(territories[6]);
     territories[6]->addAdjacentTerritory(territories[3]);
     territories[6]->addAdjacentTerritory(territories[5]);
     territories[5]->addAdjacentTerritory(territories[6]);

     cout << "----------------------------------------------------------\n"
          << "*Created test territories*\n\n";
     for (Territory *territory : territories)
          cout << *territory;
     cout << "----------------------------------------------------------\n";

     Player *tristan = new Player("Tristan");
     tristan->addTerritory(territories[0]);
     tristan->addTerritory(territories[1]);
     tristan->addTerritory(territories[5]);
     tristan->addTerritory(territories[6]);
     tristan->addReinforcements(10);

     Player *zouzou = new Player("Zouzou");
     zouzou->addTerritory(territories[2]);
     zouzou->addTerritory(territories[3]);

     Player *claire = new Player("Claire");
     claire->addTerritory(territories[4]);
     claire->addTerritory(territories[7]);

     Order *o1 = new Deploy(tristan, 4, territories[1]);
     Order *o2 = new Deploy(tristan, 6, territories[4]);
     Order *o3 = new Blockade(tristan, territories[1]);
     Order *o4 = new Blockade(tristan, territories[2]);
     Order *o5 = new Blockade(tristan, territories[0]);
     Order *o6 = new Bomb(tristan, territories[2]);
     Order *o7 = new Bomb(tristan, territories[3]);
     Order *o8 = new Negotiate(tristan, zouzou);
     Order *o9 = new Bomb(tristan, territories[3]);
     Order *o10 = new Bomb(zouzou, territories[6]);
     Order *o11 = new Advance(claire, 5, territories[4], territories[1]);
     Order *o12 = new Advance(tristan, 3, territories[5], territories[3]);
     Order *o13 = new Advance(claire, 14, territories[7], territories[4]);
     Order *o14 = new Advance(claire, 13, territories[4], territories[1]);
     Order *o15 = new Airlift(zouzou, 2, territories[2], territories[3]);
     Order *o16 = new Airlift(tristan, 7, territories[6], territories[0]);
     tristan->issueOrder(o1);
     tristan->issueOrder(o2);
     tristan->issueOrder(o3);
     tristan->issueOrder(o4);
     tristan->issueOrder(o5);
     tristan->issueOrder(o6);
     tristan->issueOrder(o7);
     tristan->issueOrder(o8);
     tristan->issueOrder(o9);
     tristan->issueOrder(o12);
     zouzou->issueOrder(o10);
     claire->issueOrder(o11);
     claire->issueOrder(o13);
     claire->issueOrder(o14);
     zouzou->issueOrder(o15);
     tristan->issueOrder(o16);

     cout << "*Created test player*\n"
          << "*Assigned 'Eritrea', 'Chile', 'Palau' to player*\n"
          << "*Added 10 units to player's reinforcement pool*\n"
          << "*Issued test orders*\n\n"
          << *tristan
          << "Player's reinforcement pool : " << tristan->getReinforcementPool() << endl
          << "----------------------------------------------------------\n";

     territories[3]->setArmyUnits(5);
     territories[0]->setArmyUnits(7);
     territories[2]->setArmyUnits(3);
     territories[6]->setArmyUnits(10);
     territories[4]->setArmyUnits(6);
     territories[5]->setArmyUnits(4);
     territories[7]->setArmyUnits(15);

     cout << "*Assigned random army units for testing*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test execution of deploy orders
     Order *next = tristan->nextOrder();
     cout << "\n\t\t**Execute Deploy Orders**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();
     cout << "Effect: ";
     (next = tristan->nextOrder())->execute();

     cout << "\n*Player & Territory units after deploy orders*\n"
          << "\tPlayer's reinforcement pool : " << tristan->getReinforcementPool() << endl;
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test execution of first blockade order
     next = tristan->nextOrder();
     cout << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();

     cout << "\n*Territory units after order execution*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "*Neutral player has been created*\n\n"
          << *Order::neutralPlayer()
          << "----------------------------------------------------------\n"
          << "*Player status after execution*\n\n"
          << *tristan
          << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test execution of second blockade order
     next = tristan->nextOrder();
     cout << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();

     cout << "\n*Territory units after order execution*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "*Neutral player after execution*\n\n"
          << *Order::neutralPlayer()
          << "----------------------------------------------------------\n"
          << "*Player status after execution*\n\n"
          << *tristan
          << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test execution of third blockade order
     next = tristan->nextOrder();
     cout << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();

     cout << "\n*Territory units after order execution*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "*Neutral player after execution*\n\n"
          << *Order::neutralPlayer()
          << "----------------------------------------------------------\n"
          << "*Player status after execution*\n\n"
          << *tristan
          << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test execution of first bomb order
     next = tristan->nextOrder();
     cout << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();

     cout << "\n*Territory units after order execution*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     // test execution of second bomb order
     next = tristan->nextOrder();
     cout << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();

     cout << "\n*Territory units after order execution*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     cout << *zouzou
          << "----------------------------------------------------------\n";

     next = tristan->nextOrder();
     cout << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();

     next = tristan->nextOrder();
     cout << "----------------------------------------------------------\n"
          << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();
     cout << "Effect: ";
     (next = zouzou->nextOrder())->execute();

     cout << "\n*Player & Territory units after deploy orders*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     //
     // TEST ADVANCE ORDERS
     next = claire->nextOrder();
     cout << "----------------------------------------------------------\n"
          << "\n      **Execute next order: " << *next << "**\n\n"
          << "----------------------------------------------------------\n";

     cout << "Effect: ";
     next->execute();

     cout << "\nNext order: " << *(next = tristan->nextOrder()) << "\nEffect: ";
     next->execute();

     cout << "\nNext order: " << *(next = claire->nextOrder()) << "\nEffect: ";
     next->execute();

     cout << "\nNext order: " << *(next = claire->nextOrder()) << "\nEffect: ";
     next->execute();

     cout << "----------------------------------------------------------\n"
          << "----------------------------------------------------------\n";

     cout << "\n*Player & Territory units after execution*\n";
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
     cout << "----------------------------------------------------------\n"
          << *tristan << endl
          << "----------------------------------------------------------\n"
          << *zouzou << endl
          << "----------------------------------------------------------\n"
          << *claire << endl
          << "----------------------------------------------------------\n"
          << *Order::neutralPlayer() << endl
          << "----------------------------------------------------------\n";

     cout << "\nNext order: " << *(next = zouzou->nextOrder()) << "\nEffect: ";
     next->execute();

     cout << "\nNext order: " << *(next = tristan->nextOrder()) << "\nEffect: ";
     next->execute();
     for (Territory *territory : territories)
          cout << "\t" << std::left << std::setw(9) << territory->getTerritoryName()
               << " : " << territory->getArmyUnits() << " units\n";
}
