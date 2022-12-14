#include "Orders/Orders.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "Cards/Cards.h"
#include "GameEngine/GameEngine.h"
#include "PlayerStrategies/PlayerStrategies.h"

#include <iostream>
#include <fstream>
#include <string>
using std::string;
#include <algorithm>
#include <random>
using std::cout;
using std::endl;
using std::to_string;

Order::Order() : m_player(nullptr), m_description(""), m_effect("Not executed"),
                 m_executed(false) {}

Order::Order(Player *player) : m_player(player), m_description(""), m_effect("Not executed"),
                               m_executed(false) {}

Order::Order(const Order &other) : m_player(other.m_player), m_description(other.m_description),
                                   m_effect(other.m_effect), m_executed(other.m_executed) {}

Order &Order::operator=(const Order &right)
{
    m_player = right.m_player;
    m_description = right.m_description;
    m_effect = right.m_effect;
    return *this;
}

ostream &operator<<(ostream &output, const Order &order)
{
    if (order.m_executed)
        output << order.m_effect;
    else
        output << order.m_description;

    return output;
}

Order::~Order() {}

void Order::turnEnd()
{
    cout << "\n\n~~End of Turn~~\n"
         << "~~Active Negotiations Cleared~~\n"
         << "~~Players who conquered at least one territory draw a card~~\n\n";

    for (Player *player : m_getsCard)
    {
        cout << player->getPlayerName() + " draws a card: ";
        Card *cardDrawn = GameEngine::getDeck()->draw();

        if (!cardDrawn)
            cout << "~~DECK EMPTY: NO CARD DRAWN~~";
        else
            player->getHand()->addToHand(cardDrawn);
    }

    m_getsCard.clear();
    m_ceaseFire.clear();
}

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

bool Order::inNegotiations(Player *target) const
{
    for (auto pair : m_ceaseFire)
        if ((pair.first == m_player && pair.second == target) ||
            (pair.first == target && pair.second == m_player))
            return true;

    return false;
}

vector<Player *> *Order::getsCard()
{
    return &m_getsCard;
}

void Order::playerEarnedCard(Player *player)
{
    if (std::find(m_getsCard.begin(), m_getsCard.end(), player) == m_getsCard.end())
        m_getsCard.push_back(player);
}

vector<pair<Player *, Player *>> *Order::negotiations()
{
    return &m_ceaseFire;
}

void Order::decrementOrderCount(bool inOrderExecutionPhase)
{
    --m_orderCount;

    if (m_orderCount == 0 && inOrderExecutionPhase)
        turnEnd();
}

int Order::m_orderCount = 0;
vector<pair<Player *, Player *>> Order::m_ceaseFire;
vector<Player *> Order::m_getsCard;

Deploy::Deploy() : Order() {}

Deploy::Deploy(Player *player, int armyUnits, Territory *target)
    : Order(player), m_units(armyUnits), m_target(target)
{
    m_description = "Deploy " + to_string(armyUnits) + " units to " + target->getTerritoryName();
    ++m_orderCount;
}

Deploy::Deploy(const Deploy &other) : Order(other), m_units(other.m_units),
                                      m_target(other.m_target) {}

Deploy &Deploy::operator=(const Deploy &right)
{
    Order::operator=(right);
    m_units = right.m_units;
    m_target = right.m_target;

    return *this;
}

Order *Deploy::clone() const
{
    return (new Deploy(*this));
}

bool Deploy::validate()
{
    if (m_target->getOwner() != m_player)
    {
        cout << "~INVALID ORDER~\n";
        m_effect = player() + " does not control " + m_target->getTerritoryName() + ".";
        return false;
    }
    if (m_units > m_player->getReinforcementPool())
    {
        cout << "~INVALID ORDER~\n";
        m_effect = "Insufficient units.\n\t" + player() + "'s reinforcement pool only has " +
                   to_string(m_player->getReinforcementPool()) + " units.";
        return false;
    }
    cout << "~order validated~\n";
    return true;
}

void Deploy::execute()
{
    if (validate())
    {
        m_target->setArmyUnits(m_target->getArmyUnits() + m_units);
        m_player->addReinforcements(-m_units);

        m_effect = "Effect: " + player() + " deployed " + to_string(m_units) + " units to " +
                   m_target->getTerritoryName() + ".";
        cout << m_effect << endl;
    }
    else
        cout << (m_effect = "Effect: No effect. " + m_effect) << endl;

    m_executed = true;
    Notify(this);
    decrementOrderCount(true);
}

Deploy::~Deploy() {}

Advance::Advance() : Order() {}

Advance::Advance(Player *player, int units, Territory *source, Territory *target)
    : Order(player), m_units(units), m_source(source), m_target(target)
{
    m_description = "Advance " + to_string(units) + " units from " + source->getTerritoryName() +
                    " to " + target->getTerritoryName();
    ++m_orderCount;
}

