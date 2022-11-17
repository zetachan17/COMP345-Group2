#pragma once
#pragma once
#include <list>
#include <string>
//#include "Orders/Orders.h"
using std::string;
using std::list;

class CommandProcessor;
class Command;
class Order;
class OrdersList;
class GameEngine;

class ILoggable {

public:
	ILoggable();
	~ILoggable();

	virtual string stringToLog() = 0;
};

class Observer {

public:
	~Observer();
	virtual void Update(ILoggable* ilog) = 0;

protected:
	Observer();
};

class Subject {

public:
	Subject();
	~Subject();

	virtual void Attach(Observer* o);
	virtual void Detach(Observer* o);
	virtual void Notify(ILoggable* ilog);

private:
	list<Observer*>* observers;
};

class LogObserver : public Observer {

public:
	LogObserver();
	virtual ~LogObserver();
	LogObserver(CommandProcessor* saveCommand);
	LogObserver(Order* execute);
	LogObserver(Command* saveEffect);
	LogObserver(OrdersList* orders);
	LogObserver(GameEngine* transition);

	void Update(ILoggable* ilog);

	CommandProcessor* subjectSC;
	Order* subjectE;
	Command* subjectSE;
	OrdersList* subjectOL;
	GameEngine* subjectT;
};