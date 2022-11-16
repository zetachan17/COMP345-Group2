#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"
#include <string>
#include <iostream>

void testStartupPhase()
{
    CommandProcessor* commandProcessor = nullptr;
    GameEngine::State currentState = GameEngine::State::End;
   
    while (commandProcessor == nullptr|| currentState == GameEngine::State::End) {
        GameEngine* gameEngine = new GameEngine();
        currentState = GameEngine::State::Start;
        commandProcessor = gameEngine->initializeCommandProcessor();
        while (currentState != GameEngine::State::End)
        {
            currentState = gameEngine->startupPhase(gameEngine->getState(), commandProcessor);
        }
        if (commandProcessor != nullptr) 
        {
            delete commandProcessor;
            delete gameEngine;
            break;
        }
    }
}

void testMainGameLoop()
{
    cout << "----------------------------------------------------------\n"
        << "----------------------------------------------------------\n"
        << "\t** PART 3: MAIN GAME LOOP**\n\n";

    CommandProcessor* commandProcessor = nullptr;
    GameEngine::State currentState = GameEngine::State::End;

    while (commandProcessor == nullptr || currentState == GameEngine::State::End) {
        GameEngine* gameEngine = new GameEngine();
        currentState = GameEngine::State::Start;
        commandProcessor = gameEngine->initializeCommandProcessor();
        while (currentState != GameEngine::State::End)
        {
            currentState = gameEngine->startupPhase(gameEngine->getState(), commandProcessor);
        }
        if (commandProcessor != nullptr)
        {
            delete commandProcessor;
            delete gameEngine;
        }
    }
}
