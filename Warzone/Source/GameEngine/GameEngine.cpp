#include <iostream>
#include "GameEngine/GameEngine.h"
#include <regex>
#include "Map/Map.h"
#include "CommandProcessing/CommandProcessing.h"

#include <string>

GameEngine::GameEngine()
{
    this->state = GameEngine::State::Start;
}

GameEngine::GameEngine(const GameEngine& game)
{
    this->state = game.state;
}

GameEngine& GameEngine::operator=(const GameEngine&)
{
    return *this;
}

GameEngine::~GameEngine()
{
    std::cout << "Game engine destroyed!" << std::endl;
}

std::ostream& operator<<(std::ostream& out, const GameEngine& g)
{
    return out;
}

std::istream& operator>>(std::istream& in, GameEngine& g)
{
    return in;
}


GameEngine::State GameEngine::StartGame(State state, CommandProcessor* cmdP)
{
  
    MapLoader *mLoader = new MapLoader;
    
        std::string userInput;
        
        switch (state)
        {

        case GameEngine::State::Start:
            std::cout << "Welcome to Warzone!" << std::endl;
            cmdP->getCommand(); //Getting the command from the user
            bool cmdValidateValue;
            cmdValidateValue = cmdP->validate(cmdP->listCommands[cmdP->nbCommands], this); //validating the command
            std::cout << cmdValidateValue;
            if (cmdValidateValue)
            {
                if (((cmdP->listCommands[cmdP->nbCommands])->commandName).substr(0, 7) == "loadmap") //Just making sure that this is indeed the loadmap command
                {
                    std::cout << "Making sure that this is gone intpo"<<endl;
                    //loadMap();
                    this->state = GameEngine::State::MapLoaded;
                    cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], stateToString(this->state));//Saving the effect inside the Command object as a string
                    //std::cout << cmdP->listCommands[cmdP->nbCommands]->commandEffect;
                    cmdP->nbCommands++;//Keeping track of the number of Commands
                    break;
                }
                break;
            }
            else {
                std::cout << "Invalid input, please try again!" << std::endl;
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], ("This command was invalid in the "+stateToString(this->getState())+" state"));//Saving the effect inside the Command object as a string
                cmdP->nbCommands++;
                break;
            }
    case GameEngine::State::MapLoaded:

        std::cout << this->stateToString(getState());

        cmdP->getCommand();
        std::cout << this->stateToString(this->getState());
        cmdValidateValue = cmdP->validate(cmdP->listCommands[cmdP->nbCommands], this); //validating the command
        std::cout << cmdValidateValue;
        //cmdP->nbCommands++;//Keeping track of the number of Commands
        if (cmdValidateValue)
        {
            if ((cmdP->listCommands[cmdP->nbCommands])->commandName == "validatemap")
            {
                //P2.2, validate the map
                mLoader->getMap().validate();
                this->state = GameEngine::State::MapValidated;
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
                cmdP->nbCommands++;//Keeping track of the number of Commands
                break;
            }
            else//In this case the command entered must have been "loadmap <filename>
            {
                //loadMap();
                this->state = GameEngine::State::MapLoaded;
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], stateToString(this->state));//Saving the effect inside the Command object as a string
                //std::cout << cmdP->listCommands[cmdP->nbCommands]->commandEffect;
                cmdP->nbCommands++;//Keeping track of the number of Commands
                break;
            }
            
        }
        else
        {
            cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
            cmdP->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::MapValidated:
        
        cmdP->getCommand();
        cmdValidateValue = cmdP->validate(cmdP->listCommands[cmdP->nbCommands], this);
        if (cmdValidateValue)
        {
            //addPlayer();
            this->state = GameEngine::State::PlayersAdded;
            cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
            cmdP->nbCommands++;//Keeping track of the number of Commands
            break;
        }
        else
        {
            cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
            cmdP->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::PlayersAdded:
        cmdP->getCommand();
        cmdValidateValue = cmdP->validate(cmdP->listCommands[cmdP->nbCommands], this);
        if (cmdValidateValue)
        {   
            if ((cmdP->listCommands[cmdP->nbCommands]->commandName).substr(0, 9) == "addplayer")
            {
                // addPlayer();
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
                cmdP->nbCommands++;
                break;
            }
            else //In this case, the command must be "gamestart"
            {
                //gamestart() function?
                this->state = GameEngine::State::AssignReinforcement;
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
                cmdP->nbCommands++;
                for (Command* cmd : cmdP->listCommands)
                {
                    std::cout << "The name of the command is :" + cmd->commandName + " its corresponding effect is :" + cmd->commandEffect<<endl;
                }
                break;
            }
        }
        else
        {
            cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
            cmdP->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::AssignReinforcement:
        std::cout << "Please enter \"issueorder\" to order issue" << std::endl;
        std::cin >> userInput;

        if (userInput == "issueorder")
        {
            //issueOrder();
            std::cout << "Reinforcements assigned!" << std::endl;
            state = GameEngine::State::IssueOrders;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::IssueOrders:
        std::cout << "Enter \"issueorder\" to issues more orders or \"endissueorders\" to execute orders" << std::endl;
        std::cin >> userInput;

        if (userInput == "issueorder")
        {
            //issueOrder();
            break;
        }
        else if (userInput == "endissueorders")
        {
            //executeOrder();
            state = GameEngine::State::ExecuteOrders;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::ExecuteOrders:
        std::cout << "Enter \"execorder\" to execute more orders or \"endexecorders\" to end execute orders" << std::endl;
        std::cin >> userInput;

        if (userInput == "execorder")
        {
            //executeOrder();
            break;
        }
        else if (userInput == "endexecorders")
        {
            state = GameEngine::State::AssignReinforcement;
            break;
        }
        // temporary testing win condition
        //if (/*some win condition*/)
        else if (userInput == "win")
        {
            state = GameEngine::State::Win;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::Win:
        
        cmdP->getCommand();
        cmdValidateValue = cmdP->validate(cmdP->listCommands[cmdP->nbCommands], this);
        if (cmdValidateValue)
        {
            if (cmdP->listCommands[cmdP->nbCommands]->commandName == "replay")
            {
                this->state = GameEngine::State::Start;    
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
                cmdP->nbCommands++;
                break;
            }
            else //In this case the command must have been "quit"
            {
                this->state = GameEngine::State::End;
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
                cmdP->nbCommands++;
                break;
            }
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
            cmdP->nbCommands++;
            break;
        }
    case GameEngine::State::End:
        break;
    }

    return state;
}

GameEngine::State GameEngine::getState()
{
    return this->state;
}

std::string GameEngine::stateToString(State state)
{
    if (state == GameEngine::State::Start)
    {
        return "Start";
    }
    else if (state == GameEngine::State::MapLoaded)
    {
        return "MapLoaded";
    }
    else if (state == GameEngine::State::MapValidated)
    {
        return "MapValidated";
    }
    else if (state == GameEngine::State::PlayersAdded)
    {
        return "PlayersAdded";
    }
    else if (state == GameEngine::State::PlayersAdded)
    {
        return "PlayersAdded";
    }
    else if (state == GameEngine::State::Win)
    {
        return "Win";
    }
    else
    {
        return "End";
    }
    
}
