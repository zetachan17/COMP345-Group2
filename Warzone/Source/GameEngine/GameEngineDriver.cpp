#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"

void testGameStates()
{
    GameEngine ge;
    GameEngine::State currentState = GameEngine::State::Start;
    
    while (currentState != GameEngine::State::End)
    {
        currentState = ge.StartGame(currentState);
    }
    
}
