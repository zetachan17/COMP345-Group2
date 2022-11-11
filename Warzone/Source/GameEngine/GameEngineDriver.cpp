#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"

void testStartupPhase()
{
    GameEngine* gameEngine = new GameEngine();
    GameEngine::State currentState = GameEngine::State::Start;
    
    while (currentState != GameEngine::State::End)
    {
        currentState = gameEngine->startupPhase(currentState);
    }

    delete gameEngine;
}
