#pragma once

#include <vector>
#include<string>
#include<iostream>
using std::ostream;
using std::string;
using std::vector;

class TerritoryStub
{
public:
	string name;

	TerritoryStub(string name);
};

class OrderStub
{
public:
	string name;

	OrderStub(string name);
};

class OrdersListStub
{
public:
	vector<OrderStub*> orders;

	OrdersListStub(vector<OrderStub*> orders);
};

class CardStub
{
public:
	string name;

	CardStub(string name);
};

class HandStub
{
public:
	vector<CardStub*> cards;

	HandStub(vector<CardStub*> cards);
};

class Player
{
public:
	Player(); //default constructor

	Player(vector<TerritoryStub*> territories, vector<CardStub*> cards, vector<OrderStub*> orders); // parameterized constructor
	
	Player(const Player& player); // copy constructor

	~Player(); // destructor

	Player& operator=(const Player& otherPlayer); //assignment operator

	friend ostream& operator<<(ostream& output, const Player& player); //output stream operator

	void toDefend(); // lists territories that Player owns and needs to defend

	void toAttack(); // lists territories that the Player is able to attack

	// issues an order by adding it to the player's ordersList.
	// if the order requires a card, the player's cards are checked before issuing the order
	// the card is then removed from the player's cards
	void issueOrder(string orderName); 

//private:
	vector<TerritoryStub*> territories;
	HandStub* hand;
	OrdersListStub* ordersList;
};



