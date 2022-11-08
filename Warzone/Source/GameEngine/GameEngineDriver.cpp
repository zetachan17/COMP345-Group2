#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"

void testGameStates()
{
    GameEngine* gameEngine = new GameEngine();
    CommandProcessor* cmdP = new CommandProcessor();
    GameEngine::State currentState = GameEngine::State::Start;
    
    while (currentState != GameEngine::State::End)
    {
        currentState = gameEngine->StartGame(gameEngine->getState(), cmdP);
    }

    delete gameEngine;
}
