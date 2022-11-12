#pragma once
#include "GameEngine/GameEngine.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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
	friend GameEngine::State GameEngine::startupPhase(GameEngine::State state, CommandProcessor* cmdP);

};



class CommandProcessor
{
private:
	//This is the collection of Command objects
	vector<Command*> listCommands;
	int nbCommands;

protected:
	virtual string readCommand();

public:
	//Default constructor
	CommandProcessor();

	//methods
	void getCommand(CommandProcessor* commandProcessor);
	Command* saveCommand(string cmdName);
	void saveEffect(Command* cmd, string effectName);
	bool validate(Command* cmd, GameEngine* gameEngine);
	
	//friends
	friend GameEngine::State GameEngine::startupPhase(GameEngine::State state, CommandProcessor* cmdP);
	friend GameEngine::State GameEngine::getState();

};

class FileLineReader {
private:
	string line;
	ifstream inputstream;
	string filename;
public:
	//Default Constructor
	FileLineReader();

	//method
	string readLineFromFile();

	//friend
	friend class FileCommandProcessorAdapter;
};

class FileCommandProcessorAdapter: public CommandProcessor
{
public:
	FileCommandProcessorAdapter(FileLineReader* fileLineReader, string filename);
	~FileCommandProcessorAdapter();

	FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& adapter);
	friend std::ostream& operator<<(std::ostream& output, const FileCommandProcessorAdapter& adapter);

protected:
	
	 string readCommand() override;

private:
	FileLineReader* fileLineReader;


};


