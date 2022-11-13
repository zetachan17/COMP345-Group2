#pragma once
#include "GameEngine/GameEngine.h"
#include "LoggingObserver/LoggingObserver.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Command : public Subject, public ILoggable
{
private:
	string commandName;
	string commandEffect;

public:
	//Default constructor
	Command();

	//Copy constructor
	Command(const Command& command);

	//Destructor
	~Command();

	//Another constructor useful for the saveCommand() method
	Command(string cmdName);

	//Operator overloads
	Command& operator=(const Command& adapter);
	friend std::ostream& operator<<(std::ostream& output, const Command& adapter);

	//methods
	void saveEffect(Command* cmd, string effectName);

	//friend class
	friend class CommandProcessor;
	friend GameEngine::State GameEngine::startupPhase(GameEngine::State state, CommandProcessor*commandProcessor);

};



class CommandProcessor : public Subject, public ILoggable
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

	//Copy constructor
	CommandProcessor(const CommandProcessor& commandProcessor);

	//Destructor
	virtual ~CommandProcessor();

	//methods
	void getCommand(CommandProcessor* commandProcessor);
	Command* saveCommand(string cmdName);
	//void saveEffect(Command* cmd, string effectName);
	bool validate(Command* cmd, GameEngine* gameEngine);

	//Operator Overloads
	CommandProcessor& operator=(const CommandProcessor& adapter);
	friend std::ostream& operator<<(std::ostream& output, const CommandProcessor& adapter);
	
	//friends
	friend GameEngine::State GameEngine::startupPhase(GameEngine::State state, CommandProcessor*commandProcessor);
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

	//Copy constructor
	FileLineReader(const FileLineReader& filelinereader);

	//Operator Overloads
	FileLineReader& operator=(const FileLineReader& filelinereader);
	friend std::ostream& operator<<(std::ostream& output, const FileLineReader& filelinereader);

	//Destructor
	~FileLineReader();

	//method
	string readLineFromFile();

	//friend
	friend class FileCommandProcessorAdapter;
};

class FileCommandProcessorAdapter: public CommandProcessor
{
public:
	//Constructor
	FileCommandProcessorAdapter(FileLineReader* fileLineReader, string filename);

	//Destructor
	virtual ~FileCommandProcessorAdapter();

	//Copy constructor
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

	//Operator overloads
	FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& adapter);
	friend std::ostream& operator<<(std::ostream& output, const FileCommandProcessorAdapter& adapter);

protected:
	
	 string readCommand() override;

private:
	FileLineReader* fileLineReader;
	vector<Command*> listCommands;


};


