#pragma once
#include <list>
using std::list;

class Observer {

public:
	~Observer();
	virtual void Update() = 0;

protected:
	Observer();
};

class Subject {

public:
	Subject();
	~Subject();

	virtual void Attach(Observer* o);
	virtual void Detach(Observer* o);
	virtual void Notify();

private:
	list<Observer*>* _observers;
};

class ILoggable {

public:
	virtual void stringToLog() = 0;
};

class LogObserver {
public:

};