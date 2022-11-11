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
	string userInput;
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
		}



	}
	return userInput;


}

void CommandProcessor::getCommand()
{
	string cmdName = readCommand();
	saveCommand(cmdName);
}

Command* CommandProcessor::saveCommand(string cmdName) //remember to track where the command object goes
{
	Command* cmd = new Command(cmdName);
	listCommands.push_back(cmd);

	std::cout << "The command's name is : " << cmd->commandName;
	return cmd;

}