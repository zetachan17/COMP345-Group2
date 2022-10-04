#include "Orders.h"
#include <iostream>
using std::cout;
using std::endl;

void testOrdersList()
{
     OrdersList orders;

     // Issue some test orders to the list.
     cout << string(21, '-') << endl
          << "Issued 5 orders:\n";
     orders.issue(Deploy());
     orders.issue(Deploy());
     orders.issue(Advance());
     orders.issue(Bomb());
     orders.issue(Negotiate());
     orders.output();
     cout << string(21, '-') << endl;

     // Test moving orders around with move().
     cout << string(21, '-') << endl
          << "Moved order #4 (Bomb) to position #1:\n";
     orders.move(4, 1);
     orders.output();
     cout << string(21, '-') << endl;

     cout << string(21, '-') << endl
          << "Moved order #1 (Bomb) back to position #4:\n";
     orders.move(1, 4);
     orders.output();
     cout << string(21, '-') << endl;

     cout << string(21, '-') << endl
          << "Moved order #2 (Deploy) to position #3:\n";
     orders.move(2, 3);
     orders.output();
     cout << string(21, '-') << endl;

     // Test removing orders with remove() orders.
     cout << string(21, '-') << endl
          << "Removed order #3 (Deploy):\n";
     orders.remove(3);
     orders.output();
     cout << string(21, '-') << endl;

     cout << string(21, '-') << endl
          << "Removed order #2 (Advance):\n";
     orders.remove(2);
     orders.output();
     cout << string(21, '-') << endl;

     // Issue new orders to check issueing after moving and removing.
     orders.issue(Deploy());
     orders.issue(Deploy());
     orders.issue(Blockade());
     orders.issue(Airlift());
     cout << string(21, '-') << endl
          << "Issued 4 new orders:\n";
     orders.output();
     cout << string(21, '-') << endl;
}