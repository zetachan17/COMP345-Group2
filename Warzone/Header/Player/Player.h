#pragma once

#include "Orders/Orders.h"
#include <vector>
#include<string>
#include<iostream>
using std::ostream;
using std::string;
using std::vector;

class Player
{
public:
	Player(); //default constructor

	Player(vector<string*> territories, vector<string*> cards, vector<string*> ordersList); // parameterized constructor
	
	Player(const Player& player); // copy constructor

	~Player(); // destructor

	Player& operator=(const Player& otherPlayer); //assignment operator

	friend ostream& operator<<(ostream& output, const Player& player); //output stream operator

	void toDefend(); // lists territories that Player owns and needs to defend

	void toAttack(); // lists territories that the Player is able to attack

	// issues an order by adding it to the player's orderslist.
	// if the order requires a card, the player's cards are checked before issuing the order
	// the card is then removed from the player's cards
	void issueOrder(string orderName); 

//private:
	vector<string*> territories;
	vector<string*> cards;
	vector<string*> ordersList;
};

