#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"
#include <string>
#include <iostream>

void testStartupPhase()
{
    GameEngine* gameEngine = new GameEngine();
    CommandProcessor* cmdP = gameEngine->initializeCommandProcessor();
    GameEngine::State currentState = GameEngine::State::Start;
    
    while (currentState != GameEngine::State::End)
    {

        currentState = gameEngine->startupPhase(gameEngine->getState(), cmdP);
    }
    
    delete gameEngine;
    
}
