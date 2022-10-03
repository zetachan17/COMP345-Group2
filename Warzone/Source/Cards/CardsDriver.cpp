#include "Cards/CardsDriver.h"
#include "Cards/Cards.h"
#include <iostream>

void testCards() {
	std::cout << "Assignment 1, Part 4: Cards deck/hand." << std::endl;

	std::cout << "Creating deck." << std::endl;
	Deck d1;
	Deck* ptrD1 = &d1;
	ptrD1->createDeck();

	std::cout << "Creating hand." << std::endl;
	Hand h1;
	Hand* ptrH1 = &h1;
	ptrH1->createHand();

	std::cout << "Content of deck before drawing cards." << std::endl;
	ptrD1->printDeck();

	std::cout << "Drawing 10 cards." << std::endl;
	for (int i = 0; i < 10; i++) {
		ptrD1->draw();
	}

	std::cout << "Content of deck after drawing cards." << std::endl;
	ptrD1->printDeck();

	std::cout << "Content of hand." << std::endl;
	ptrH1->printHand();

	std::cout << "Playing all 10 cards." << std::endl;
	for (int i = 0; i < 10; i++) {
		ptrH1->play();
	}
	
	std::cout << "Content of deck after playing cards." << std::endl;
	ptrD1->printDeck();
	
}