#include "LoggingObserver/LoggingObserver.h"
#include "Orders/Orders.h"
#include "CommandProcessing/CommandProcessing.h"
#include <iostream>
#include <fstream>
#include <string>
using std::string;

// ILoggable
ILoggable::ILoggable() {}

ILoggable::~ILoggable() {}

// Observer
Observer::Observer() {}

Observer::~Observer() {}

// Subject
Subject::Subject()
{
	observers = new list<Observer *>;
}

Subject::~Subject()
{
	delete observers;
}

void Subject::Attach(Observer *o)
{
	observers->push_back(o);
}

void Subject::Detach(Observer *o)
{
	observers->remove(o);
}

// Notify Method
void Subject::Notify(ILoggable *ilog)
{
	list<Observer *>::iterator i = observers->begin();
	for (; i != observers->end(); ++i)
	{
		(*i)->Update(ilog);
	}
}

// LogObserver
LogObserver::LogObserver() {}

LogObserver::~LogObserver() {}

LogObserver::LogObserver(CommandProcessor *saveCommand)
{

	subjectSC = saveCommand;
	subjectSC->Attach(this);
}

LogObserver::LogObserver(Order *execute)
{

	subjectE = execute;
	subjectE->Attach(this);
}

LogObserver::LogObserver(Command *saveEffect)
{

	subjectSE = saveEffect;
	subjectSE->Attach(this);
}

LogObserver::LogObserver(OrdersList *orders)
{

	subjectOL = orders;
	subjectOL->Attach(this);
}

LogObserver::LogObserver(GameEngine *transition)
{

	subjectT = transition;
	subjectT->Attach(this);
}

// Update Method
void LogObserver::Update(ILoggable *ilog)
{

	string output = ilog->stringToLog();
	std::ofstream gameLog;
	gameLog.open("gamelog.txt", std::ios_base::app);
	gameLog << output << "\n";
	gameLog.close();
}