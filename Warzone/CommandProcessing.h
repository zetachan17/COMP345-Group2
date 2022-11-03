#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;


struct Command
{
private:
	string commandName;
	string commandEffect;

public:
	//Default constructor
	Command();

	//friend class
	friend class CommandProcessor;

};



class CommandProcessor
{
private:
	//This is the collection of Command objects
	vector<Command*> listCommands;
	int nbCommands;
	string readCommand();

public:
	//Default constructor
	CommandProcessor();

	//methods
	Command* getCommand();
	void saveCommand(Command* command);
	void saveEffect(string effectName);






};
