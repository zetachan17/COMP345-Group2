#include "Player/Player.h"
#include <iostream>
using std::cout;
using std::endl;

//stub classes
OrderStub::OrderStub(string name)
{
	this->name = name;
}

OrdersListStub::OrdersListStub(vector<OrderStub*> orders)
{
	this->orders = orders;
}

//OrdersListStub& OrdersListStub::operator=(const OrdersListStub& otherOrdersList)
//{
//	this->orders = vector<OrderStub*>(otherOrdersList.orders.size());
//	for (OrderStub* order : otherOrdersList.orders)
//	{
//		this->orders.push_back(new OrderStub(order->name));
//	}
//
//	return *this;
//}

TerritoryStub::TerritoryStub(string name)
{
	this->name = name;
}

CardStub::CardStub(string name)
{
	this->name = name;
}

HandStub::HandStub(vector<CardStub*> cards)
{
	this->cards = cards;
}

//HandStub& HandStub::operator=(const HandStub& otherHand)
//{
//	this->cards = vector<CardStub*>(otherHand.cards.size());
//	for (CardStub* card : otherHand.cards)
//	{
//		this->cards.push_back(new CardStub(card->name));
//	}
//
//	return *this;
//}

Player::Player()
{

}

Player::Player(vector<TerritoryStub*> territories, vector<CardStub*> cards, vector<OrderStub*> orders)
{
	this->territories = territories;
	this->hand = new HandStub(cards);
	this->ordersList = new OrdersListStub(orders);
}

Player::Player(const Player &otherPlayer)
{
	vector<CardStub*> cards;
	for (CardStub* card : otherPlayer.hand->cards)
	{
		cards.push_back(new CardStub(card->name));
	}


	vector<OrderStub*> orders;
	for (OrderStub* order : otherPlayer.ordersList->orders)
	{
		orders.push_back(new OrderStub(order->name));
	}

	vector<TerritoryStub*> territories;
	for (TerritoryStub* territory : otherPlayer.territories)
	{
		territories.push_back(new TerritoryStub(territory->name));
	}

	this->territories = territories;
	this->hand = new HandStub(cards);
	this->ordersList = new OrdersListStub(orders);
}

Player::~Player() 
{
	territories.clear();
	hand->cards.clear();
	ordersList->orders.clear();
	delete hand;
	delete ordersList;
}

Player &Player::operator=(const Player &otherPlayer)
{
	vector<CardStub*> cards;
	for (CardStub* card : otherPlayer.hand->cards)
	{
		cards.push_back(new CardStub(card->name));
	}


	vector<OrderStub*> orders;
	for (OrderStub* order : otherPlayer.ordersList->orders)
	{
		orders.push_back(new OrderStub(order->name));
	}

	vector<TerritoryStub*> territories;
	for (TerritoryStub* territory : otherPlayer.territories)
	{
		territories.push_back(new TerritoryStub(territory->name));
	}

	this->territories = territories;
	this->hand = new HandStub(cards);
	this->ordersList = new OrdersListStub(orders);

	return *this;
}

ostream& operator<<(ostream& output, const Player& player)
{
	output << "Player owned territories:" << endl;
	for (TerritoryStub* territory : player.territories)
	{
		output << "\t" << territory->name << endl;
	}

	output << "Player cards in hand:" << endl;
	for (CardStub* card : player.hand->cards)
	{
		output << "\t" << card->name << endl;
	}

	output << "Player orders list:" << endl;
	for (OrderStub* order : player.ordersList->orders)
	{
		output << "\t" << order->name << endl;
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
		cout << "\t" << territories[i]->name << endl;
}

vector<string> cardOrders{ "bomb", "blockade", "airlift", "diplomacy" };

void Player::issueOrder(string orderName)
{
	cout << "Handling order: " << orderName << endl;

	bool isCardOrder = (std::find(cardOrders.begin(), cardOrders.end(), orderName) != cardOrders.end());


	if (isCardOrder)
	{
		cout << "This order requires a card." << endl;

		std::vector<CardStub*>::iterator it = (std::find_if(hand->cards.begin(), hand->cards.end(), [orderName](CardStub* card) { return card->name == orderName; }));

		bool isCardInHand = it != hand->cards.end();

		if (isCardInHand)
		{
			cout << "Removing card from the player's hand." << endl;
			hand->cards.erase(it);
			cout << "Issuing order" << endl;
			this->ordersList->orders.push_back(new OrderStub(orderName));
		}
		else {
			cout << "The player does not possess the card." << endl;
		}
	}
	else {
		cout << "Issuing order" << endl;
		this->ordersList->orders.push_back(new OrderStub(orderName));
	}
}




