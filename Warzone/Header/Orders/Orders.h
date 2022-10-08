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
    Order(string type);
    Order(const Order &other);
    virtual Order *clone() const;
    virtual bool validate() const;
    virtual void execute();
    Order &operator=(const Order &rightSide);
    friend ostream &operator<<(ostream &output, const Order &order);
    virtual ~Order();

private:
    string m_type;
};

class Deploy : public Order
{
public:
    Deploy();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Deploy();
};

class Advance : public Order
{
public:
    Advance();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Advance();
};

class Bomb : public Order
{
public:
    Bomb();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Bomb();
};

class Blockade : public Order
{
public:
    Blockade();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Blockade();
};

class Airlift : public Order
{
public:
    Airlift();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Airlift();
};

class Negotiate : public Order
{
public:
    Negotiate();
    Order *clone() const override;
    bool validate() const override;
    void execute() override;
    ~Negotiate();
};

class OrdersList
{
public:
    OrdersList();
    OrdersList(const OrdersList &other);
    void issue(Order *newOrder);
    void move(int pos, int newPos);
    void remove(int pos);
    void executeNextOrder();
    OrdersList &operator=(const OrdersList &rightSide);
    friend ostream &operator<<(ostream &output, const OrdersList &orders);
    ~OrdersList();

private:
    vector<Order *> m_orders;
};
