#include "CommandProcessing.h"
#include "CommandProcessingDriver.h"
#include <iostream>
#include <algorithm>
#include <string>
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
	string userInput;
	int condition = 0;
	while (condition == 0)
	{
		cout << "Please enter the command you would like to send next, in lowercase: ";
		cin >> userInput;
		cout << endl;

		if (userInput == "loadmap")
		{
			cout << "We will load the map" << endl;
			condition = 1;
		}
		else if (userInput == "validatemap")
		{
			cout << "We will validate the map" << endl;
			condition = 1;
		}
		else if (userInput == "addplayer")
		{
			cout << "We will add a player" << endl;
			condition = 1;
		}
		else if (userInput == "gamestart")
		{
			cout << "We will begin the game" << endl;
			condition = 1;
		}
		else if (userInput == "replay")
		{
			cout << "We will begin a new game" << endl;
			condition = 1;
		}
		else if (userInput == "quit")
		{
			cout << "We will end the game" << endl;
			condition = 1;
		}
		else
		{
			cout << "That was not a valid input, please try again" << endl;
		}

		//cout << userInput;


	}
	return userInput;


}

void CommandProcessor::getCommand()
{
	string cmdName = readCommand();
	saveCommand(cmdName);
}

Command* CommandProcessor::saveCommand(string cmdName)
{
	Command* cmd = new Command(cmdName);
	listCommands.push_back(cmd);
	nbCommands++;
	cout << "The command's name is : " << cmd->commandName;
	return cmd;

}