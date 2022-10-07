#include "Orders.h"
#include <iostream>
using std::cout;
using std::endl;

// Order class constructor
Order::Order(string order) : name(order) {}

// The different kinds of orders implemented as subclasses of the Order class
Deploy::Deploy() : Order("Deploy") {}

bool Deploy::validate() const
{
    if (true)
    {
        return true;
    }
    else if (false)
    {
        return false;
    }
}

void Deploy::execute()
{
    if (validate())
    {
    }
}

Advance::Advance() : Order("Advance") {}

bool Advance::validate() const
{
    if (true)
    {
        return true;
    }
    else if (false)
    {
        return false;
    }
}

void Advance::execute()
{
    if (validate())
    {
    }
}

Bomb::Bomb() : Order("Bomb") {}

bool Bomb::validate() const
{
    if (true)
    {
        return true;
    }
    else if (false)
    {
        return false;
    }
}

void Bomb::execute()
{
    if (validate())
    {
    }
}

Blockade::Blockade() : Order("Blockade") {}

bool Blockade::validate() const
{
    if (true)
    {
        return true;
    }
    else if (false)
    {
        return false;
    }
}

void Blockade::execute()
{
    if (validate())
    {
    }
}

Airlift::Airlift() : Order("Airlift") {}

bool Airlift::validate() const
{
    if (true)
    {
        return true;
    }
    else if (false)
    {
        return false;
    }
}

void Airlift::execute()
{
    if (validate())
    {
    }
}

Negotiate::Negotiate() : Order("Negotiate") {}

bool Negotiate::validate() const
{
    if (true)
    {
        return true;
    }
    else if (false)
    {
        return false;
    }
}

void Negotiate::execute()
{
    if (validate())
    {
    }
}

ostream &operator<<(ostream &output, const Order *o)
{
    output << o->name << endl;
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

void OrdersList::executeNextOrder()
{
    m_Orders[0]->execute();
    remove(1);
}

ostream &operator<<(ostream &output, const OrdersList *oList)
{
    int i = 1;
    for (auto &order : oList->m_Orders)
        cout << i++ << ". " << order;
    return output;
}
