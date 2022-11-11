#pragma once
#include <list>
#include <string>
//#include "Orders/Orders.h"
using std::string;
using std::list;

class Order;
class OrdersList;

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
	//LogObserver(D* saveCommand);
	//LogObserver(D* execute);
	//LogObserver(D* saveEffect);
	LogObserver(OrdersList* orders);
	//LogObserver(D* transition);

	void Update(ILoggable* ilog);

	//D* subjectSC;
	//D* subjectE;
	//D* subjectSE;
	OrdersList* subjectOL;
	//D* subjectT;
};