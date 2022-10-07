#pragma once
#include <vector>
#include <string>
#include <iostream>
using std::ostream;
using std::string;
using std::vector;

class Order
{
public:
    Order(string name);
    virtual bool validate();
    virtual void execute();
    friend ostream &operator<<(ostream &output, const Order *o);

private:
    string name;
};

class Deploy : public Order
{
public:
    Deploy();
    bool validate() const;
    void execute();
};

class Advance : public Order
{
public:
    Advance();
    bool validate() const;
    void execute();
};

class Bomb : public Order
{
public:
    Bomb();
    bool validate() const;
    void execute();
};

class Blockade : public Order
{
public:
    Blockade();
    bool validate() const;
    void execute();
};

class Airlift : public Order
{
public:
    Airlift();
    bool validate() const;
    void execute();
};

class Negotiate : public Order
{
public:
    Negotiate();
    bool validate() const;
    void execute();
};

class OrdersList
{
public:
    void issue(Order *newOrder);
    void move(int pos, int newPos);
    void remove(int pos);
    void executeNextOrder();
    friend ostream &operator<<(ostream &output, const OrdersList *oList);

private:
    vector<Order *> m_Orders;
};
