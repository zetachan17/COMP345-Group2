#include "LoggingObserver/LoggingObserverDriver.h"
#include "LoggingObserver/LoggingObserver.h"
#include "Orders/Orders.h"
#include "CommandProcessing/CommandProcessing.h"
#include "GameEngine/GameEngine.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
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


    cout << "\nCommandProcessor::saveCommand() and GameEngine::transition() being added to log file:" << endl;

    CommandProcessor* commandProcessor = nullptr;
    GameEngine::State currentState = GameEngine::State::End;

    while (commandProcessor == nullptr || currentState == GameEngine::State::End) {
        GameEngine* gameEngine = new GameEngine();
        currentState = GameEngine::State::Start;
        commandProcessor = gameEngine->initializeCommandProcessor();
        LogObserver* view1 = new LogObserver(commandProcessor);
        LogObserver* view5 = new LogObserver(gameEngine);

        // if(commandProcessor != NULL){
        while (currentState != GameEngine::State::End)
        {
            currentState = gameEngine->startupPhase(gameEngine->getState(), commandProcessor);
        }

        if (commandProcessor != nullptr)
        {
            delete commandProcessor;
            commandProcessor = nullptr;
            delete gameEngine;
            gameEngine = nullptr;
            delete view1;
            delete view5;
            break;
        }
    }

    cout << "___________________________________________________________\n" << endl;

    
    cout << "\nOrder::execute() being added to log file:" << endl;

    // setup game engine (just to test access to the deck), territories, and players to test
    GameEngine* game = new GameEngine();

    Territory* territories[7] = { new Territory(0, "Villeray", 4), new Territory(1, "Little Italy", 4),
                                 new Territory(2, "Mile End", 3), new Territory(3, "Plateau", 3),
                                 new Territory(4, "Mount Royal", 2), new Territory(5, "Downtown", 1),
                                 new Territory(6, "The Village", 1) };

    territories[0]->addAdjacentTerritory(territories[1]);
    territories[1]->addAdjacentTerritory(territories[0]);
    territories[1]->addAdjacentTerritory(territories[2]);
    territories[2]->addAdjacentTerritory(territories[1]);
    territories[2]->addAdjacentTerritory(territories[3]);
    territories[3]->addAdjacentTerritory(territories[2]);
    territories[3]->addAdjacentTerritory(territories[4]);
    territories[4]->addAdjacentTerritory(territories[3]);
    territories[3]->addAdjacentTerritory(territories[6]);
    territories[6]->addAdjacentTerritory(territories[3]);
    territories[4]->addAdjacentTerritory(territories[5]);
    territories[5]->addAdjacentTerritory(territories[4]);
    territories[5]->addAdjacentTerritory(territories[6]);
    territories[6]->addAdjacentTerritory(territories[5]);
    territories[2]->addAdjacentTerritory(territories[4]);
    territories[4]->addAdjacentTerritory(territories[2]);

    cout << "-----------------------------------------------------------\n"
        << "*CREATED TEST TERRITORIES*\n\n";
    for (Territory* territory : territories)
        cout << *territory;
    cout << "-----------------------------------------------------------\n";

    Player* tina = new Player("Tina");
    Player* eugene = new Player("Eugene");

    // assign territories for testing
    tina->addTerritory(territories[4]);
    tina->addTerritory(territories[5]);
    tina->addTerritory(territories[6]);
    eugene->addTerritory(territories[0]);
    eugene->addTerritory(territories[3]);

    tina->addReinforcements(50);
    eugene->addReinforcements(50);

    // issue test deploy orders
    tina->issueOrder(new Deploy(tina, 10, territories[4]));

    // issue test advance orders
    eugene->issueOrder(new Advance(eugene, 5, territories[4], territories[0]));

    // test execution of deploy orders
    Order* next = tina->nextOrder();
    LogObserver* view2 = new LogObserver(next);

    cout << "\n\t\t**EXECUTE DEPLOY ORDERS**\n\n"
        << "-----------------------------------------------------------\n";
    cout << tina->getPlayerName() << "'s next order: " << *next << endl;
    next->execute();
    cout << endl;


    // test execution of advance orders
    cout << "\n\t\t**EXECUTE ADVANCE ORDERS**\n\n"
        << "-----------------------------------------------------------\n";
    cout << eugene->getPlayerName() << "'s next order: " << *(next = eugene->nextOrder()) << endl;
    LogObserver* view21 = new LogObserver(next);
    next->execute();
    cout << endl;

    delete view2;
    delete view21;

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

    // setup game engine (just to test access to the deck), territories, and players to test
    GameEngine* game2 = new GameEngine();

    Territory* territories2[7] = { new Territory(0, "Villeray", 4), new Territory(1, "Little Italy", 4),
                                 new Territory(2, "Mile End", 3), new Territory(3, "Plateau", 3),
                                 new Territory(4, "Mount Royal", 2), new Territory(5, "Downtown", 1),
                                 new Territory(6, "The Village", 1) };

    territories2[0]->addAdjacentTerritory(territories2[1]);
    territories2[1]->addAdjacentTerritory(territories2[0]);
    territories2[1]->addAdjacentTerritory(territories2[2]);
    territories2[2]->addAdjacentTerritory(territories2[1]);
    territories2[2]->addAdjacentTerritory(territories2[3]);
    territories2[3]->addAdjacentTerritory(territories2[2]);
    territories2[3]->addAdjacentTerritory(territories2[4]);
    territories2[4]->addAdjacentTerritory(territories2[3]);
    territories2[3]->addAdjacentTerritory(territories2[6]);
    territories2[6]->addAdjacentTerritory(territories2[3]);
    territories2[4]->addAdjacentTerritory(territories2[5]);
    territories2[5]->addAdjacentTerritory(territories2[4]);
    territories2[5]->addAdjacentTerritory(territories2[6]);
    territories2[6]->addAdjacentTerritory(territories2[5]);
    territories2[2]->addAdjacentTerritory(territories2[4]);
    territories2[4]->addAdjacentTerritory(territories2[2]);

    cout << "-----------------------------------------------------------\n"
        << "*CREATED TEST TERRITORIES*\n\n";
    for (Territory* territory : territories2)
        cout << *territory;
    cout << "-----------------------------------------------------------\n";

    Player* sarah = new Player("Sarah");
    Player* bob = new Player("Bob");

    // assign territories for testing
    sarah->addTerritory(territories2[4]);
    sarah->addTerritory(territories2[5]);
    sarah->addTerritory(territories2[6]);
    bob->addTerritory(territories2[0]);
    bob->addTerritory(territories2[3]);

    sarah->addReinforcements(50);
    bob->addReinforcements(50);

    // adding orders to the list
    orders->addOrder(new Deploy(bob, 40, territories2[0]));
    orders->addOrder(new Deploy(sarah, 30, territories2[2]));
    orders->addOrder(new Deploy(bob, 20, territories2[3]));
    orders->addOrder(new Advance(sarah, 20, territories2[2], territories2[3]));
    orders->addOrder(new Advance(bob, 35, territories2[0], territories2[1]));
    orders->addOrder(new Airlift(sarah, 2, territories2[1], territories2[3]));
    orders->addOrder(new Blockade(bob, territories2[1]));
    orders->addOrder(new Negotiate(sarah, bob));

    delete orders;
    delete view4;

    cout << "___________________________________________________________\n" << endl;
}