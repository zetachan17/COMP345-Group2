#include "Orders/Orders.h"
#include <iostream>
using std::cout;
using std::endl;

Order::Order(string type) : m_type(type) {}

Order::Order(const Order &other)
{
    m_type = other.m_type;
}

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

Deploy::Deploy() : Order("Deploy") {}

Order *Deploy::clone() const
{
    return (new Deploy(*this));
}

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Deploy::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
        return true;
    }
    else if (false)
    {
        cout << *this << " order not valid.\n";
        return false;
    }
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
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

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Advance::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
        return true;
    }
    else if (false)
    {
        cout << *this << " order not valid.\n";
        return false;
    }
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
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

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Bomb::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
        return true;
    }
    else if (false)
    {
        cout << *this << " order not valid.\n";
        return false;
    }
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
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

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Blockade::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
        return true;
    }
    else if (false)
    {
        cout << *this << " order not valid.\n";
        return false;
    }
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
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

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Airlift::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
        return true;
    }
    else if (false)
    {
        cout << *this << " order not valid.\n";
        return false;
    }
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
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

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Negotiate::validate() const
{
    if (true)
    {
        cout << *this << " order validated.\n";
        return true;
    }
    else if (false)
    {
        cout << *this << " order not valid.\n";
        return false;
    }
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
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
        addOrder(o->clone());
}

OrdersList::OrdersList(const vector<Order*> orders)
{
    for (Order* order : orders)
    {
        this->addOrder(order);
    }
}

// addOrder() adds an order to the list.
void OrdersList::addOrder(Order *newOrder)
{
    m_orders.push_back(newOrder);
}

// move() takes in the current position of an order, from a list of current orders shown to the
// user, and moves it to a new position chosen by the user. Note the orders are shown to the user
// starting from 1 (first order), so the positions received are subtracted by 1
void OrdersList::move(int p, int newP)
{
    if (!((p < 1) || (p > m_orders.size()) || (newP < 1) || (newP > m_orders.size())))
    {
        Order *temp = m_orders[p - 1];
        m_orders.erase(m_orders.begin() + (p - 1));
        m_orders.insert(m_orders.begin() + (newP - 1), temp);
    }
}

// remove() takes in the position of an order, from a list of current orders shown to the user,
// and removes it from the list. Note the orders are shown to user starting from 1 (first order),
// so the position received is subtracted by 1
void OrdersList::remove(int p)
{
    if (!((p < 1) || (p > m_orders.size())))
    {
        delete (m_orders[p - 1]);
        m_orders.erase(m_orders.begin() + (p - 1));
    }
}

// executes the top order in the list (position 0 in the vector), deletes Order object
// and removes it from the list
void OrdersList::executeNextOrder()
{
    m_orders[0]->execute();
    remove(1);
}

vector<Order*> OrdersList::getOrders()
{
    return m_orders;
}

// destructor deletes every Order object and then clears the underlying vector container
OrdersList::~OrdersList()
{
    for (auto &order : m_orders)
        delete order;
    m_orders.clear();
}

// overloaded assignment operator, first checks if the LHS order list is empty, if not,
// deletes its current Order objects and clears the list, then creates a hard copy of the RHS
// order list and assigns it to the LHS OrdersList
OrdersList &OrdersList::operator=(const OrdersList &rightSide)
{
    if (!(this->m_orders.empty()))
    {
        for (auto &order : m_orders)
            delete order;
        m_orders.clear();
    }

    for (auto &o : rightSide.m_orders)
        addOrder(o->clone());

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
