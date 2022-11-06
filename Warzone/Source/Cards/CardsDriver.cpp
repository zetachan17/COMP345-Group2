#include "Cards/CardsDriver.h"
#include "Cards/Cards.h"
#include <iostream>

void testCards()
{

	std::cout << "Assignment 1, Part 4: Cards/Deck/Hand" << std::endl;

	std::cout << "Creating deck pointer." << std::endl;
	Deck *ptrD1 = new Deck();

	std::cout << "Creating hand pointer." << std::endl;
	Hand *ptrH1 = new Hand();

	std::cout << "Creating card pointer." << std::endl;
	Cards *ptrC1 = new Cards();

	std::cout << "Creating Deck." << std::endl;
	ptrD1->createDeck();

	std::cout << "Printing Deck before drawing cards:" << std::endl;
	ptrD1->printDeck();

	std::cout << "Printing Hand before drawing cards:" << std::endl;
	ptrH1->printHand();

	std::cout << "Drawing and adding card to hand (10 times)." << std::endl;
	for (int i = 0; i < 10; i++)
	{
		ptrH1->addToHand(ptrD1->draw());
	}

	std::cout << "Printing Deck after drawing cards:" << std::endl;
	ptrD1->printDeck();

	std::cout << "Printing Hand after drawing cards:" << std::endl;
	ptrH1->printHand();

	std::cout << "Playing cards (all 10)." << std::endl;
	for (int i = 0; i < 10; i++)
	{
		//commented out for order list
		//ptrC1->play(ptrH1->playCard(ptrD1));
	}

	std::cout << "Printing Deck after playing cards." << std::endl;
	ptrD1->printDeck();

	std::cout << "Printing Hand after playing cards." << std::endl;
	ptrH1->printHand();

	// deleting to avoid memory leaks
	delete ptrD1;
	delete ptrH1;
	delete ptrC1;
}