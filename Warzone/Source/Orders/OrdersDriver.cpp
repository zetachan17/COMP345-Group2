#include "Orders.h"
#include <iostream>
using std::cout;

/// Tests the functionality of the Orders & OrdersList classes.
/// Orders are created and added to the list, removed, moved around, and then executed.
void testOrdersLists()
{
     cout << "** PART 3: ORDERS LIST **\n\n";

     OrdersList *orders = new OrdersList();
     cout << "*Created order list*\n";

     // testing creating orders and adding them to the list sequentially
     orders->issue(new Negotiate());
     orders->issue(new Bomb());
     orders->issue(new Advance());
     orders->issue(new Deploy());
     orders->issue(new Airlift());
     orders->issue(new Blockade());
     cout << "*Created & placed the 6 different orders into the orders list*\n"
          << "-------------------------------------------\n"
          << "CURRENT ORDER LIST:\n"
          << *orders
          << "-------------------------------------------\n";

     orders->issue(new Bomb());
     orders->issue(new Airlift());
     orders->issue(new Negotiate());
     orders->issue(new Airlift());
     orders->issue(new Advance());
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
     orders->issue(new Bomb());
     orders->issue(new Advance());
     orders->issue(new Negotiate());
     orders->issue(new Deploy());
     orders->issue(new Blockade());
     orders->issue(new Airlift());
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
          << "-------------------------------------------\n"
          << "-------------------------------------------\n";
}
