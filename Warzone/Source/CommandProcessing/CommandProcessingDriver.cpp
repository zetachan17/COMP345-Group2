#include "CommandProcessing/CommandProcessingDriver.h"
#include "CommandProcessing/CommandProcessing.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/GameEngineDriver.h"

void testCommandProcessor()
{
    CommandProcessor* commandProcessor = NULL;
    GameEngine::State currentState = GameEngine::State::End;

    while (commandProcessor == NULL || currentState == GameEngine::State::End) {
        GameEngine* gameEngine = new GameEngine();
        currentState = GameEngine::State::Start;
        CommandProcessor* commandProcessor = gameEngine->initializeCommandProcessor();


        // if(commandProcessor != NULL){
        while (currentState != GameEngine::State::End)
        {
            currentState = gameEngine->startupPhase(gameEngine->getState(), commandProcessor);
        }
        if (commandProcessor != NULL)
        {
            delete commandProcessor;
            commandProcessor = NULL;
            delete gameEngine;
            gameEngine = NULL;
        }

    }
}