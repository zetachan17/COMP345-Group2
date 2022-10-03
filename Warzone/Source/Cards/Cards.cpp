#include "Cards/Cards.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

//array to convert number of card to its respective type
std::string cardsType[5] {
	//0=bomb, 1=reinforcement, 2=blockade, 3=airlift,  4=diplomacy
	"bomb",
	"reinforcement",
	"blockade",
	"airlift",
	"diplomacy"
};

//nbOfCards is the number of cards per type. 5 is the number of types there are
int nbOfCards = 3;
int totalCards = nbOfCards * 5;

std::vector<Cards> deckOfCards;
std::vector<Cards> cardsInHand;

void Deck::createDeck() {

	
	//Deck* ptrDeck = &deckOfCards;

	int counter = 0;
	for (int i = 0; i < totalCards; i++) {
		switch (counter) {
			case 0: {
				Cards c;
				Cards* ptrC = &c;
				ptrC->type = 0;
				deckOfCards.push_back(*ptrC);
				counter++;
				break;
			}
			case 1: {
				Cards c;
				Cards* ptrC = &c;
				ptrC->type = 1;
				deckOfCards.push_back(*ptrC);
				counter++;
				break;
			}
			case 2: {
				Cards c;
				Cards* ptrC = &c;
				ptrC->type = 2;
				deckOfCards.push_back(*ptrC);
				counter++;
				break;
			}
			case 3: {
				Cards c;
				Cards* ptrC = &c;
				ptrC->type = 3;
				deckOfCards.push_back(*ptrC);
				counter++;
				break;
			}
			case 4: {
				Cards c;
				Cards* ptrC = &c;
				ptrC->type = 4;
				deckOfCards.push_back(*ptrC);
				counter = 0;
				break;
			}
		}

	}

	//shuffle the deck of cards
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(deckOfCards.begin(), deckOfCards.end(), g);

	//print the deck of cards
	/*for (int i = 0; i < deckOfCards.size();i++) {
		std::cout << cardsType[deckOfCards[i].type] << std::endl;;
	}

	std::cout << "----";*/
	
}

//draw card randomly from the deck and place it in hand
void Deck::draw() {

	//take the top card of the deck and place it in your hand, then remove it from the deck
	cardsInHand.push_back(deckOfCards[0]);
	deckOfCards.erase(deckOfCards.begin());
	std::cout << "You have drawn " << cardsType[cardsInHand.back().type] << "." << std::endl;

	
}

//Print the contents of the deck
void Deck::printDeck() {
	std::cout << "Content of deck:" << std::endl;
	for (int i = 0; i < deckOfCards.size();i++) {
		std::cout << cardsType[deckOfCards[i].type] << std::endl;;
	}
}

//create a hand that can get filled with cards
void Hand::createHand() {
	
	
}

//Print the contents of the hand
void Hand::printHand() {
	std::cout << "Content of hand:" << std::endl;
	for (int i = 0; i < cardsInHand.size();i++) {
		std::cout << cardsType[cardsInHand[i].type] << std::endl;;
	}
}

//play the card, create an order and add it to the order list, return card back in deck
void Cards::playCard() {

	//create special order
	
	//right now its just playing the cards in order
	deckOfCards.push_back(cardsInHand[0]);
	cardsInHand.erase(cardsInHand.begin());
	std::cout << "You have played " << cardsType[deckOfCards.back().type] << "." << std::endl;
}

//
void Hand::play() {
	playCard();
}

//Create the order after playing a card
void Cards::createOrder() {

}