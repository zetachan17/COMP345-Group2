#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "Orders/Orders.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;

void testTournament()
{
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
            break;
        }
    }
}