// Warzone.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "PlayerStrategies/PlayerStrategiesDriver.h"
#include <iostream>

int main()
{
    //this seed is necessary for rand to behave properly
    srand(time(0));

    //P1
    testPlayerStrategies();
    //P2
    //testStartupPhase();
}
