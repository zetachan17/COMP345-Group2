#include "Orders.h"
#include <iostream>
using std::cout;

void testOrdersList()
{
     OrdersList orders;

     // Issue some test orders to the list.
     cout << "-----------\n"
          << "Issued 5 orders:\n";
     orders.issue(Order("Deploy"));
     orders.issue(Order("Deploy"));
     orders.issue(Order("Advance"));
     orders.issue(Order("Bomb"));
     orders.issue(Order("Negotiate"));
     orders.output();
     cout << "-----------\n";

     // Test moving orders around.
     cout << "-----------\n"
          << "Moved order #4 (Bomb) to position #1:\n";
     orders.move(4, 1);
     orders.output();
     cout << "-----------\n";

     cout << "-----------\n"
          << "Moved order #1 (Bomb) back to position #4:\n";
     orders.move(1, 4);
     orders.output();
     cout << "-----------\n";

     cout << "-----------\n"
          << "Moved order #2 (Deploy) to position #3:\n";
     orders.move(2, 3);
     orders.output();
     cout << "-----------\n";

     // Test removing orders.
     cout << "-----------\n"
          << "Removed order #3 (Deploy):\n";
     orders.remove(3);
     orders.output();
     cout << "-----------\n";

     cout << "-----------\n"
          << "Removed order #2 (Advance):\n";
     orders.remove(2);
     orders.output();
     cout << "-----------\n";

     // Issue new orders to check issueing after moving and removing.
     orders.issue(Order("Deploy"));
     orders.issue(Order("Deploy"));
     orders.issue(Order("Blockade"));
     orders.issue(Order("Airlift"));
     cout << "-----------\n"
          << "Issued 4 new orders:\n";
     orders.output();
     cout << "-----------\n";
}
