#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "LoggingObserver/LoggingObserver.h"
using std::ostream;
using std::string;
using std::vector;

/// Base Order class representing an order issued by the player to then be executed
class Order : public Subject, public ILoggable
{
public:
    // constructors
    Order(string type); 
    Order(const Order &other);

    // clones() creates a new order object identical to this
    // order and returns a pointer to the new order
    virtual Order* clone() const = 0;

    // destructor
    virtual ~Order();

    // assignment operator
    Order& operator=(const Order& rightSide);

    // stream output operator
    friend ostream& operator<<(ostream& output, const Order& order);
    
    // REQUIRED - verifies if the order is valid, each subclass overrides this function for their
    // own validity check
    virtual bool validate() const = 0;

    // REQUIRED - executes order, checks validity first, each subclass overrides this function for their own
    // individual implementation resulting in some game action depending on the specific order
    virtual void execute() = 0;

    //stringToLog
    string stringToLog();

private:
    string m_type;
};

/// Order subclass representing a Deploy order
class Deploy : public Order
{
public:
    Deploy();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Deploy();
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
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Bomb();
};

/// Order subclass representing a Blockade order
class Blockade : public Order
{
public:
    Blockade();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Blockade();
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
class OrdersList : public Subject, public ILoggable
{
public:
    // constructors
    OrdersList();
    OrdersList(const OrdersList &other);

    // destructor
    virtual ~OrdersList();
    
    // assignment operator
    OrdersList& operator=(const OrdersList& ordersList);

    // stream output operator
    friend ostream& operator<<(ostream& output, const OrdersList& ordersList);

    // adds an order to list
    void addOrder(Order *newOrder);

    // moves an order to a different position in the list, first int parameter represents the
    // current position of the order th ebe moved, second int indicated the target position
    void move(int position, int newPosition);

    // removes an order from the list, int parameter indicates position of order to remove
    void remove(int position);

    // executes the next order and then removes it from the list
    void executeNextOrder();

    //stringToLog
    string stringToLog();

private:
    vector<Order*> m_orders;
};
