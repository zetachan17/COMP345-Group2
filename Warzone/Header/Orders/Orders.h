#pragma once

class Territory;
class Player;

#include <vector>
#include <string>
#include <iostream>
using std::ostream;
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
    Order(const string &type); // to delete when all subclasses are implemented

    // clone() creates new order object identical to this order, returns a pointer to the new order
    virtual Order *clone() const = 0;

    // destructor
    virtual ~Order();

    // assignment operator
    Order &operator=(const Order &rightSide);

    // stream output operator
    friend ostream &operator<<(ostream &output, const Order &order);

    // REQUIRED - verifies if order is valid; subclasses override this for their own verification
    virtual bool validate() const = 0;

    // REQUIRED - executes order; checks validity first, each subclass overrides this for their own
    // implementation resulting in some game action depending on the specific order
    virtual void execute() = 0;

    static Player *neutralPlayer();

protected:
    Player *m_player;
    string m_effect;
    string m_description;
    static Player *m_neutralPlayer;
};

/// Order subclass representing a Deploy order
class Deploy : public Order
{
public:
    Deploy(Player *player, int armyUnits, Territory *target);

    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Deploy();

private:
    int m_units;
    Territory *m_territory;
};

/// Order subclass representing an Advance order
class Advance : public Order
{
public:
    Advance();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Advance();
};

/// Order subclass representing a Bomb order
class Bomb : public Order
{
public:
    Bomb();
    Bomb(Player *player, Territory* target);

    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Bomb();

private:
    Territory* m_territory;
};

/// Order subclass representing a Blockade order
class Blockade : public Order
{
public:
    Blockade();
    Blockade(Player *player, Territory *target);

    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Blockade();

private:
    Territory *m_territory;
};

/// Order subclass representing an Airlift order
class Airlift : public Order
{
public:
    Airlift();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Airlift();
};

/// Order subclass representing a Negotiate order
class Negotiate : public Order
{
public:
    Negotiate();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Negotiate();
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

private:
    vector<Order *> m_orders;
};
