#include "Player/Player.h"
#include "Player/PlayerDriver.h"

#include <iostream>
using std::cout;
using std::endl;
void testPlayers()
{
    cout << "** PART 2: PLAYERS **\n\n";

    vector<TerritoryStub*> playerTerritories{ new TerritoryStub("ireland"), new TerritoryStub("bresil"), new TerritoryStub("morocco") };
    vector<CardStub*> playerCards{ new CardStub("bomb"), new CardStub("airlift"), new CardStub("diplomacy") };
    vector<OrderStub*> playerOrdersList;

    Player* player = new Player(playerTerritories, playerCards, playerOrdersList);
    cout << "*Created player*\n" << *player << endl;

    // testing toDefend()
    cout << "*Calling toDefend*" << endl;
    player->toDefend();
    cout << endl;

    // testing toAttack()
    cout << "*Calling toAttack*" << endl;
    player->toAttack();
    cout << endl;

    //testing issueOrder()
    cout << "*Testing issueOrder with various orders*\n" << endl;
    player->issueOrder("airlift");
    cout << "\nCurrent player state: \n" << *player << endl;
    cout << endl;

    player->issueOrder("bomb");
    cout << "\nCurrent player state: \n" << *player << endl;
    cout << endl;

    player->issueOrder("blockade");
    cout << "\nCurrent player state: \n" << *player << endl;
    cout << endl;

    player->issueOrder("diplomacy");
    cout << "\nCurrent player state: \n" << *player << endl;
    cout << endl;

    player->issueOrder("advance");
    cout << "\nCurrent player state: \n" << *player << endl;
    cout << endl;

    player->issueOrder("deploy");
    cout << "\nCurrent player state: \n" << *player << endl;
    cout << endl;

    //testing copy constructor
    cout << "*Calling copy constructor*" << endl;
    Player* player2 = new Player(*player);

    cout << "Copied player object, player2:" << endl;
    cout << *player2 << endl;

    //testing asignment operator
    cout << "*Calling assignment operator*" << endl;
    Player player3 = *player2;

    cout << "Copied by assignment player3" << endl;
    cout << player3 << endl;

    cout << *player << endl;

    cout << "Demonstrating deep copies by assigning new cards to player2:" << endl;
    vector<CardStub*> newcards{ new CardStub("blockade"), new CardStub("blockade"), new CardStub("blockade") };
    HandStub* newhand = new HandStub(newcards);

    cout << "Player states before:" << endl;
    cout << "--player1-- \n" << *player << "\n" << endl;
    cout << "--player2-- \n" << *player2 << "\n" << endl;
    cout << "--player3-- \n" << player3 << "\n" << endl;

    player2->hand = newhand;

    cout << "Player states after:" << endl;
    cout << "--player1-- \n" << *player << "\n" << endl;
    cout << "--player2-- \n" << *player2 << "\n" << endl;
    cout << "--player3-- \n" << player3 << "\n" << endl;

    delete player;
    delete player2;
}
