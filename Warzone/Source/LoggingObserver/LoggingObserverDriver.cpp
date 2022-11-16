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

    cout << "___________________________________________________________\n" << endl;

    cout << "\nCommandProcessor::saveCommand() being added to log file:" << endl;

    //CommandProcessor* commandProcessor = nullptr;
    //GameEngine::State currentState = GameEngine::State::End;

    //while (commandProcessor == nullptr || currentState == GameEngine::State::End) {
    //    GameEngine* gameEngine = new GameEngine();
    //    currentState = GameEngine::State::Start;
    //    commandProcessor = gameEngine->initializeCommandProcessor();
    //    LogObserver* view1 = new LogObserver(commandProcessor);

    //    cout << "\nShow that CommandProcessor is a subclass of Subject and ILoggable:" << endl;
    //    if (dynamic_cast<Subject*>(commandProcessor) != nullptr && dynamic_cast<ILoggable*>(commandProcessor) != nullptr) {
    //        cout << "CommandProcessor is a subclass of Subject and ILoggable.\n" << endl;
    //    }
    //    
    //    // if(commandProcessor != NULL){
    //    while (currentState != GameEngine::State::End)
    //    {
    //        currentState = gameEngine->startupPhase(gameEngine->getState(), commandProcessor);
    //    }
    //    
    //    if (commandProcessor != nullptr)
    //    {
    //        delete commandProcessor;
    //        commandProcessor = nullptr;
    //        delete gameEngine;
    //        gameEngine = nullptr;
    //        delete view1;
    //        break;
    //    }
    //}

    cout << "___________________________________________________________\n" << endl;

    cout << "\nOrder::execute() being added to log file:" << endl;

    // setup game engine (just to test access to the deck)
    GameEngine* game = new GameEngine();

    // setup test territories
    Territory* territories[4] = { new Territory(0, "Plateau", 3),
                                 new Territory(1, "Mount Royal", 2),
                                 new Territory(2, "Downtown", 1),
                                 new Territory(3, "The Village", 1) };

    territories[0]->addAdjacentTerritory(territories[1]);
    territories[1]->addAdjacentTerritory(territories[0]);
    territories[0]->addAdjacentTerritory(territories[3]);
    territories[3]->addAdjacentTerritory(territories[0]);
    territories[1]->addAdjacentTerritory(territories[2]);
    territories[2]->addAdjacentTerritory(territories[1]);
    territories[2]->addAdjacentTerritory(territories[3]);
    territories[3]->addAdjacentTerritory(territories[2]);

    cout << "-----------------------------------------------------------\n"
        << "*CREATED TEST TERRITORIES*\n"
        << "*CREATED 2 TEST PLAYERS*\n"
        << "*ASSIGNED TERRITORIES*\n"
        << "*ISSUED ORDERS TO TEST*\n"
        << "-----------------------------------------------------------\n"
        << "\n\t\t**PLAYERS & ORDERS LISTS**\n\n"
        << "-----------------------------------------------------------\n";

    // setup test players
    Player* tina = new Player("Tina");
    Player* eugene = new Player("Eugene");

    // assign territories & assign reinfcorcements for testing
    tina->addTerritory(territories[1]);
    tina->addTerritory(territories[2]);
    eugene->addTerritory(territories[3]);
    eugene->addTerritory(territories[0]);
    tina->addReinforcements(50);
    eugene->addReinforcements(50);

    // issue test orders
    eugene->issueOrder(new Deploy(eugene, 40, territories[0]));
    tina->issueOrder(new Deploy(tina, 30, territories[2]));

    cout << endl
        << *tina
        << "-----------------------------------------------------------\n"
        << *eugene
        << "-----------------------------------------------------------\n";

    // test execution of deploy orders
    Order* next = eugene->nextOrder();
    LogObserver* view2 = new LogObserver(next);

    cout << "\n\t\t**ORDERS EXECUTION**\n\n"
        << "-----------------------------------------------------------\n";
    cout << eugene->getPlayerName() << "'s next order: " << *next << endl;
    next->execute();
    cout << endl;
    cout << tina->getPlayerName() << "'s next order: " << *(next = tina->nextOrder()) << endl;
    LogObserver* view21 = new LogObserver(next);
    next->execute();
    cout << endl;

    delete view2;
    delete view21;
    delete game;
    
    cout << "___________________________________________________________\n" << endl;

    cout << "\nCommand::saveEffect() being added to log file:" << endl;

    Command* saveEffectTest = new Command();
    Command* cmd = new Command();
    LogObserver* view3 = new LogObserver(cmd);
    cmd->saveEffect(saveEffectTest, "Start");
    cmd->saveEffect(saveEffectTest, "MapLoaded");
    cmd->saveEffect(saveEffectTest, "MapValidated");

    cout << "Show that Command is a subclass of Subject and ILoggable:" << endl;
    if (dynamic_cast<Subject*>(saveEffectTest) != nullptr && dynamic_cast<ILoggable*>(saveEffectTest) != nullptr) {
        cout << "Command is a subclass of Subject and ILoggable." << endl;
    }

    delete saveEffectTest;
    delete cmd;
    delete view3;

    cout << "\n___________________________________________________________\n" << endl;

    cout << "\n\nOrderList::addOrder() being added to log file:" << endl;

    OrdersList* orders = new OrdersList();
    LogObserver* view4 = new LogObserver(orders);

    // setup test territories
    Territory* territoriesAddOrder[4] = { new Territory(0, "Plateau", 3),
                                 new Territory(1, "Mount Royal", 2),
                                 new Territory(2, "Downtown", 1),
                                 new Territory(3, "The Village", 1) };

    territoriesAddOrder[0]->addAdjacentTerritory(territoriesAddOrder[1]);
    territoriesAddOrder[1]->addAdjacentTerritory(territoriesAddOrder[0]);
    territoriesAddOrder[0]->addAdjacentTerritory(territoriesAddOrder[3]);
    territoriesAddOrder[3]->addAdjacentTerritory(territoriesAddOrder[0]);
    territoriesAddOrder[1]->addAdjacentTerritory(territoriesAddOrder[2]);
    territoriesAddOrder[2]->addAdjacentTerritory(territoriesAddOrder[1]);
    territoriesAddOrder[2]->addAdjacentTerritory(territoriesAddOrder[3]);
    territoriesAddOrder[3]->addAdjacentTerritory(territoriesAddOrder[2]);

    cout << "-----------------------------------------------------------\n"
        << "*CREATED TEST TERRITORIES*\n"
        << "*CREATED 2 TEST PLAYERS*\n"
        << "*ASSIGNED TERRITORIES*\n"
        << "*ISSUED ORDERS TO TEST*\n"
        << "-----------------------------------------------------------\n"
        << "\n\t\t**PLAYERS & ORDERS LISTS**\n\n"
        << "-----------------------------------------------------------\n";

    // setup test players
    Player* sarah = new Player("Sarah");
    Player* bob = new Player("Bob");

    // adding orders to the list
    orders->addOrder(new Deploy(bob, 40, territoriesAddOrder[0]));
    orders->addOrder(new Deploy(sarah, 30, territoriesAddOrder[2]));
    orders->addOrder(new Deploy(bob, 20, territoriesAddOrder[3]));
    orders->addOrder(new Advance(sarah, 20, territoriesAddOrder[2], territoriesAddOrder[3]));
    orders->addOrder(new Advance(bob, 35, territoriesAddOrder[0], territoriesAddOrder[1]));
    orders->addOrder(new Airlift(sarah, 2, territoriesAddOrder[1], territoriesAddOrder[3]));
    orders->addOrder(new Blockade(bob, territoriesAddOrder[1]));
    orders->addOrder(new Negotiate(sarah, bob));

    cout << "Show that OrderList is a subclass of Subject and ILoggable:" << endl;
    if (dynamic_cast<Subject*>(orders) != nullptr && dynamic_cast<ILoggable*>(orders) != nullptr) {
       cout << "OrderList is a subclass of Subject and ILoggable." << endl;
    }

    delete orders;
    delete view4;

    cout << "___________________________________________________________\n" << endl;

    cout << "\nGameEngine::transition() being added to log file:" << endl;

    CommandProcessor* commandProcessorTransition = NULL;
    GameEngine::State currentStateTransition = GameEngine::State::End;
   
    while (commandProcessorTransition == NULL|| currentStateTransition == GameEngine::State::End) {
        GameEngine* gameEngineTransition = new GameEngine();
        LogObserver* view5 = new LogObserver(gameEngineTransition);
        currentStateTransition = GameEngine::State::Start;
        CommandProcessor* commandProcessorTransition = gameEngineTransition->initializeCommandProcessor();
        while (currentStateTransition != GameEngine::State::End)
        {
            currentStateTransition = gameEngineTransition->startupPhase(gameEngineTransition->getState(), commandProcessorTransition);
        }
        if (commandProcessorTransition != NULL)
        {
            delete commandProcessorTransition;
            delete gameEngineTransition;
            delete view5;
        }
       
    }

    cout << "Show that gameEngine is a subclass of Subject and ILoggable:" << endl;
    GameEngine* testGameEngine = new GameEngine();
    if (dynamic_cast<Subject*>(testGameEngine) != nullptr && dynamic_cast<ILoggable*>(testGameEngine) != nullptr) {
        cout << "gameEngine is a subclass of Subject and ILoggable." << endl;
    }
    delete testGameEngine;

    cout << "___________________________________________________________\n" << endl;
}