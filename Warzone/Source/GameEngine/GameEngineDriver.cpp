#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"

void testGameStates()
{
    GameEngine* gameEngine = new GameEngine();
    GameEngine::State currentState = GameEngine::State::Start;
    
    while (currentState != GameEngine::State::End)
    {
        currentState = gameEngine->StartEngine(currentState);
    }

    delete gameEngine;
}
