#include "Player/Player.h"
#include <iostream>
using std::cout;
using std::endl;

Player::Player()
{

}

Player::Player(vector<string*> territories, vector<string*> cards, vector<string*> ordersList)
{
	this->territories = territories;
	this->cards = cards;
	this->ordersList = ordersList;
}

Player::Player(const Player &otherPlayer)
{
	territories = otherPlayer.territories;
	cards = otherPlayer.cards;
	ordersList = otherPlayer.ordersList;
}

Player::~Player() 
{
	territories.clear();
	cards.clear();
}

Player &Player::operator=(const Player &otherPlayer)
{
	this->territories = otherPlayer.territories;
	this->cards = otherPlayer.cards;
	this->ordersList = otherPlayer.ordersList;

	return *this;

}

ostream& operator<<(ostream& output, const Player& player)
{
	output << "Player owned territories:" << endl;
	for (string* territory : player.territories)
	{
		output << "\t" << * territory << endl;
	}

	output << "Player cards in hand:" << endl;
	for (string* card: player.cards)
	{
		output << "\t" << *card << endl;
	}

	output << "Player orders list:" << endl;
	for (string* order : player.ordersList)
	{
		output << "\t" << *order << endl;
	}

	return output;
}

void Player::toAttack()
{
	cout << "Territories to be attacked:" << endl;
	cout << "(list of territories adjacent to the player's owned territories, not owned by the player) " << endl;
}

void Player::toDefend()
{
	cout << "Territories to be defended:" << endl;
	for (int i = 0; i < territories.size(); i++)
		cout << "\t" << *territories[i] << endl;
}

vector<string> cardOrders{ "bomb", "blockade", "airlift", "diplomacy" };

void Player::issueOrder(string orderName)
{
	cout << "Handling order: " << orderName << endl;

	bool isCardOrder = (std::find(cardOrders.begin(), cardOrders.end(), orderName) != cardOrders.end());


	if (isCardOrder)
	{
		cout << "This order requires a card." << endl;

		std::vector<string*>::iterator it = (std::find_if(cards.begin(), cards.end(), [orderName](string* order) { return *order == orderName; }));

		bool isCardInHand = it != cards.end();
		
		if (isCardInHand)
		{
			cout << "Removing card from the player's hand." << endl;
			cards.erase(it);
			cout << "Issuing order" << endl;
			this->ordersList.push_back(new string(orderName));
		} 
		else {
			cout << "The player does not possess the card." << endl;
		}
	}
	else {
		cout << "Issuing order" << endl;
		this->ordersList.push_back(new string(orderName));
	}
}




