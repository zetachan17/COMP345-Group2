#include "GameEngine/GameEngineDriver.h"
#include "GameEngine/GameEngine.h"

void testGameStates()
{
    State currentState = Start;
    while (currentState != End)
    {
        currentState = GameEngine::StartGame(currentState);
    }
    
}
