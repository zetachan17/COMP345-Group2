#pragma once

#include "LoggingObserver/LoggingObserver.h"

#include <vector>
#include <string>
#include <iostream>
#include <utility>
using std::ostream;
using std::pair;
using std::string;
using std::vector;

class Territory;
class Player;

/// Base Order class representing an order issued by the player to then be executed
class Order : public Subject, public ILoggable
{
public:
    // constructors
    Order();
    Order(Player *player);
    Order(const Order &other);

    // creates new order object identical to this order, returns a pointer to the new order
    virtual Order *clone() const = 0;

    // destructor
    virtual ~Order();

    // assignment operator
    Order &operator=(const Order &right);

    // stream output operator
    friend ostream &operator<<(ostream &output, const Order &order);

    // REQUIRED - verifies if order is valid; subclasses override this for their own verification
    virtual bool validate() = 0;

    // REQUIRED - executes order; checks validity first, each subclass overrides this for their own
    // implementation resulting in some game action depending on the specific order
    virtual void execute() = 0;

    // returns string representation of type of order (Deploy, Advance, Bomb, ...)
    string type() const;

    // returns string of name of player who issued the order
    string player() const;

    // returns string of effect of executing order
    string effect() const;

    bool inNegotiations(Player *target) const;

    // returns vector list of players who can draw a card at the end of the current turn
    static vector<Player *> *getsCard();

    // returns vector list of pair of players in negotiations (i.e. cannot attack each other)
    static vector<pair<Player *, Player *>> *negotiations();

    // decrements the order counter for the round by one
    static void decrementOrderCount(bool inOrderExecutionPhase);

    // stringToLog
    string stringToLog();

protected:
    Player *m_player;
    string m_effect;
    string m_description;
    bool m_executed;
    static int m_orderCount;
    static vector<Player *> m_getsCard;
    static vector<pair<Player *, Player *>> m_ceaseFire;

    // clears negotiations and draws cards for players who conquered at least one territory
    static void turnEnd();
};

/// Tells a certain number of army units taken from the reinforcement pool of the player issuing
/// this order to deploy to one of this player's territories.
class Deploy : public Order
{
public:
    // constructors
    Deploy();
    Deploy(Player *player, int armyUnits, Territory *target);
    Deploy(const Deploy &other);

    // destructor
    ~Deploy();

    // assignment operator
    Deploy &operator=(const Deploy &right);

    // creates new order object identical to this order, returns a pointer to the new order
    Order *clone() const override;

    // if target territory does not belong to the player that issued the order or if the units
    // deployed exceed units remaining in the player's reinforcement pool, the order is invalid
    bool validate() override;

    // validates and executes order and records the resulting effect
    void execute() override;

private:
    int m_units;
    Territory *m_target;
};

/// Tells a certain number of army units to move from a source territory to a target adjacent
/// territory. If the target territory belongs to an enemy player, an attack is triggered.
class Advance : public Order
{
public:
    // constructors
    Advance();
    Advance(Player *player, int units, Territory *source, Territory *target);
    Advance(const Advance &other);

    // destructor
    ~Advance();

    // assignment operator
    Advance &operator=(const Advance &right);

    // creates new order object identical to this order, returns a pointer to the new order
    Order *clone() const override;

    // If the source territory does not belong to the player issuing the order or if the target
    // territory is not adjacent to the source territory, the order is invalid.
    bool validate() override;

    // validates and executes order and records the resulting effect
    void execute() override;

private:
    // returns number of enemy units killed by a given number of opposing units
    int killCount(int units, int probability) const;

    // transfers territory from defending player to attacking player and updates army units
    // after the attack
    void conquer(int attackUnits);

    // simulates battle bettween the active player's attacking units and the target territory's
    // defending units, recording the outcome
    void battle();

    Territory *m_source;
    Territory *m_target;
    int m_units;
};

/// Removes half of the army units from an target enemy territory
class Bomb : public Order
{
public:
    // constructors
    Bomb();
    Bomb(Player *player, Territory *target);
    Bomb(const Bomb &other);

    // destructor
    ~Bomb();

    Territory *target() { return m_target; };
    void setTarget(Territory *target) { m_target = target; }
    // assignment operator
    Bomb &operator=(const Bomb &right);

    // creates new order object identical to this order, returns a pointer to the new order
    Order *clone() const override;

    // If the target belongs to the player that issued the order or is not adjacent to any
    // territories owned by the player issuing the order, the order is invalid.
    bool validate() override;

    // validates and executes order and records the resulting effect
    void execute() override;

private:
    Territory *m_target;
};

/// Doubles the number of army units on the target territory and transfers its ownership from the
/// player issuing the order to the Neutral player.
class Blockade : public Order
{
public:
    // constructors
    Blockade();
    Blockade(Player *player, Territory *target);
    Blockade(const Blockade &other);

    // destructor
    ~Blockade();

    // assignment operator
    Blockade &operator=(const Blockade &right);

    // creates new order object identical to this order, returns a pointer to the new order
    Order *clone() const override;

    // If the target territory does not belong to the player issuing the order, the order is
    // declared invalid.
    bool validate() override;

    // validates and executes order and records the resulting effect
    void execute() override;

private:
    Territory *m_target;
};

/// Tells a certain number of army units taken from a source territory to be moved to a target
/// territory, the source and the target territory being owned by the player issuing the order.
class Airlift : public Order
{
public:
    // constructors
    Airlift();
    Airlift(Player *player, int units, Territory *source, Territory *target);
    Airlift(const Airlift &other);

    // destructor
    ~Airlift();

    // assignment operator
    Airlift &operator=(const Airlift &right);

    // creates new order object identical to this order, returns a pointer to the new order
    Order *clone() const override;

    // If the source or target territory does not belong to the player that issued the order, the
    // order is declared invalid.
    bool validate() override;

    // validates and executes order and records the resulting effect
    void execute() override;

private:
    Territory *m_source;
    Territory *m_target;
    int m_units;
};

/// Results in an enemy player and the player issuing the order to not be able to successfully
/// attack each others’ territories for the remainder of the turn.
class Negotiate : public Order
{
public:
    // constructors
    Negotiate();
    Negotiate(Player *player, Player *target);
    Negotiate(const Negotiate &other);

    // destructor
    ~Negotiate();   

    // assignment operator
    Negotiate &operator=(const Negotiate &right);

    // creates new order object identical to this order, returns a pointer to the new order
    Order *clone() const override;

    // If the target is the player issuing the order, then the order is declared invalid.
    bool validate() override;

    // validates and executes order and records the resulting effect
    void execute() override;

private:
    Player *m_target;
};

/// OrdersList manages a list of Order objects representing the sequential orders issued by a
/// player. Orders can be added, moved around, removed, and executed.
class OrdersList : public Subject, public ILoggable
{
public:
    // constructors
    OrdersList();
    OrdersList(const OrdersList &other);

    // destructor
    virtual ~OrdersList();

    // assignment operator
    OrdersList &operator=(const OrdersList &ordersList);

    // stream output operator
    friend ostream &operator<<(ostream &output, const OrdersList &ordersList);

    // adds an order to the list
    void addOrder(Order *newOrder);

    // moves an order to a different position in the list; first int parameter represents the
    // current position of the order to be moved, second int indicates the target position
    void move(int position, int newPosition);

    // removes an order from the list, int parameter indicates position of order to remove
    void remove(int position);

    // returns the next order and removes it from the list
    // if deployOnly is true, returns the next order if it is Deploy or returns nullptr
    Order *nextOrder(bool deployOnly = false);

    // stringToLog
    string stringToLog();

private:
    vector<Order *> m_orders;
};
