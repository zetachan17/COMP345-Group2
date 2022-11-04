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

	//Another constructor useful for the saveCommand() method
	Command(string cmdName);

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
	void getCommand();
	Command* saveCommand(string cmdName);
	//void saveEffect(string effectName);

};
