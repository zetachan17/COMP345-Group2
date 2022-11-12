#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"
#include <string>
#include <iostream>

void testStartupPhase()
{
    GameEngine* gameEngine = new GameEngine();
    CommandProcessor* cmdP = new CommandProcessor();
    //FileLineReader* filelineReader = new FileLineReader();
    //FileCommandProcessorAdapter* fileCommandProcessorAdapter = new FileCommandProcessorAdapter(filelineReader);
    GameEngine::State currentState = GameEngine::State::Start;
    
    while (currentState != GameEngine::State::End)
    {
        std::cout << "Welcome to Warzone! \n" << endl;
        std::cout << "Would you like to play using the command line or a prepared text file?\n" << endl;
        std::cout << "Please enter ";

        currentState = gameEngine->startupPhase(gameEngine->getState(), cmdP);
    }

    delete gameEngine;
}
