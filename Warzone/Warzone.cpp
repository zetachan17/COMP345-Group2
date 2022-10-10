// Warzone.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Map/MapDriver.h"
#include "Player/PlayerDriver.h"
#include "Orders/OrdersDriver.h"
#include "Cards/CardsDriver.h"
#include "GameEngine/GameEngineDriver.h"

int main()
{
    //P1
    testLoadMaps();
    //P2
    testPlayers();
    //P3
    testOrdersLists();
    //P4
    testCards();
    //P5
    testGameStates();

}
