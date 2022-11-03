#include "CommandProcessing/CommandProcessing.h"
#include "CommandProcessingDriver.h"
#include <iostream>
#include <string>
#include <vector>

Command:: Command()
{
	commandName = "None";
	commandEffect = "None";
}

CommandProcessor::CommandProcessor()
{
	nbCommands = 0;
}

string CommandProcessor::readCommand()
{
	cout << "Please enter the command you would like to send next: ";


}

Command* CommandProcessor::getCommand()
{
	readCommand();
}