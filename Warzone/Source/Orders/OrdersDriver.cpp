#include "Map/Map.h"
#include "Player/Player.h"
#include "Orders/Orders.h"
class Territory;

#include <iostream>
using std::cout;
using std::endl;

// clang++ -std=c++11 test_Orders.cpp OrdersDriver.cpp Map.cpp Player.cpp Orders.cpp Cards.cpp

void testOrderExecution()
{
     cout << "-------------------------------------------\n"
          << "** PART 4: ORDER EXECUTION IMPLEMENTATION **\n\n";

     Territory *brazil = new Territory(0, "Brazil", 1);
     Territory *chile = new Territory(1, "Chile", 1);
     Territory *canada = new Territory(2, "Canada", 2);

     cout << "*Created test territories*\n"
          << "-------------------------------------------\n"
          << *brazil
          << *chile
          << *canada
          << "-------------------------------------------\n";

     Player *tristan = new Player("Tristan");
     tristan->addTerritory(brazil);
     tristan->addTerritory(chile);
     tristan->addReinforcements(10);
     Order *o1 = new Deploy(tristan, 4, chile);
     chile->addUnits(6);
     brazil->addUnits(2);
     Order *o2 = new Blockade(tristan, chile);
     Order *o3 = new Blockade(tristan, canada);
     Order *o4 = new Blockade(tristan, brazil);
     tristan->issueOrder(o1);
     tristan->issueOrder(o2);
     tristan->issueOrder(o3);
     tristan->issueOrder(o4);

     cout << "*Created test player*\n"
          << "*Added 'Brazil' territory to player*\n"
          << "*Added 10 reinforcements to player*\n"
          << "*Issued order*\n"
          << "-------------------------------------------\n"
          << *tristan
          << "-------------------------------------------\n";

     Order *next = tristan->nextOrder();

     cout << "*Removed top order to execute*\n"
          << "Next order: " << *next << endl
          << "-------------------------------------------\n"
          << *tristan
          << "-------------------------------------------\n"
          << "Player's units : " << tristan->getReinforcementPool() << endl
          // << "Brazil's units before order execution: " << brazil->getUnits() << endl
          << "Chile's units : " << chile->getUnits() << endl
          << "Chile's owner : " << chile->getOwner()->getName() << endl;

     if (Order::neutralPlayer() == NULL)
          cout << "Neutral player not created\n";
     else
          cout << *Order::neutralPlayer() << endl;

     cout << "-------------------------------------------\n";

     next->execute();

     cout << "-------------------------------------------\n"
          << "Tristan's units : " << tristan->getReinforcementPool() << endl
          // << "Brazil's units after order execution: " << brazil->getUnits() << endl
          << "Chile's units : " << chile->getUnits() << endl
          << "Chile's owner : " << chile->getOwner()->getName() << endl
          << "-------------------------------------------\n";

     if (Order::neutralPlayer() == NULL)
          cout
              << "Neutral player not created\n";
     else
          cout << *Order::neutralPlayer() << endl;

     cout << *tristan
          << "-------------------------------------------\n"
          << "-------------------------------------------\n";

     next = tristan->nextOrder();
     next->execute();
     next = tristan->nextOrder();
     next->execute();
     next = tristan->nextOrder();
     next->execute();

     if (Order::neutralPlayer() == NULL)
          cout
              << "Neutral player not created\n";
     else
          cout << *Order::neutralPlayer() << endl;

     cout << "-------------------------------------------\n"
          << *tristan;
}

/*
/// First driver, testing the initial functionality of the Orders & OrdersList classes.
/// Orders are created and added to the list, removed, moved around, and then executed.
/// A few methods have since been deleted or rewritten.
void testOrdersLists()
{
     cout << "** PART 3: ORDERS LIST **\n\n";

     OrdersList *orders = new OrdersList();
     cout << "*Created order list*\n";

     // testing creating orders and adding them to the list sequentially
     orders->addOrder(new Negotiate());
     orders->addOrder(new Bomb());
     orders->addOrder(new Advance());
     orders->addOrder(new Deploy());
     orders->addOrder(new Airlift());
     orders->addOrder(new Blockade());
     cout << "*Created & placed the 6 different orders into the orders list*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";

     orders->addOrder(new Bomb());
     orders->addOrder(new Airlift());
     orders->addOrder(new Negotiate());
     orders->addOrder(new Airlift());
     orders->addOrder(new Advance());
     cout << "*Created & placed 5 additional orders*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n"
          << "-------------------------------------------\n";

     // testing remove() on different orders and positions of the list
     orders->remove(1);
     cout << "*Removed order #1 (Negotiate)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(10);
     cout << "*Removed order #10 (Advance)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(6);
     cout << "*Removed order #6 (Bomb)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(3);
     cout << "*Removed order #3 (Deploy)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(3);
     cout << "*Removed order #3 (Airlift)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(3);
     cout << "*Removed order #3 (Blockade)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n"
          << "-------------------------------------------\n";

     // testing move() on the different orders and positions of the list
     orders->move(1, 5);
     cout << "*Moved order #1 (Bomb) to position #5*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->move(3, 1);
     cout << "*Moved order #3 (Negotiate) to position #1*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->move(5, 2);
     cout << "*Moved order #5 (Bomb) to position #2*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->move(5, 1);
     cout << "*Moved order #5 (Airlift) to position #1*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n"
          << "-------------------------------------------\n";

     // Testing emptying the list
     orders->remove(1);
     cout << "*Removed order #1 (Airlift)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(2);
     cout << "*Removed order #2 (Bomb)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(3);
     cout << "*Removed order #3 (Airlift)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(1);
     cout << "*Removed order #1 (Negotiate)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->remove(1);
     cout << "*Removed order #1 (Advance)*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     orders->addOrder(new Bomb());
     orders->addOrder(new Advance());
     orders->addOrder(new Negotiate());
     orders->addOrder(new Deploy());
     orders->addOrder(new Blockade());
     orders->addOrder(new Airlift());
     cout << "*Added 6 different orders to the list*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n"
          << "-------------------------------------------\n";

     // executing and validating orders
     cout << "*Execute first order*\n";
     orders->executeNextOrder();
     cout << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     cout << "*Execute next order*\n";
     orders->executeNextOrder();
     cout << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     cout << "*Execute next order*\n";
     orders->executeNextOrder();
     cout << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     cout << "*Execute next order*\n";
     orders->executeNextOrder();
     cout << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     cout << "*Execute next order*\n";
     // orders->executeNextOrder();
     cout << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";
     cout << "*Execute next order*\n";
     // orders->executeNextOrder();
     cout << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n"
          << "-------------------------------------------\n";
}
*/
