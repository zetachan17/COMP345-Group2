#include "Orders.h"
#include <iostream>
using std::cout;
using std::endl;

// Order class constructor
Order::Order(string order) : name(order) {}

// Order class name accessor
string Order::getName() const
{
    return name;
}

// The different kinds of orders implemented as subclasses of the Order class
Deploy::Deploy() : Order("Deploy") {}

Advance::Advance() : Order("Advance") {}

Bomb::Bomb() : Order("Bomb") {}

Blockade::Blockade() : Order("Blockade") {}

Airlift::Airlift() : Order("Airlift") {}

Negotiate::Negotiate() : Order("Negotiate") {}

ostream &operator<<(ostream &output, const Order &o)
{
    output << o.name << endl;
    return output;
}

// issue() adds an order to the list.
void OrdersList::issue(Order *newOrder)
{
    m_Orders.push_back(newOrder);
}

// move() takes in the current position of an order, from a list of current orders shown to the
// user, and moves it to a new position chosen by the user. Note the orders are shown to the user
// starting from 1 (first order), so the positions received are subtracted by 1
void OrdersList::move(int p, int newP)
{
    Order *temp = m_Orders[p - 1];
    m_Orders.erase(m_Orders.begin() + (p - 1));
    m_Orders.insert(m_Orders.begin() + (newP - 1), temp);
}

// remove() takes in the position of an order, from a list of current orders shown to the user,
// and removes it from the list. Note the orders are shown to user starting from 1 (first order),
// so the position received is subtracted by 1
void OrdersList::remove(int p)
{
    delete m_Orders[p - 1];
    m_Orders.erase(m_Orders.begin() + (p - 1));
}

// Outputs a list of the issued orders, in the order they were issued starting from "1."
void OrdersList::output() const
{
    int i = 1;
    for (auto &order : m_Orders)
        cout << i++ << ". " << *order;
}

ostream &operator<<(ostream &output, const OrdersList &oList)
{
    int i = 1;
    for (auto &order : oList.m_Orders)
        cout << i++ << ". " << *order;
    return output;
}
