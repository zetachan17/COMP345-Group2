#include "Player/Player.h"
#include "Player/PlayerDriver.h"

#include <iostream>
using std::cout;
using std::endl;
void testPlayers()
{
    cout << "** PART 2: PLAYERS **\n\n";

    Player* player = new Player();
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

    delete player;
    delete player2;
}
