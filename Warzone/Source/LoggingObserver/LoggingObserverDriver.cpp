#include "LoggingObserver/LoggingObserverDriver.h"
#include "LoggingObserver/LoggingObserver.h"
#include "Orders/Orders.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;

void testLoggingObserver() {

    cout << "Assignment 2, Part 5: Game Logger" << endl;

    cout << "Clearing the contents of the gamelog for this new iteration of the game." << endl;
    std::ofstream gameLog;
    //gameLog.open("gamelog.txt");
    gameLog.open("gamelog.txt", std::fstream::out | std::fstream::trunc);
    gameLog << "Output of commands:\n";
    gameLog.close();

    cout << "\nCommandProcessor::saveCommand() being added to log file:" << endl;

    //D* saveCommand = new D();
    //LogObserver* view = new LogObserver(saveCommand);


    cout << "\nOrder::execute() being added to log file:" << endl;

    //D* execute = new D();
    //LogObserver* view = new LogObserver(execute);


    cout << "\nCommand::saveEffect() being added to log file:" << endl;

    //D* saveEffect = new D();
    //LogObserver* view = new LogObserver(saveEffect);


    cout << "\nOrderList::addOrder() being added to log file:" << endl;

    OrdersList* orders = new OrdersList();
    LogObserver* view = new LogObserver(orders);
    cout << "*Created order list*\n";

    //adding orders to the list
    orders->addOrder(new Negotiate());
    orders->addOrder(new Bomb());
    orders->addOrder(new Advance());
    orders->addOrder(new Deploy());
    orders->addOrder(new Airlift());
    orders->addOrder(new Blockade());

    delete orders;
    delete view;

    cout << "\nGameEngine::transition() being added to log file:" << endl;

    //D* transition = new D();
    //LogObserver* view = new LogObserver(transition);

}