#include "CommandProcessing/CommandProcessing.h"
#include "CommandProcessing/CommandProcessingDriver.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <regex>
#include <vector>


Command::Command()
{
	commandName = "None";
	commandEffect = "None";
}

Command::Command(string cmdName)
{
	commandName = cmdName;
}

Command::Command(const Command& command)
{
	this->commandName = command.commandName;
	this->commandEffect = command.commandEffect;
}

Command::~Command()
{
	std::cout << "Command object destroyed!";
}


Command& Command::operator=(const Command& command)
{
	this->commandName = command.commandName;
	this->commandEffect = command.commandEffect;
	return *this;
}

ostream& operator<<(std::ostream& output, const Command& adapter)
{
	output << "The name of this command is: " << adapter.commandName << " and the name of the effect it stores is: " << adapter.commandEffect << endl;
	return output;
}

CommandProcessor::CommandProcessor()
{
	nbCommands = 0;
}

CommandProcessor::CommandProcessor(const CommandProcessor& commandProcessor)
{
	nbCommands = commandProcessor.nbCommands;
	for (Command* command : commandProcessor.listCommands)
	{
		listCommands.push_back(command);
	}
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& commandProcessor)
{
	this->nbCommands = commandProcessor.nbCommands;
	for (Command* command : commandProcessor.listCommands)
	{
		listCommands.push_back(command);
	}
	return *this;
}

CommandProcessor::~CommandProcessor() 
{
	if (!this->listCommands.empty())
	{
		for (Command* command : this->listCommands)
		{
			delete command;
			command = nullptr;
		}
	}
}

ostream& operator<<(std::ostream& output, const CommandProcessor& adapter)
{
	output << "CommandProcessor has " << adapter.nbCommands << "number of Commands" << endl;
	output << "And the Commands contained in this CommandProcessor are: " << endl;
	for (Command* command : adapter.listCommands)
	{
		output << *command;
	}
	return output;
}

void Command::saveEffect(Command* cmd, string effectName)
{
	cmd->commandEffect = effectName;
	effectToLog = effectName;
	Notify(this);
}

string CommandProcessor::readCommand()
{

	std::string userInput;
	const std::string delimiter = " ";
	std::string secondInput;
	int condition = 0;

	while (condition == 0)
	{
		std::cout << "Please enter the command you would like to send next, in lowercase: ";
		std::getline(std::cin, userInput);
		std::cout << endl;

		if (userInput.substr(0, 7) == "loadmap")

		{
			if (userInput.find(delimiter) == -1) {// This is to ensure that there is a space between "loadmap" and the file name
				std::cout << "Make sure to add a space between \"loadmap\" and the file name" << endl;

			}
			else {
				std::cout << "We will load the map" << endl;
				secondInput = userInput.substr(userInput.find(delimiter) + 1, userInput.size() - 1); //We store the name of the map file in secondInput
				std::cout << "the name of the file is: " << secondInput << endl;
				condition = 1;
			}
		}
		else if (userInput.substr(0, 9) == "addplayer")
		{
			if (userInput.find(delimiter) == -1) {// This is to ensure that there is a space between "addplayer" and the player name
				std::cout << "Make sure to add a space between \"loadmap\" and the file name" << endl;

			}
			else {
				std::cout << "We will add the Player : ";
				secondInput = userInput.substr(userInput.find(delimiter) + 1, userInput.size() - 1); //We store the name of the player in secondInput
				std::cout << secondInput << endl;
				condition = 1;
			}
		}
		else if (userInput == "gamestart")
		{
			std::cout << "We will begin the game" << endl;
			condition = 1;
		}
		else if (userInput == "validatemap")
		{
			std::cout << "We will validate the map" << endl;
			condition = 1;
		}
		else if (userInput == "replay")
		{
			std::cout << "We will begin a new game" << endl;
			condition = 1;
		}
		else if (userInput == "quit")
		{
			std::cout << "We will end the game" << endl;
			condition = 1;
		}
		else
		{
			std::cout << "That was not a valid input, please try again" << endl;
			condition = 1;
		}
	}
	return userInput;
}

void CommandProcessor::getCommand(CommandProcessor* commandProcessor)
{
	std::string commandName;
	commandName = commandProcessor->readCommand();
	saveCommand(commandName);
}

Command* CommandProcessor::saveCommand(string commandName) //remember to track where the command object goes
{
	Command* cmd = new Command(commandName);
	listCommands.push_back(cmd);
	std::cout << "The command's name is : " << cmd->commandName << std::endl;
	
	Notify(this);
	
	return cmd;
}

bool CommandProcessor::validate(Command* cmd, GameEngine* gameEngine)
{
	if ((cmd->commandName).substr(0, 7) == "loadmap")
	{
		if (gameEngine->getState() == GameEngine::State::MapLoaded || gameEngine->getState() == GameEngine::State::Start)
		{
			return true;
		}
		else return false;
	}
	else if ((cmd->commandName).substr(0, 9) == "addplayer")
	{
		if (gameEngine->getState() == GameEngine::State::MapValidated || gameEngine->getState() == GameEngine::State::PlayersAdded)
		{
			return true;
		}
		else return false;
	}
	else if (cmd->commandName == "validatemap")
	{
		if (gameEngine->getState() == GameEngine::State::MapLoaded)
		{
			return true;
		}
		else return false;
	}
	else if (cmd->commandName == "gamestart")
	{
		if (gameEngine->getState() == GameEngine::State::PlayersAdded)
		{
			return true;
		}
		else return false;
	}
	else if (cmd->commandName == "replay")
	{
		if (gameEngine->getState() == GameEngine::State::Win)
		{
			return true;
		}
		else return false;
	}
	else if (cmd->commandName == "quit")
	{
		if (gameEngine->getState() == GameEngine::State::Win)
		{
			return true;
		}
		else return false;
	}
	else {
		return false;
	}
}

