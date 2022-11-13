#include <iostream>
#include "GameEngine/GameEngine.h"

#include <random>
#include <regex>
#include "Map/Map.h"
#include "CommandProcessing/CommandProcessing.h"

#include <string>

#include "Player/Player.h"

GameEngine::GameEngine()
{
    this->state = GameEngine::State::Start;
    //TODO:: PASS DECK FROM CARD.CPP TO HERE
    this->deck = new Deck();
    deck->createDeck();
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


void GameEngine::addPlayer(string name)
{
    Player* p = new Player();
    //p->name = name;
    activePlayers.push_back(p);
}


MapLoader* mLoader = new MapLoader;

GameEngine::State GameEngine::startupPhase(State state, CommandProcessor* commandProcessor)
{
    std::string userInput;

    switch (state)
    {
    case GameEngine::State::Start:
        std::cout << "Welcome to Warzone!" << std::endl;
        
        commandProcessor->getCommand(commandProcessor); //Getting the command from the user
        if ((commandProcessor->listCommands[commandProcessor->nbCommands]->commandName) == "NULL")
        {
            std::cout << "The file you requested to open could not be opened. You will be redirected to choosing to use the command line or a prepared file";
            (commandProcessor->nbCommands)++;
            commandProcessor = NULL;
            return GameEngine::State::End;
        }
        bool commandValidateValue;
        commandValidateValue = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this); //validating the command
        if (commandValidateValue)
        {
            if (((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(0, 7) == "loadmap") //Just making sure that this is indeed the loadmap command
            {
                if (mLoader->readFile(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(8)))
                {
                   
                    this->state = GameEngine::State::MapLoaded;
                    (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], stateToString(this->state));//Saving the effect inside the Command object as a string
                    std::cout << "\n\nThe effect of this command is: "<<commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect<<endl;
                   commandProcessor->nbCommands++;//Keeping track of the number of Commands
                   break;
                    
                }
                else
                {
                    break;
                }
                
            }
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
           (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
           commandProcessor->nbCommands++;
            break;
        }
        
    case GameEngine::State::MapLoaded:

       commandProcessor->getCommand(commandProcessor);
        bool commandValidateValue2;
        commandValidateValue2 = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this); //validating the command
        if (commandValidateValue2)
        {
            if ((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName == "validatemap")
            {
                //P2.2, validate the map
                mLoader->getMap()->validate();
                this->state = GameEngine::State::MapValidated;
               (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
               std::cout << "The effect of this command is: " <<commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
               commandProcessor->nbCommands++;//Keeping track of the number of Commands
                break;
            }
            else//In this case the command entered must have been "loadmap <filename>
            {
                
                if (mLoader->readFile(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(8)))
                {

                    this->state = GameEngine::State::MapLoaded;
                    (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], stateToString(this->state));//Saving the effect inside the Command object as a string
                    std::cout << "\n\nThe effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
                    commandProcessor->nbCommands++;//Keeping track of the number of Commands
                    break;
                }
                else
                {
                    break;
                }
            }
            
        }
        else
        {
           (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
           commandProcessor->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
        
    case GameEngine::State::MapValidated:
        
       commandProcessor->getCommand(commandProcessor);
        commandValidateValue = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this);
        if (commandValidateValue)
        {
            //P2.3 add players
            //TODO: find a better place to add players
            Player* newPlayer = new Player(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(10));
            activePlayers.push_back(newPlayer);


            this->state = GameEngine::State::PlayersAdded;
           (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
           std::cout << "The effect of this command is: " <<commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
           commandProcessor->nbCommands++;//Keeping track of the number of Commands
            break;
        }
        else
        {
           (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
           std::cout << "The effect of this command is: " <<commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
           commandProcessor->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::PlayersAdded:
       commandProcessor->getCommand(commandProcessor);
        commandValidateValue =commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this);
        if (commandValidateValue)
        {   
            if ((commandProcessor->listCommands[commandProcessor->nbCommands]->commandName).substr(0, 9) == "addplayer")
            {
                
                Player* newPlayer = new Player(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(10));
                activePlayers.push_back(newPlayer);
               (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
               std::cout << "The effect of this command is: " <<commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
               commandProcessor->nbCommands++;
                break;
            }
            else //In this case, the command must be "gamestart"
            {
                //TODO: IS THIS THE CORRECT PLACE?
                distributeTerritories(mLoader);
                giveInitialArmies();
                randomizePlayerOrder();
                //drawInitialCards();

                this->state = GameEngine::State::AssignReinforcement;
               (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
               std::cout << "The effect of this command is: " <<commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
               commandProcessor->nbCommands++;
                for (Command* cmd :commandProcessor->listCommands)
                {
                    std::cout << "The name of the command is :" + cmd->commandName + " its corresponding effect is :" + cmd->commandEffect<<endl;
                }
                break;
            }
        }
        else
        {
           (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
           std::cout << "The effect of this command is: " <<commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
          commandProcessor->nbCommands++;
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
            this->state = GameEngine::State::IssueOrders;
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
            this->state = GameEngine::State::ExecuteOrders;
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
            this->state = GameEngine::State::AssignReinforcement;
            break;
        }
        // temporary testing win condition
        //if (/*some win condition*/)
        else if (userInput == "win")
        {
            this->state = GameEngine::State::Win;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::Win:
        
       commandProcessor->getCommand(commandProcessor);
        commandValidateValue =commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this);
        if (commandValidateValue)
        {
            if (commandProcessor->listCommands[commandProcessor->nbCommands]->commandName == "replay")
            {
                this->state = GameEngine::State::Start;    
               (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
               commandProcessor->nbCommands++;
                break;
            }
            else //In this case the command must have been "quit"
            {
                this->state = GameEngine::State::End;
               (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
               commandProcessor->nbCommands++;
                break;
            }
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
           (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
           commandProcessor->nbCommands++;
            break;
        }
    case GameEngine::State::End:
        
        break;
    }
    Notify(this);
    
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


void GameEngine::distributeTerritories(MapLoader* mLoader)
{
    vector<Territory*> territories = mLoader->getMap()->getTerritories();

    auto randomizer = std::default_random_engine {};
    std::shuffle(std::begin(territories), std::end(territories), randomizer);
    
    while (!territories.empty())
    {
        for (Player* player : activePlayers)
        {
            if (territories.empty())
                break;

            player->addTerritory(territories.back());
            territories.pop_back();
        }
    }

    // some printing messages
    for (Player* player : activePlayers)
    {
        int count = 0;
        std::cout << player->getPlayerName() << std::endl;
        
        for (Territory* terr :player->getTerritories())
        {
            std::cout << terr->getTerritoryName() << std::endl;
            count++;
        }
        std::cout << "Player " << player->getPlayerName() <<  " has " << count << " armies." << std::endl;
    }
}

void GameEngine::randomizePlayerOrder()
{
    auto randomizer = std::default_random_engine {};
    std::shuffle(std::begin(activePlayers), std::end(activePlayers), randomizer);
}

void GameEngine::giveInitialArmies()
{
    for (Player* player : activePlayers)
    {
        player->addNumArmies(50);
    }
}

void GameEngine::drawInitialCards()
{
    for (Player* player : activePlayers)
    {
        //draw two initial cards
        player->getHand()->addToHand(deck->draw());
        player->getHand()->addToHand(deck->draw());
    }
}

CommandProcessor* GameEngine::initializeCommandProcessor()
{
    CommandProcessor* commandProcessor;
    string userInput;
    std::regex fileRegex("-file");
    
    std::cout << "Please choose if you want the game accept commands form the console (-console) or from a file (-file filename)" << std::endl;
    std::getline(cin, userInput);
    
    if (userInput == "-console")
    {   
        commandProcessor = new CommandProcessor();
        
    }
    else if (std::regex_search(userInput, fileRegex))
    {
        string fileName = userInput.substr(6);
        std::cout << "The file name is:"<<fileName << std::endl;
        FileLineReader* fileLineReader = new FileLineReader();
        commandProcessor = new FileCommandProcessorAdapter(fileLineReader,fileName);
        
    }
    else
    {
        commandProcessor = NULL;
        this->state = GameEngine::State::End;
        std::cout << "Invalid input. Please enter -console or -file \"filename\" exclusively" << endl;
    }
    
    return commandProcessor;
    
}


string GameEngine::stringToLog() {

    string stringLog = "New state is " + stateToString(getState()); + " transition().";
    cout << stringLog << endl;
    return stringLog;
}