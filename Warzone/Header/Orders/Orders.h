#pragma once
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
    Order(string type); // constructor, sets the type of order to m_type

    Order(const Order &other); // copy constructor

    virtual Order *clone() const; // clones() creates a new order object identical to this
                                  // order and returns a pointer to the new order

    // verifies if the order is valid, each subclass overrides this function for their
    // own validity check
    virtual bool validate() const;

    // executes order, checks validity first, each subclass overrides this function for their own
    // individual implementation resulting in some game action depending on the specific order
    virtual void execute();

    // overloaded assignment operator, returns a copy of the RHS Order object 
    Order &operator=(const Order &rightSide); 

    // overloaded stream insertion operator, prints the order type (Deploy, Bomb, Advance, ...)
    friend ostream &operator<<(ostream &output, const Order &order);

    virtual ~Order(); // destructor

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
class OrdersList
{
public:
    OrdersList(); // default constructor

    OrdersList(const OrdersList &other); // copy constructor

    void issue(Order *newOrder); // adds an order to list

    // moves an order to a different position in the list, first int parameter represents the
    // current position of the order th ebe moved, second int indicated the target position
    void move(int pos, int newPos);

    // removes an order from the list, int parameter indicates position of order to remove
    void remove(int pos);

    // executes the next order and then removes it from the list
    void executeNextOrder();

    // overloaded assignment operator returning a hard copy of the RHS OrdersList object
    OrdersList &operator=(const OrdersList &rightSide);

    // overloaded stream insertion operator returning a numbered list representation of the
    // current orders in the list.
    friend ostream &operator<<(ostream &output, const OrdersList &orders);

    ~OrdersList(); // destructor

private:
    vector<Order *> m_orders;
};
