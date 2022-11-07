#include "CommandProcessing/CommandProcessingDriver.h"
#include "CommandProcessing/CommandProcessing.h"

void testCommandProcessor()
{
	CommandProcessor *comP = new CommandProcessor;
	comP->getCommand();
}