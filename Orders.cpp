#include "Orders.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

// Order class representing a possible player order.
Order::Order(string order)
{
    description = order;
}

string Order::getDescription()
{
    return description;
}

Deploy::Deploy(string order) : Order(order)
{
}

Advance::Advance(string order) : Order(order)
{
}

Bomb::Bomb(string order) : Order(order)
{
}

Blockade::Blockade(string order) : Order(order)
{
}

Airlift::Airlift(string order) : Order(order)
{
}

Negotiate::Negotiate(string order) : Order(order)
{
}

/*
ostream& operator << (ostream& output, Order& o)
{
    output << o.type << endl;
    return o;
}
*/

// OrdersList class representing the list of orders issued by a player.
void OrdersList::issue(Order newOrder)
{
    m_Orders.push_back(newOrder);
}

// move() takes in the current position of an order, from a list of current orders shown to the user,
// and moves it to a new position chosen by the user.
void OrdersList::move(int p, int newP)
{
    Order temp = m_Orders[p - 1];
    int newPos = newP - 1;
    remove(p);
    m_Orders.insert(m_Orders.begin() + newPos, temp);
}

void OrdersList::remove(int p)
{
    int pos = p - 1;
    m_Orders.erase(m_Orders.begin() + pos);
}

void OrdersList::output()
{
    for (int i = 0; i < m_Orders.size(); i++)
        cout << (i + 1) << ". " << m_Orders[i].getDescription() << endl;
}