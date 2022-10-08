#include "Orders.h"
#include <iostream>
using std::cout;
using std::endl;

// Order class constructor
Order::Order(string type) : m_type(type) {}

Order::Order(const Order &other)
{
    m_type = other.m_type;
}

Order *Order::clone() const
{
    return (new Order(*this));
}

bool Order::validate() const
{
    return false;
}

void Order::execute() {}

Order &Order::operator=(const Order &rightSide)
{
    m_type = rightSide.m_type;
    return *this;
}

ostream &operator<<(ostream &output, const Order &order)
{
    output << order.m_type;
    return output;
}

Order::~Order() {}

// The different kinds of orders implemented as subclasses of the Order class
Deploy::Deploy() : Order("Deploy") {}

Order *Deploy::clone() const
{
    return (new Deploy(*this));
}

bool Deploy::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
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
        cout << *this << " order executed.\n";
    }
}

Deploy::~Deploy() {}

Advance::Advance() : Order("Advance") {}

Order *Advance::clone() const
{
    return (new Advance(*this));
}

bool Advance::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
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
        cout << *this << " order executed.\n";
    }
}

Advance::~Advance() {}

Bomb::Bomb() : Order("Bomb") {}

Order *Bomb::clone() const
{
    return (new Bomb(*this));
}

bool Bomb::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
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
        cout << *this << " order executed.\n";
    }
}

Bomb::~Bomb() {}

Blockade::Blockade() : Order("Blockade") {}

Order *Blockade::clone() const
{
    return (new Blockade(*this));
}

bool Blockade::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
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
        cout << *this << " order executed.\n";
    }
}

Blockade::~Blockade() {}

Airlift::Airlift() : Order("Airlift") {}

Order *Airlift::clone() const
{
    return (new Airlift(*this));
}

bool Airlift::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
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
        cout << *this << " order executed.\n";
    }
}

Airlift::~Airlift() {}

Negotiate::Negotiate() : Order("Negotiate") {}

Order *Negotiate::clone() const
{
    return (new Negotiate(*this));
}

bool Negotiate::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
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
        cout << *this << " order executed.\n";
    }
}

Negotiate::~Negotiate() {}

OrdersList::OrdersList() {}

OrdersList::OrdersList(const OrdersList &other)
{
    for (auto &o : other.m_orders)
        issue(o->clone());
}

// issue() adds an order to the list.
void OrdersList::issue(Order *newOrder)
{
    m_orders.push_back(newOrder);
}

// move() takes in the current position of an order, from a list of current orders shown to the
// user, and moves it to a new position chosen by the user. Note the orders are shown to the user
// starting from 1 (first order), so the positions received are subtracted by 1
void OrdersList::move(int p, int newP)
{
    Order *temp = m_orders[p - 1];
    m_orders.erase(m_orders.begin() + (p - 1));
    m_orders.insert(m_orders.begin() + (newP - 1), temp);
}

// remove() takes in the position of an order, from a list of current orders shown to the user,
// and removes it from the list. Note the orders are shown to user starting from 1 (first order),
// so the position received is subtracted by 1
void OrdersList::remove(int p)
{
    delete (m_orders[p - 1]);
    m_orders.erase(m_orders.begin() + (p - 1));
}

void OrdersList::executeNextOrder()
{
    m_orders[0]->execute();
    remove(1);
}

OrdersList::~OrdersList()
{
    for (auto &order : m_orders)
        delete order;
    m_orders.clear();
}

OrdersList &OrdersList::operator=(const OrdersList &rightSide)
{
    if (!(this->m_orders.empty()))
    {
        for (auto &order : m_orders)
            delete order;
        m_orders.clear();
    }

    for (auto &o : rightSide.m_orders)
        issue(o->clone());

    return *this;
}

ostream &operator<<(ostream &output, const OrdersList &orders)
{
    if (orders.m_orders.empty())
    {
        cout << "-ORDERS LIST EMPTY-" << endl;
        return output;
    }
    else
    {
        int i = 1;
        for (auto &order : orders.m_orders)
            cout << i++ << ". " << *order << endl;
        return output;
    }
}
