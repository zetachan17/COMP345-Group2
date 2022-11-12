#include "LoggingObserver/LoggingObserverDriver.h"
#include "LoggingObserver/LoggingObserver.h"
#include "Orders/Orders.h"
#include "CommandProcessing/CommandProcessing.h"
#include "GameEngine/GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
using std::cout;
using std::endl;
using std::string;

void testLoggingObserver() {

    cout << "Assignment 2, Part 5: Game Logger" << endl;

    cout << "Clearing the contents of the gamelog for this new iteration of the game." << endl;
    std::ofstream gameLog;
    gameLog.open("gamelog.txt", std::fstream::out | std::fstream::trunc);
    gameLog << "Output of commands:\n";
    gameLog.close();

    cout << "\nCommandProcessor::saveCommand() being added to log file:" << endl;

    /*CommandProcessor* saveCommand = new CommandProcessor;
    LogObserver* view1 = new LogObserver(saveCommand);
    saveCommand->getCommand();

    delete saveCommand;
    delete view1;*/

    cout << "\nOrder::execute() being added to log file:" << endl;

    //D* execute = new D();
    //LogObserver* view2 = new LogObserver(execute);

    //delete execute;
    //delete view2;

    cout << "\nCommand::saveEffect() being added to log file:" << endl;

    /*GameEngine* gameEngine = new GameEngine();
    CommandProcessor* cmdP = new CommandProcessor();
    GameEngine::State currentState = GameEngine::State::Start;
    LogObserver* view3 = new LogObserver(cmdP);
    while (currentState != GameEngine::State::End)
    {
        currentState = gameEngine->startupPhase(gameEngine->getState(), cmdP);
    }

    delete gameEngine;
    delete cmdP;
    delete view3;*/

    cout << "\nOrderList::addOrder() being added to log file:" << endl;

    //OrdersList* orders = new OrdersList();
    //LogObserver* view4 = new LogObserver(orders);
    //cout << "*Created order list*\n";

    ////adding orders to the list
    //orders->addOrder(new Negotiate());
    //orders->addOrder(new Bomb());
    //orders->addOrder(new Advance());
    //orders->addOrder(new Deploy());
    //orders->addOrder(new Airlift());
    //orders->addOrder(new Blockade());

    //delete orders;
    //delete view4;

    cout << "\nGameEngine::transition() being added to log file:" << endl;

    GameEngine* transition = new GameEngine();
    LogObserver* view5 = new LogObserver(transition);

    CommandProcessor* cmdP = new CommandProcessor();
    GameEngine::State currentState = GameEngine::State::Start;

    while (currentState != GameEngine::State::End)
    {
        currentState = transition->startupPhase(transition->getState(), cmdP);
    }

    delete transition;
    delete view5;
    delete cmdP;

}