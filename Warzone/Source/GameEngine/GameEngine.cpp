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

//TODO: THIS IS SO UGLY, NEEDS TO BE FIXED
MapLoader* mLoader = new MapLoader;

GameEngine::State GameEngine::StartGame(State state, CommandProcessor* cmdP)
{
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
                if (mLoader->readFile(((cmdP->listCommands[cmdP->nbCommands])->commandName).substr(8)))
                {
                    std::cout << "Making sure that this is gone intpo" << endl;
                    //loadMap();
                    this->state = GameEngine::State::MapLoaded;
                    cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], stateToString(this->state));//Saving the effect inside the Command object as a string
                    //std::cout << cmdP->listCommands[cmdP->nbCommands]->commandEffect;
                    cmdP->nbCommands++;//Keeping track of the number of Commands
                    //break;
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
            cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state"));//Saving the effect inside the Command object as a string
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
            //P2.3 add players
            //TODO: find a better place to add players
            Player* newPlayer = new Player("P1");
            activePlayers.push_back(newPlayer);


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
                Player* newPlayer = new Player("P2");
                activePlayers.push_back(newPlayer);
                cmdP->saveEffect(cmdP->listCommands[cmdP->nbCommands], this->stateToString(getState()));//Saving the effect inside the Command object as a string
                cmdP->nbCommands++;
                break;
            }
            else //In this case, the command must be "gamestart"
            {
                //gamestart() function?
                //TODO: IS THIS THE CORRECT PLACE?
                distributeTerritories(mLoader);
                giveInitialArmies();
                randomizePlayerOrder();
                //drawInitialCards();

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
    return state;
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

    return state;
}
