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

    CommandProcessor* saveCommand = new CommandProcessor;
    LogObserver* view1 = new LogObserver(saveCommand);
    saveCommand->getCommand(saveCommand);

    cout << "Show that CommandProcessor is a subclass of Subject and ILoggable:" << endl;
    if (dynamic_cast<Subject*>(saveCommand) != nullptr && dynamic_cast<ILoggable*>(saveCommand) != nullptr) {
        cout << "CommandProcessor is a subclass of Subejct and ILoggable." << endl;
    }

    delete saveCommand;
    delete view1;

    cout << "\nOrder::execute() being added to log file:" << endl;

    //D* execute = new D();
    //LogObserver* view2 = new LogObserver(execute);

    //cout << "Show that Order is a subclass of Subject and ILoggable:" << endl;
    //if (dynamic_cast<Subject*>(execute) != nullptr && dynamic_cast<ILoggable*>(execute) != nullptr) {
    //    cout << "Order is a subclass of Subejct and ILoggable." << endl;
    //}

    //delete execute;
    //delete view2;

    cout << "\nCommand::saveEffect() being added to log file:" << endl;

    Command* saveEffectTest = new Command();
    Command* cmd = new Command();
    LogObserver* view3 = new LogObserver(saveEffectTest);
    cmd->saveEffect(saveEffectTest, "");

    cout << "Show that Command is a subclass of Subject and ILoggable:" << endl;
    if (dynamic_cast<Subject*>(saveEffectTest) != nullptr && dynamic_cast<ILoggable*>(saveEffectTest) != nullptr) {
        cout << "Command is a subclass of Subejct and ILoggable." << endl;
    }

    delete saveEffectTest;
    delete cmd;
    delete view3;

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

    //cout << "Show that OrderList is a subclass of Subject and ILoggable:" << endl;
    //if (dynamic_cast<Subject*>(orders) != nullptr && dynamic_cast<ILoggable*>(orders) != nullptr) {
    //    cout << "OrderList is a subclass of Subejct and ILoggable." << endl;
    //}

    //delete orders;
    //delete view4;

    cout << "\nGameEngine::transition() being added to log file:" << endl;

    CommandProcessor* commandProcessor = NULL;
    GameEngine::State currentState = GameEngine::State::End;
   
    while (commandProcessor == NULL|| currentState == GameEngine::State::End) {
        GameEngine* gameEngine = new GameEngine();
        LogObserver* view5 = new LogObserver(gameEngine);
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
            delete view5;
        }
       
    }

    cout << "Show that gameEngine is a subclass of Subject and ILoggable:" << endl;
    GameEngine* testGameEngine = new GameEngine();
    if (dynamic_cast<Subject*>(testGameEngine) != nullptr && dynamic_cast<ILoggable*>(testGameEngine) != nullptr) {
        cout << "gameEngine is a subclass of Subejct and ILoggable." << endl;
    }
    delete testGameEngine;

}