Advance::Advance(const Advance &other) : Order(other), m_units(other.m_units),
                                         m_target(other.m_target), m_source(other.m_source) {}

Advance &Advance::operator=(const Advance &right)
{
    Order::operator=(right);
    m_units = right.m_units;
    m_target = right.m_target;
    m_source = right.m_source;

    return *this;
}

Order *Advance::clone() const
{
    return (new Advance(*this));
}

bool Advance::validate()
{
    if (m_units > m_source->getArmyUnits())
    {
        m_effect = "Invalid number of units. " + m_source->getTerritoryName() +
                   " has a max of " + to_string(m_source->getArmyUnits()) + " available units.";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    if (m_units == 0)
    {
        m_effect = "0 is not a valid number of units.";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    if (m_source->getOwner() != m_player)
    {
        m_effect = player() + " does not control " + m_source->getTerritoryName() + ".";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    // check if target territory is adjacent to source territory
    vector<Territory *> adjacent = m_source->getAdjacentTerritories();
    if (std::find(adjacent.begin(), adjacent.end(), m_target) == adjacent.end())
    {
        m_effect = m_source->getTerritoryName() + " and " + m_target->getTerritoryName() +
                   " are not adjacent.";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    if (m_target->getOwner() == m_player)
    {
        cout << "~order validated~\n";
        return true;
    }

    // check for active Negotiate order
    if (inNegotiations(m_target->getOwner()))
    {
        m_effect = "Negotiations are in effect between " + player() + " and " +
                   m_target->getOwner()->getPlayerName() + ".";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    cout << "~order validated~\n";
    return true;
}

void Advance::execute()
{
    if (validate())
    {
        if (m_target->getOwner() == m_player)
        {
            m_source->setArmyUnits(m_source->getArmyUnits() - m_units);
            m_target->setArmyUnits(m_target->getArmyUnits() + m_units);

            m_effect = "Effect: " + player() + " advanced " + to_string(m_units) + " units from " +
                       m_source->getTerritoryName() + " to " + m_target->getTerritoryName() + ".";
        }
        else
            battle();

        cout << m_effect << endl;
    }
    else
        cout << (m_effect = "Effect: No effect. " + m_effect) << endl;

    m_executed = true;
    Notify(this);
    decrementOrderCount(true);
}

void Advance::battle()
{
    m_target->getOwner()->underAttack();
    int defenceUnits = m_target->getArmyUnits();
    int attackingUnits = m_units;
    string defencePlayer = m_target->getOwner()->getPlayerName();

    m_effect = "Effect: " + player() + " advanced " + to_string(attackingUnits) + " units from " +
               m_source->getTerritoryName() + " to attack " + m_target->getTerritoryName() + ".\n";

    int attackKills = killCount(attackingUnits, 60);
    int defenceKills = killCount(defenceUnits, 70);

    defenceKills = std::min(defenceKills, attackingUnits);
    attackingUnits -= defenceKills;

    if ((attackingUnits > 0) && (attackKills >= defenceUnits))
    {
        conquer(attackingUnits);

        m_effect += "\tAttacker's " + to_string(m_units) + " units killed " +
                    to_string(defenceUnits) + " of " + to_string(defenceUnits) +
                    " defence units.\n\tDefender's " + to_string(defenceUnits) + " units killed " +
                    to_string(defenceKills) + " of " + to_string(m_units) + " attack units.\n\t" +
                    "Attacker victory! " + player() + " conquered " +
                    m_target->getTerritoryName() + " from " + defencePlayer + " with " +
                    to_string(attackingUnits) + " surviving units.";
    }
    else
    {
        m_source->setArmyUnits(m_source->getArmyUnits() - defenceKills);
        m_target->setArmyUnits(defenceUnits - attackKills);

        string retreat = "";
        if (attackingUnits > 0)
            retreat = "\n" + player() + " retreats " + to_string(attackingUnits) + " units to " +
                      m_source->getTerritoryName() + ".";

        m_effect += "\tAttacker's " + to_string(m_units) + " units killed " +
                    to_string(defenceUnits - m_target->getArmyUnits()) + " of " +
                    to_string(defenceUnits) + " defence units.\n\tDefender's " +
                    to_string(defenceUnits) + " units killed " + to_string(defenceKills) + " of " +
                    to_string(m_units) + " attack units.\n\tDefender victory! " + defencePlayer +
                    " retains control of " + m_target->getTerritoryName() + ". " + retreat;
    }
}

int Advance::killCount(int units, int probability) const
{
    std::random_device rd;
    std::mt19937 g{rd()};

    int kills = 0;
    for (int i = 0; i < units; i++)
        kills += ((g() % 100) < probability) ? 1 : 0;

    return kills;
}

void Advance::conquer(int survivingUnits)
{
    m_source->setArmyUnits(m_source->getArmyUnits() - m_units);

    m_target->getOwner()->removeTerritory(m_target);
    m_target->setArmyUnits(survivingUnits);
    m_player->addTerritory(m_target);

    playerEarnedCard(m_player);
}

Advance::~Advance() {}

Bomb::Bomb() : Order() {}

Bomb::Bomb(Player *player, Territory *target) : Order(player), m_target(target)
{
    m_description = "Bomb " + target->getTerritoryName();
    ++m_orderCount;
}

Bomb::Bomb(const Bomb &other) : Order(other), m_target(other.m_target) {}

Bomb &Bomb::operator=(const Bomb &right)
{
    Order::operator=(right);
    m_target = right.m_target;

    return *this;
}

Order *Bomb::clone() const
{
    return (new Bomb(*this));
}

bool Bomb::validate()
{
    // check if target territory is adjacent to one of the current player's territories
    vector<Territory *> adjacent = m_player->toAttack();
    if (std::find(adjacent.begin(), adjacent.end(), m_target) == adjacent.end())
    {
        m_effect = m_target->getTerritoryName() + " is not adjacent to any of " + player() +
                   "'s territories.";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    // check for active negotiations between the two players involved
    if (inNegotiations(m_target->getOwner()))
    {
        m_effect = "Negotiations are in effect between " + player() + " and " +
                   m_target->getOwner()->getPlayerName() + ".";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    cout << "~order validated~\n";
    return true;
}

void Bomb::execute()
{
    if (validate())
    {
        m_target->getOwner()->underAttack();

        int initialUnits = m_target->getArmyUnits();
        int survivingUnits = initialUnits / 2;
        m_target->setArmyUnits(survivingUnits);

        string territory = m_target->getTerritoryName();
        string lostUnits = to_string(initialUnits - survivingUnits);
        m_effect = "Effect: " + player() + " bombed " + territory + ". " + lostUnits +
                   " units killed. " + territory + " now has " + to_string(survivingUnits) +
                   " units.";
        cout << m_effect << endl;
    }
    else
        cout << (m_effect = "Effect: No effect. " + m_effect) << endl;

    m_executed = true;
    Notify(this);
    decrementOrderCount(true);
}

Bomb::~Bomb() {}

Blockade::Blockade() : Order() {}

Blockade::Blockade(Player *player, Territory *target) : Order(player), m_target(target)
{
    m_description = "Blockade " + target->getTerritoryName();
    ++m_orderCount;
}

Blockade::Blockade(const Blockade &other) : Order(other), m_target(other.m_target) {}

Blockade &Blockade::operator=(const Blockade &right)
{
    Order::operator=(right);
    m_target = right.m_target;

    return *this;
}

Order *Blockade::clone() const
{
    return (new Blockade(*this));
}

bool Blockade::validate()
{
    if (m_target->getOwner() == m_player)
    {
        cout << "~order validated~\n";
        return true;
    }

    m_effect = player() + " does not control " + m_target->getTerritoryName() + ".";
    cout << "~INVALID ORDER~\n";
    return false;
}

void Blockade::execute()
{
    if (validate())
    {
        m_target->setArmyUnits(m_target->getArmyUnits() * 2);
        m_player->removeTerritory(m_target);

        Player *neutralPlayer = nullptr;
        for (Player *player : GameEngine::getPlayers())
        {
            if (player->getStrategyType() == "Neutral")
            {
                neutralPlayer = player;
                break;
            }
        }

        string neutralCreated = "";
        if (!neutralPlayer)
        {
            GameEngine::addPlayer(neutralPlayer = new Player(new NeutralPlayerStrategy));
            neutralCreated = "Neutral Player created.";
        }
        neutralPlayer->addTerritory(m_target);

        m_effect = "Effect: " + player() + " blockaded " + m_target->getTerritoryName() + ". " +
                   neutralCreated + "\n\t" + m_target->getTerritoryName() + " now has " +
                   to_string(m_target->getArmyUnits()) + " units and belongs to a Neutral Player.";
        cout << m_effect << endl;
    }
    else
        cout << (m_effect = "Effect: No effect. " + m_effect) << endl;

    m_executed = true;
    Notify(this);
    decrementOrderCount(true);
}

Blockade::~Blockade() {}

Airlift::Airlift() : Order() {}

Airlift::Airlift(Player *player, int units, Territory *source, Territory *target)
    : Order(player), m_units(units), m_source(source), m_target(target)
{
    m_description = "Airlift " + to_string(units) + " units from " + source->getTerritoryName() +
                    " to " + target->getTerritoryName();
    ++m_orderCount;
}

Airlift::Airlift(const Airlift &other) : Order(other), m_units(other.m_units),
                                         m_target(other.m_target), m_source(other.m_source) {}

Airlift &Airlift::operator=(const Airlift &right)
{
    Order::operator=(right);
    m_units = right.m_units;
    m_target = right.m_target;
    m_source = right.m_source;

    return *this;
}

Order *Airlift::clone() const
{
    return (new Airlift(*this));
}

bool Airlift::validate()
{

    if (m_source->getOwner() != m_player)
    {
        m_effect = player() + " does not control " + m_source->getTerritoryName() + ".";
        cout << "~INVALID ORDER~\n";
        return false;
    }
    if (m_target->getOwner() != m_player)
    {
        m_effect = player() + " does not control " + m_target->getTerritoryName() + ".";
        cout << "~INVALID ORDER~\n";
        return false;
    }
    if (m_units > m_source->getArmyUnits())
    {
        m_effect = "Invalid number of advanced army units. " + m_source->getTerritoryName() +
                   " has a maximum of " + to_string(m_source->getArmyUnits()) + " available.";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    cout << "~order validated~\n";
    return true;
}

void Airlift::execute()
{
    if (validate())
    {
        m_source->setArmyUnits(m_source->getArmyUnits() - m_units);
        m_target->setArmyUnits(m_target->getArmyUnits() + m_units);

        m_effect = "Effect: " + player() + " airlifted " + to_string(m_units) + " units from " +
                   m_source->getTerritoryName() + " to " + m_target->getTerritoryName() + ".";
        cout << m_effect << endl;
    }
    else
        cout << (m_effect = "Effect: No effect. " + m_effect) << endl;

    m_executed = true;
    Notify(this);
    decrementOrderCount(true);
}

Airlift::~Airlift() {}

Negotiate::Negotiate() : Order() {}

Negotiate::Negotiate(Player *player, Player *target) : Order(player), m_target(target)
{
    m_description = "Negotiate with " + target->getPlayerName();
    ++m_orderCount;
}

Negotiate::Negotiate(const Negotiate &other) : Order(other), m_target(other.m_target) {}

Negotiate &Negotiate::operator=(const Negotiate &right)
{
    Order::operator=(right);
    m_target = right.m_target;

    return *this;
}

Order *Negotiate::clone() const
{
    return (new Negotiate(*this));
}

bool Negotiate::validate()
{
    if (m_target == m_player)
    {
        m_effect = "A player cannot negotiate with themselves.";
        cout << "~INVALID ORDER~\n";
        return false;
    }
    if (inNegotiations(m_target))
    {
        m_effect = "Players already in active negotiations.";
        cout << "~INVALID ORDER~\n";
        return false;
    }

    cout << "~order validated~\n";
    return true;
}

void Negotiate::execute()
{
    if (validate())
    {
        m_ceaseFire.emplace_back(m_player, m_target);

        m_effect = "Effect: " + player() + " entered negotiations with " +
                   m_target->getPlayerName() +
                   ".\n\tAttacks between them are invalid for the rest of the turn.";
        cout << m_effect << endl;
    }
    else
        cout << (m_effect = "Effect: No effect. " + m_effect) << endl;

    m_executed = true;
    Notify(this);
    decrementOrderCount(true);
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

    Notify(this);
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
        Order::decrementOrderCount(false);
    }
}

Order *OrdersList::nextOrder(bool deployOnly)
{
    Order *temp = nullptr;
    if (!(m_orders.empty()))
    {
        temp = m_orders.front();

        // only want deploy orders but the next order is not deploy
        if (deployOnly && dynamic_cast<const Deploy *>(temp) == nullptr)
        {
            return nullptr;
        }

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

// assignment operator, first checks if the LHS order list is empty, if not,
// deletes its current Order objects and clears the list, then creates a hard copy of the RHS
// order list and assigns it to the LHS OrdersList
OrdersList &OrdersList::operator=(const OrdersList &right)
{
    if (!(this->m_orders.empty()))
    {
        for (auto &order : m_orders)
            delete order;
        m_orders.clear();
    }

    for (auto &o : right.m_orders)
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
            cout << "    " << i++ << ". " << *order << endl;
        return output;
    }
}

const int OrdersList::size() const
{
    return m_orders.size();
}

// stringToLog
string Order::stringToLog()
{
    string stringLog = "Order has been executed. " + m_effect;
    cout << stringLog << endl;
    return stringLog;
}

// stringToLog
string OrdersList::stringToLog()
{
    string stringLog = m_orders.back()->type() + " Order has been added to the Order List.";
    cout << stringLog << endl;
    return stringLog;
}