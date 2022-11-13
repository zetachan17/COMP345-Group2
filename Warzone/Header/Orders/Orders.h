#pragma once

class Territory;
class Player;

#include <vector>
#include <string>
#include <iostream>
#include <utility>
using std::ostream;
using std::pair;
using std::string;
using std::vector;

/// Base Order class representing an order issued by the player to then be executed
class Order
{
public:
    // constructors
    Order();
    Order(Player *player);
    Order(const Order &other);

    // clone() creates new order object identical to this order, returns a pointer to the new order
    virtual Order *clone() const = 0;

    // destructor
    virtual ~Order();

    // assignment operator
    Order &operator=(const Order &rightSide);

    // stream output operator
    friend ostream &operator<<(ostream &output, const Order &order);

    // REQUIRED - verifies if order is valid; subclasses override this for their own verification
    virtual bool validate() = 0;

    // REQUIRED - executes order; checks validity first, each subclass overrides this for their own
    // implementation resulting in some game action depending on the specific order
    virtual void execute() = 0;

    string type() const;

    string player() const;

    string effect() const;

    void turnEnd();

    static vector<Player *> *getsCard();

    static vector<pair<Player *, Player *>> *negotiations();

    static Player *neutralPlayer();

    static void decrementOrderCount();

protected:
    Player *m_player;
    string m_effect;
    string m_description;

    static int m_orderCount;
    static vector<Player *> m_getsCard;
    static vector<pair<Player *, Player *>> m_ceaseFire;
    static Player *m_neutralPlayer;
};

/// Order subclass representing a Deploy order
class Deploy : public Order
{
public:
    Deploy();
    Deploy(Player *player, int armyUnits, Territory *target);

    Order *clone() const override;
    bool validate() override;
    void execute() override;
    ~Deploy();

private:
    int m_units;
    Territory *m_target;
};

/// Order subclass representing an Advance order
class Advance : public Order
{
public:
    Advance();
    Advance(Player *player, int units, Territory *source, Territory *target);

    Order *clone() const override;
    bool validate() override;
    void execute() override;
    ~Advance();

private:
    int killCount(int units, int probability) const;
    void conquer();
    void battle();

    Territory *m_source;
    Territory *m_target;
    bool m_negotiate;
    int m_units;
};

/// Order subclass representing a Bomb order
class Bomb : public Order
{
public:
    Bomb();
    Bomb(Player *player, Territory *target);

    ~Bomb();

    Order *clone() const override;
    bool validate() override;
    void execute() override;

private:
    Territory *m_target;
    bool m_negotiate;
};

/// Order subclass representing a Blockade order
class Blockade : public Order
{
public:
    Blockade();
    Blockade(Player *player, Territory *target);

    Order *clone() const override;
    bool validate() override;
    void execute() override;
    ~Blockade();

private:
    Territory *m_target;
};

/// Order subclass representing an Airlift order
class Airlift : public Order
{
public:
    Airlift();
    Airlift(Player *player, int units, Territory *source, Territory *target);

    Order *clone() const override;
    bool validate() override;
    void execute() override;
    ~Airlift();

private:
    Territory *m_source;
    Territory *m_target;
    int m_units;
};

/// Order subclass representing a Negotiate order
class Negotiate : public Order
{
public:
    Negotiate();
    Negotiate(Player *player, Player *target);

    Order *clone() const override;
    bool validate() override;
    void execute() override;
    ~Negotiate();

private:
    Player *m_target;
};

/// OrdersList manages a list of Order objects representing the sequential orders issued by a
/// player. Orders can be added, moved around, removed, and executed.
class OrdersList
{
public:
    // constructors
    OrdersList();
    OrdersList(const OrdersList &other);

    // destructor
    ~OrdersList();

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
    Order *nextOrder();

    const vector<Order *> *orders() const;

private:
    vector<Order *> m_orders;
};
