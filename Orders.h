#pragma once
#include <vector>
#include <string>
using std::string;
using std::vector;

class Order
{
public:
    // friend ostream& operator << (ostream& output, Order& o);
    Order(string name);
    bool validate();
    void execute();
    string getDescription();

private:
    string description;
};

class Deploy : public Order
{
public:
    Deploy(string name);
    bool validate();
    void execute();
};

class Advance : public Order
{
public:
    Advance(string name);
    bool validate();
    void execute();
};

class Bomb : public Order
{
public:
    Bomb(string name);
    bool validate();
    void execute();
};

class Blockade : public Order
{
public:
    Blockade(string name);
    bool validate();
    void execute();
};

class Airlift : public Order
{
public:
    Airlift(string name);
    bool validate();
    void execute();
};

class Negotiate : public Order
{
public:
    Negotiate(string name);
    bool validate();
    void execute();
};

class OrdersList
{
public:
    void issue(Order newOrder);
    void move(int pos, int newPos);
    void remove(int pos);
    void output();

private:
    vector<Order> m_Orders;
};
