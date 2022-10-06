#pragma once
#include <vector>
#include <string>
using std::string;
using std::vector;

class Order
{
public:
    Order(string name);
    bool validate();
    void execute();
    string getName() const;

private:
    string name;
};

class Deploy : public Order
{
public:
    Deploy();
    bool validate();
    void execute();
};

class Advance : public Order
{
public:
    Advance();
    bool validate();
    void execute();
};

class Bomb : public Order
{
public:
    Bomb();
    bool validate();
    void execute();
};

class Blockade : public Order
{
public:
    Blockade();
    bool validate();
    void execute();
};

class Airlift : public Order
{
public:
    Airlift();
    bool validate();
    void execute();
};

class Negotiate : public Order
{
public:
    Negotiate();
    bool validate();
    void execute();
};

class OrdersList
{
public:
    void issue(Order *newOrder);
    void move(int pos, int newPos);
    void remove(int pos);
    void output() const;

private:
    vector<Order *> m_Orders;
};