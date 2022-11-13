#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"
#include <string>
#include <iostream>

void testStartupPhase()
{
    CommandProcessor* commandProcessor = NULL;
    GameEngine::State currentState = GameEngine::State::End;
   
    while (commandProcessor == NULL|| currentState == GameEngine::State::End) {
        GameEngine* gameEngine = new GameEngine();
        currentState = GameEngine::State::Start;
        CommandProcessor* commandProcessor = gameEngine->initializeCommandProcessor();
        while (currentState != GameEngine::State::End)
        {
            currentState = gameEngine->startupPhase(gameEngine->getState(), commandProcessor);
        }
        if (commandProcessor != NULL) 
        {
            delete commandProcessor;
            delete gameEngine;
        }
       
    }
    
   
}

