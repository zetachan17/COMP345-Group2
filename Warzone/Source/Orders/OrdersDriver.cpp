#include "Map/Map.h"
#include "Player/Player.h"
#include "Orders/Orders.h"
class Territory;

#include <iostream>
using std::cout;
using std::endl;

void testOrderExecution()
{
     cout << "** PART 4: ORDER EXECUTION IMPLEMENTATION **\n\n";

     Territory *brazil = new Territory(0, "Brazil", 1);
     Territory *chile = new Territory(0, "Chile", 1);

     cout << "*Created test territories*\n"
          << "-------------------------------------------\n"
          << *brazil
          << *chile
          << "-------------------------------------------\n";

     Player *tristan = new Player;
     tristan->addTerritory(brazil);
     tristan->addReinforcements(10);
     Deploy *o = new Deploy(tristan, 4, brazil);
     tristan->issueOrder(o);

     cout << "*Created test player*\n"
          << "*Added 'Brazil' territory to player*\n"
          << "*Added 10 reinforcements to player*\n"
          << "*Issued 'Deploy' order*\n"
          << "-------------------------------------------\n"
          << *tristan
          << "-------------------------------------------\n";

     Order *next = tristan->nextOrder();

     cout << "*Removed top order to execute*\n"
          << "Next order: " << *next << endl
          << "-------------------------------------------\n"
          << *tristan
          << "-------------------------------------------\n"
          << "Player's units before order execution: " << tristan->getReinforcementPool() << endl
          << "Brazil's units before order execution: " << brazil->getUnits() << endl
          << "-------------------------------------------\n";

     next->execute();

     cout << "-------------------------------------------\n"
          << "*Executed Deploy*\n"
          << "Tristan's units after order execution: " << tristan->getReinforcementPool() << endl
          << "Brazil's units after order execution: " << brazil->getUnits() << endl;

     cout << "-------------------------------------------\n"
          << "-------------------------------------------\n";
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
