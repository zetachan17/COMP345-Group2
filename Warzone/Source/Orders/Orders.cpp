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

string Order::effect() const
{
    return m_effect;
}

string Order::player() const
{
    return m_player->getPlayerName();
}

string Order::type() const
{
    return m_description.substr(0, m_description.find(" "));
}

Player *Order::m_neutralPlayer = NULL;
int Order::m_attackCount = 0;
vector<pair<Player *, Player *>> Order::m_ceaseFire;

Player *Order::neutralPlayer()
{
    return m_neutralPlayer;
}

Deploy::Deploy(Player *player, int armyUnits, Territory *target) : Order(player),
                                                                   m_units(armyUnits),
                                                                   m_territory(target)
{
    m_description = "Deploy " + std::to_string(armyUnits) + " units to " +
                    target->getTerritoryName();
}

Order *Deploy::clone() const
{
    return (new Deploy(*this));
}

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Deploy::validate()
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

        m_effect = "Player " + player() + " deployed " + std::to_string(m_units) + " units to " +
                   m_territory->getTerritoryName() + ".";

        cout << m_effect << endl;
    }
    else
    {
        m_effect = "Player " + player() + " deploy order invalid, no effect. " +
                   m_territory->getTerritoryName() + " is not a valid target territory.";

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
bool Advance::validate()
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

    if (--m_attackCount == 0)
        m_ceaseFire.clear();
}

Advance::~Advance() {}

Bomb::Bomb() : Order() {}

Bomb::Bomb(Player *player, Territory *target) : Order(player), m_territory(target),
                                                m_negotiate(false)
{
    m_description = "Bomb " + target->getTerritoryName();
    ++m_attackCount;
}

Order *Bomb::clone() const
{
    return (new Bomb(*this));
}

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Bomb::validate()
{
    // check for active negotiations between the two players involved
    for (auto pair : m_ceaseFire)
    {
        const Player *targetPlayer = m_territory->getOwner();
        if (pair.first == m_player && pair.second == targetPlayer)
            return !(flipNegotiate());
        if (pair.first == targetPlayer && pair.second == m_player)
            return !(flipNegotiate());
    }

    // check if target territory is adjacent to one of the current player's territories
    vector<Territory *> toAttack = m_player->toAttack();

    bool adjacent = std::find(toAttack.begin(), toAttack.end(), m_territory) != toAttack.end();

    if ((m_territory->getOwner() != m_player) && adjacent)
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
        int initialUnits = m_territory->getArmyUnits();
        m_territory->setArmyUnits(initialUnits / 2);

        string territory = m_territory->getTerritoryName();
        int survivingUnits = m_territory->getArmyUnits();
        string lostUnits = std::to_string(initialUnits - survivingUnits);

        m_effect = "Player " + player() + " bombed " + territory + ". " + lostUnits +
                   " units killed. " + territory + " now has " + std::to_string(survivingUnits) +
                   " units.";

        cout << m_effect << endl;
    }
    else
    {
        m_effect = "Player " + player() + " bomb order invalid, no effect. ";

        if (m_negotiate)
            m_effect += "Negotiations are in effect between " + player() + " and " +
                        m_territory->getOwner()->getPlayerName() + ".";
        else
            m_effect += m_territory->getTerritoryName() + " is not a valid target territory.";

        cout << m_effect << endl;
    }

    if (--m_attackCount == 0)
        m_ceaseFire.clear();
}

bool Bomb::flipNegotiate()
{
    return (m_negotiate = !m_negotiate);
}

Bomb::~Bomb()
{
}

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
bool Blockade::validate()
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

        m_effect = "Player " + player() + " blockaded " + territory + ". " + neutralCreated +
                   territory + " now has " + units + " units and belongs to the Neutral player.";

        cout << m_effect << endl;
    }
    else
    {
        m_effect = "Player " + player() + " blockade order invalid, no effect. " +
                   m_territory->getTerritoryName() + " is not a valid target territory.";

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
bool Airlift::validate()
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

Negotiate::Negotiate(Player *player, Player *target) : Order(player), m_target(target)
{
    m_description = "Negotiate with " + target->getPlayerName();
}

Order *Negotiate::clone() const
{
    return (new Negotiate(*this));
}

// validate() not fully implemented yet for part 1, simply returns true (valid) for now to
// demonstrate it can be accessed to check validity.
bool Negotiate::validate()
{
    if (m_target != m_player)
        return true;

    return false;
}

// execute() not fully implemented yet for part 1, for now it simply calls validate()
// to check validity first and then prints that the order was execute.
// If not valid, function does nothing.
void Negotiate::execute()
{
    if (validate())
    {
        m_ceaseFire.emplace_back(m_player, m_target);

        m_effect = "Player " + player() + " entered negotiations with " +
                   m_target->getPlayerName() +
                   ". Attacks between them are invalid for the remainder of the turn.";

        cout << m_effect << endl;
    }
    else
    {
        m_effect = "Player " + player() + " negotiate order invalid, no effect. " +
                   "A player cannot negotiate with themselves.";

        cout << m_effect << endl;
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

const vector<Order *> *OrdersList::orders() const
{
    return &m_orders;
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