FileLineReader::FileLineReader() 
{

}

FileLineReader::FileLineReader(const FileLineReader& filelinereader)
{
	this->filename = filelinereader.filename;
}

FileLineReader& FileLineReader::operator=(const FileLineReader& filelinereader)
{
	this->filename = filelinereader.filename;
	return *this;
}

ostream& operator<<(std::ostream& output, const FileLineReader& filelinereader)
{
	output << "The name of this filelinereader is: " << filelinereader.filename;
	return output;
}

FileLineReader::~FileLineReader()
{
	std::cout << "FileLineReader object destroyed!";
}

string FileLineReader::readLineFromFile()
{
	if (inputstream.eof())
	{
		std::cout << "End of the command file, now quit the game!" << std::endl;
		return "quit";
	}

	string command;
	getline(inputstream, command);
	if (command == "")
	{
		std::cout << "End of the command file, now quit the game!" << std::endl;
		return "quit";
	}

	return command;
}


FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileLineReader* fileLineReader, string filename) : CommandProcessor()
{
	this->fileLineReader = fileLineReader;
	this->fileLineReader->filename = filename;
	this->fileLineReader->inputstream.open("CommandFile/" + filename);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
	this->fileLineReader->inputstream.close();
	delete fileLineReader;
	for (Command* y : this->listCommands)
	{
		delete y;
	}
	
	std::cout << "FileCommandProcessorAdapter object destroyed!" << std::endl;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fileCommandProcessorAdapter)
{
	this->fileLineReader = fileCommandProcessorAdapter.fileLineReader;
	this->fileLineReader->filename = fileCommandProcessorAdapter.fileLineReader->filename;
	this->fileLineReader->inputstream.open("CommandFile/" +(fileCommandProcessorAdapter.fileLineReader->filename).substr(0));
}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& adapter)
{
	return *this;
}

string FileCommandProcessorAdapter::readCommand() 
{

	if((this->fileLineReader->inputstream).is_open())
	{
	string userInput = fileLineReader->readLineFromFile();
	const std::string delimiter = " ";
	std::string secondInput;
	int condition = 0;
	while (condition == 0)
	{
		if (userInput.substr(0, 7) == "loadmap")

		{
			if (userInput.find(delimiter) == -1) {// This is to ensure that there is a space between "loadmap" and the file name
				std::cout << "Make sure to add a space between \"loadmap\" and the file name" << endl;

			}
			else {
				std::cout << "We will load the map" << endl;
				secondInput = userInput.substr(userInput.find(delimiter) + 1, userInput.size() - 1); //We store the name of the map file in secondInput
				std::cout << "the name of the file is: " << secondInput << endl;
				condition = 1;
			}
		}
		else if (userInput.substr(0, 9) == "addplayer")
		{
			if (userInput.find(delimiter) == -1) {// This is to ensure that there is a space between "addplayer" and the player name
				std::cout << "Make sure to add a space between \"loadmap\" and the file name" << endl;

			}
			else {
				std::cout << "We will add the Player : ";
				secondInput = userInput.substr(userInput.find(delimiter) + 1, userInput.size() - 1); //We store the name of the player in secondInput
				std::cout << secondInput << endl;
				condition = 1;
			}
		}
		else if (userInput == "gamestart")
		{
			std::cout << "We will begin the game" << endl;
			condition = 1;
		}
		else if (userInput == "validatemap")
		{
			std::cout << "We will validate the map" << endl;
			condition = 1;
		}
		else if (userInput == "replay")
		{
			std::cout << "We will begin a new game" << endl;
			condition = 1;
		}
		else if (userInput == "quit")
		{
			std::cout << "We will end the game" << endl;
			condition = 1;
		}
		else
		{
			std::cout << "That was not a valid input, please try again" << endl;
			condition = 1;
		}
	}
	return userInput;
	}
	else {
		return "NULL";
	}
}

std::ostream& operator<<(ostream& output, const FileCommandProcessorAdapter& adapter)
{
	std::cout << "This is a file command processor adapter." <<  std::endl;
	return output;
}

std::string command::statetostringc(gameengine::state state)
{
	if (state == gameengine::state::start)
	{
		return "start";
	}
	else if (state == gameengine::state::maploaded)
	{
		return "maploaded";
	}
	else if (state == gameengine::state::mapvalidated)
	{
		return "mapvalidated";
	}
	else if (state == gameengine::state::playersadded)
	{
		return "playersadded";
	}
	else if (state == gameengine::state::playersadded)
	{
		return "playersadded";
	}
	else if (state == gameengine::state::win)
	{
		return "win";
	}
	else
	{
		return "end";
	}
}

GameEngine::State GameEngine::getStateC()
	{
	return this->state;
}

//CommandProcessor's stringToLog() method
string CommandProcessor::stringToLog() {

	string stringLog = listCommands.back()->commandName + " has been saved using saveCommand().";
	cout << stringLog << endl;
	return stringLog;
}

//Command's stringToLog() method
string Command::stringToLog() {

	string stringLog = "saveEffect() method saved the transition to the " + effectToLog + " state inside the commandEffect attribute";
	//string stringLog = commandEffect + " saveEffect() method saved the transition.";
	//string stringLog = stateToStringC(getStateC()) + " saveEffect() method saved the transition.";
	cout << stringLog << endl;
	return stringLog;
}