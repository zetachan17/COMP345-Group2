#include "Orders.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

void testOrdersList()
{
    OrdersList orders;

    orders.issue(Order("Deploy"));
    orders.issue(Order("Deploy"));
    orders.issue(Order("Advance"));
    orders.issue(Order("Bomb"));

    orders.output();

    cout << "----------\n"
         << "Moved order #4 to position #1:\n";
    orders.move(4, 1);
    orders.output();
    cout << "----------\n";

    cout << "----------\n"
         << "Moved order #1 to position #4:\n";
    orders.move(1, 4);
    orders.output();
    cout << "----------\n";

    cout << "----------\n"
         << "Moved order #2 to position #3:\n";
    orders.move(2, 3);
    orders.output();
    cout << "----------\n";
}

int main()
{
    testOrdersList();
}
