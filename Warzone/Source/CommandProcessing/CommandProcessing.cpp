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

CommandProcessor::CommandProcessor()
{
	nbCommands = 0;
}


string CommandProcessor::readCommand()
{

	std::string userInput;
	std::string delimiter = " ";
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
	return cmd;

}

void CommandProcessor::saveEffect(Command* cmd, std::string effectName)
{
	cmd->commandEffect = effectName;
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
	std::cout << "Close file command processor adapter" << std::endl;
}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& adapter)
{
	return *this;
}

string FileCommandProcessorAdapter::readCommand() 
{
	string userInput = fileLineReader->readLineFromFile();
	std::string delimiter = " ";
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

std::ostream& operator<<(ostream& output, const FileCommandProcessorAdapter& adapter)
{
	std::cout << "This is a file command processor adapter." <<  std::endl;
	return output;
}



