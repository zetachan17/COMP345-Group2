#include "Orders/Orders.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;

Order::Order() : m_player(NULL), m_description(""), m_effect("") {}

Order::Order(Player *player) : m_player(player), m_description(""), m_effect("") {}

// to remove once all suborders are implemented
Order::Order(const string &type) : m_player(NULL), m_description(type), m_effect("") {}

Order::Order(const Order &other)
{
    m_player = other.m_player;
    m_description = other.m_description;
    m_effect = other.m_effect;
}

Order &Order::operator=(const Order &rightSide)
{
    m_player = rightSide.m_player;
    m_description = rightSide.m_description;
    m_effect = rightSide.m_effect;
    return *this;
}

ostream &operator<<(ostream &output, const Order &order)
{
    output << order.m_description;
    return output;
}

Order::~Order() {}

Player *Order::m_neutralPlayer = NULL;

Player *Order::neutralPlayer()
{
    return m_neutralPlayer;
}

Deploy::Deploy(Player *player, int armyUnits, Territory *target)
    : Order(player), m_units(armyUnits), m_territory(target)
{
    m_description = "Deploy " + std::to_string(armyUnits) + " units to " + target->getTerritoryName();
}

Order *Deploy::clone() const
{
    return (new Deploy(*this));
}

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Deploy::validate() const
{
    if (m_territory->getOwner() == m_player)
        return true;

    return false;
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was executed.
// If not valid, function does nothing.
void Deploy::execute()
{
    if (validate())
    {
        m_territory->setArmyUnits(m_territory->getArmyUnits() + m_units);
        m_player->addReinforcements(-m_units);

        m_effect = "Player " + m_player->getPlayerName() + " deployed " + std::to_string(m_units);
        m_effect += " units to " + m_territory->getTerritoryName() + ".";

        // m_effect = "Player: " + m_player->getPlayerName() + "     Order: Deploy     Effect: ";
        // m_effect += std::to_string(m_units) + " units deployed to ";
        // m_effect += m_territory->getTerritoryName();

        cout << m_effect << endl;
    }
    else
    {
        m_effect = "Player " + m_player->getPlayerName() + " deploy order invalid, no effect. ";
        m_effect += m_territory->getTerritoryName() + " is not a valid target territory.";

        // m_effect = "Player: " + m_player->getPlayerName() + "     Order: Deploy     Effect: Invalid, no ";
        // m_effect += "effect. " +  m_territory->getTerritoryName() + " is not a valid territory";

        cout << m_effect << endl;
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

Bomb::Bomb() : Order() {}

Bomb::Bomb(Player *player, Territory *target) : Order(player), m_territory(target)
{
    m_description = "Bomb " + target->getTerritoryName();
}

Order *Bomb::clone() const
{
    return (new Bomb(*this));
}

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Bomb::validate() const
{
    vector<Territory *> toAttack = m_player->toAttack();

    bool adjacent = std::find(toAttack.begin(), toAttack.end(), m_territory) != toAttack.end();

    if (!(m_territory->getOwner() == m_player) && adjacent)
        return true;

    return false;
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
void Bomb::execute()
{
    if (validate())
    {
        int units = m_territory->getArmyUnits();
        m_territory->setArmyUnits(units / 2);

        string territory = m_territory->getTerritoryName();
        int remainingUnits = m_territory->getArmyUnits();
        int lostUnits = units - remainingUnits;

        m_effect = "Player " + m_player->getPlayerName() + " bombed " + territory + ". ";
        m_effect += std::to_string(lostUnits) + " units killed. ";
        m_effect += territory + " now has " + std::to_string(remainingUnits) + " units.";

        cout << m_effect << endl;
    }
    else
    {
        m_effect = "Player " + m_player->getPlayerName() + " bomb order invalid, no effect. ";
        m_effect += m_territory->getTerritoryName() + " is not a valid target territory.";

        cout << m_effect << endl;
    }
}

Bomb::~Bomb() {}

Blockade::Blockade() : Order() {}

Blockade::Blockade(Player *player, Territory *target) : Order(player), m_territory(target)
{
    m_description = "Blockade " + target->getTerritoryName();
}

Order *Blockade::clone() const
{
    return (new Blockade(*this));
}

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Blockade::validate() const
{
    if (m_territory->getOwner() == m_player)
        return true;

    return false;
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
void Blockade::execute()
{
    if (validate())
    {
        m_territory->setArmyUnits(m_territory->getArmyUnits() * 2);
        m_player->removeTerritory(m_territory);

        string neutralCreated = "";
        if (m_neutralPlayer == NULL)
        {
            m_neutralPlayer = new Player("Neutral");
            neutralCreated = "Neutral player created. ";
        }
        m_neutralPlayer->addTerritory(m_territory);

        string territory = m_territory->getTerritoryName();
        string units = std::to_string(m_territory->getArmyUnits());

        m_effect = "Player " + m_player->getPlayerName() + " blockaded " + territory + ". ";
        m_effect += neutralCreated;
        m_effect += territory + " now has " + units + " units and belongs to the Neutral player.";

        cout << m_effect << endl;
    }
    else
    {
        m_effect = "Player " + m_player->getPlayerName() + " blockade order invalid, no effect. ";
        m_effect += m_territory->getTerritoryName() + " is not a valid target territory.";

        cout << m_effect << endl;
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

Order *OrdersList::nextOrder()
{
    Order *temp = NULL;
    if (!(m_orders.empty()))
    {
        temp = m_orders.front();
        m_orders.erase(m_orders.begin());
        return temp;
    }

    return temp;
}

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
