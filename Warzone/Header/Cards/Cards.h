#include <string>
#include <vector>

#pragma once

//enum cardsType {
//
//	//0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
//	bomb,
//	reinforcement,
//	blockade,
//	airlift,
//	diplomacy
//};

class Cards {

public:

	//card type: 0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
	int type;
	void playCard();
	void createOrder();

};

class Deck {

public:
	void draw();
	void createDeck();
	void printDeck();

};

class Hand {

public:
	void createHand();
	void printHand();
	void play();
	friend void playCard();

};