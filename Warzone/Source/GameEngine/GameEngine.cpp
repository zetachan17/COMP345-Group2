#include "GameEngine/GameEngine.h"
#include "CommandProcessing/CommandProcessing.h"
#include "Map/Map.h"
#include "Player/Player.h"
#include "Cards/Cards.h"
#include "Orders/Orders.h"

#include <iostream>
#include <random>
#include <regex>
#include <string>

GameEngine::GameEngine()
{
    this->state = GameEngine::State::Start;
    // TODO:: PASS DECK FROM CARD.CPP TO HERE
    this->deck = new Deck();
    deck->createDeck();
}

GameEngine::GameEngine(const GameEngine &game)
{
    this->state = game.state;
}

GameEngine &GameEngine::operator=(const GameEngine &)
{
    return *this;
}

GameEngine::~GameEngine()
{
    std::cout << "Game engine destroyed!" << std::endl;
}

std::ostream &operator<<(std::ostream &out, const GameEngine &g)
{
    return out;
}

std::istream &operator>>(std::istream &in, GameEngine &g)
{
    return in;
}

void GameEngine::addPlayer(string name)
{
    Player *p = new Player(name);
    // p->name = name;
    activePlayers.push_back(p);
}

void GameEngine::addPlayer(Player *player)
{
    activePlayers.push_back(player);
}

// TODO: THIS IS SO UGLY, NEEDS TO BE FIXED
MapLoader *mLoader = new MapLoader;

GameEngine::State GameEngine::startupPhase(State state, CommandProcessor *commandProcessor)
{
    std::string userInput;

    switch (state)
    {
    case GameEngine::State::Start:
        activePlayers.clear();
        std::cout << "Welcome to Warzone!" << std::endl;
        commandProcessor->getCommand(commandProcessor); // Getting the command from the user
        if ((commandProcessor->listCommands[commandProcessor->nbCommands]->commandName) == "NULL")
        {
            std::cout << "The file you requested to open could not be opened. You will be redirected to choosing to use the command line or a prepared file";
            (commandProcessor->nbCommands)++;
            commandProcessor = nullptr;
            return GameEngine::State::End;
        }
        bool commandValidateValue;
        commandValidateValue = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this); // validating the command
        if (commandValidateValue)
        {
            if (((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(0, 7) == "loadmap") // Just making sure that this is indeed the loadmap command
            {
                if (mLoader->readFile(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(8)))
                {
                    this->state = GameEngine::State::MapLoaded;
                    (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], stateToString(this->state)); // Saving the effect inside the Command object as a string
                    std::cout << "\n\nThe effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
                    commandProcessor->nbCommands++; // Keeping track of the number of Commands
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
            (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state")); // Saving the effect inside the Command object as a string
            commandProcessor->nbCommands++;
            break;
        }

    case GameEngine::State::MapLoaded:
        commandProcessor->getCommand(commandProcessor);
        bool commandValidateValue2;
        commandValidateValue2 = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this); // validating the command
        if (commandValidateValue2)
        {
            if ((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName == "validatemap")
            {
                // P2.2, validate the map
                mLoader->getMap()->validate();
                this->state = GameEngine::State::MapValidated;
                (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState())); // Saving the effect inside the Command object as a string
                std::cout << "The effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
                commandProcessor->nbCommands++; // Keeping track of the number of Commands
                break;
            }
            else // In this case the command entered must have been "loadmap <filename>
            {
                if (mLoader->readFile(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(8)))
                {
                    this->state = GameEngine::State::MapLoaded;
                    (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], stateToString(this->state)); // Saving the effect inside the Command object as a string
                    std::cout << "\n\nThe effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
                    commandProcessor->nbCommands++; // Keeping track of the number of Commands
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
            (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state")); // Saving the effect inside the Command object as a string
            commandProcessor->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }

    case GameEngine::State::MapValidated:

        commandProcessor->getCommand(commandProcessor);
        commandValidateValue = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this);
        if (commandValidateValue)
        {
            // P2.3 add players
            // TODO: find a better place to add players
            Player *newPlayer = new Player(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(10));
            activePlayers.push_back(newPlayer);

            this->state = GameEngine::State::PlayersAdded;
            (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState())); // Saving the effect inside the Command object as a string
            std::cout << "The effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
            commandProcessor->nbCommands++; // Keeping track of the number of Commands
            break;
        }
        else
        {
            (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state")); // Saving the effect inside the Command object as a string
            std::cout << "The effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
            commandProcessor->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::PlayersAdded:
        commandProcessor->getCommand(commandProcessor);
        commandValidateValue = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this);
        if (commandValidateValue)
        {
            if ((commandProcessor->listCommands[commandProcessor->nbCommands]->commandName).substr(0, 9) == "addplayer")
            {

                Player *newPlayer = new Player(((commandProcessor->listCommands[commandProcessor->nbCommands])->commandName).substr(10));
                activePlayers.push_back(newPlayer);
                (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState())); // Saving the effect inside the Command object as a string
                std::cout << "The effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
                commandProcessor->nbCommands++;
                break;
            }
            else // In this case, the command must be "gamestart"
            {
                // TODO: IS THIS THE CORRECT PLACE?
                distributeTerritories(mLoader);
                giveInitialArmies();
                randomizePlayerOrder();
                drawInitialCards();
                this->state = GameEngine::State::AssignReinforcement;
                // this->state = GameEngine::State::Win;
                (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState())); // Saving the effect inside the Command object as a string
                std::cout << "The effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
                commandProcessor->nbCommands++;
                for (Command *cmd : commandProcessor->listCommands)
                {
                    std::cout << "The name of the command is :" + cmd->commandName + " its corresponding effect is :" + cmd->commandEffect << endl;
                }
                break;
            }
        }
        else
        {
            (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], ("This command was invalid in the " + stateToString(this->getState()) + " state")); // Saving the effect inside the Command object as a string
            std::cout << "The effect of this command is: " << commandProcessor->listCommands[commandProcessor->nbCommands]->commandEffect << endl;
            commandProcessor->nbCommands++;
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }

    case GameEngine::State::AssignReinforcement:
        cout << "----------------------------------------------------------\n"
             << "----------------------------------------------------------\n"
             << "\t** Assign Reinforcements Phase **\n\n";
        reinforcementPhase(mLoader);

        this->state = GameEngine::State::IssueOrders;
        break;
    case GameEngine::State::IssueOrders:
        cout << "----------------------------------------------------------\n"
             << "----------------------------------------------------------\n"
             << "\t** Issue Orders Phase **\n\n";
        issueOrdersPhase();
        this->state = GameEngine::State::ExecuteOrders;
        break;

    case GameEngine::State::ExecuteOrders:
        cout << "----------------------------------------------------------\n"
             << "----------------------------------------------------------\n"
             << "\t** Execute Orders Phase **\n\n";
        executeOrdersPhase();
        this->state = GameEngine::State::AssignReinforcement;
        checkForVictory(mLoader);
        checkForDefeats();
        break;
    case GameEngine::State::Win:

        commandProcessor->getCommand(commandProcessor);
        commandValidateValue = commandProcessor->validate(commandProcessor->listCommands[commandProcessor->nbCommands], this);
        if (commandValidateValue)
        {
            if (commandProcessor->listCommands[commandProcessor->nbCommands]->commandName == "replay")
            {
                // deleting static deck & active player list from completed game
                delete deck;
                for (Player *player : activePlayers)
                    delete player;
                activePlayers.clear();

                this->state = GameEngine::State::Start;
                (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState())); // Saving the effect inside the Command object as a string
                commandProcessor->nbCommands++;
                // delete mLoader->getMap();
                // delete mLoader;
                // mLoader = nullptr;
                break;
            }
            else // In this case the command must have been "quit"
            {
                this->state = GameEngine::State::End;
                (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState())); // Saving the effect inside the Command object as a string
                commandProcessor->nbCommands++;
                delete mLoader->getMap();
                delete mLoader;
                mLoader = nullptr;
                break;
            }
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            (commandProcessor->listCommands[commandProcessor->nbCommands])->saveEffect(commandProcessor->listCommands[commandProcessor->nbCommands], this->stateToString(getState())); // Saving the effect inside the Command object as a string
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

void GameEngine::distributeTerritories(MapLoader *mLoader)
{
    vector<Territory *> territories = mLoader->getMap()->getTerritories();

    auto randomizer = std::default_random_engine{};
    std::shuffle(std::begin(territories), std::end(territories), randomizer);

    while (!territories.empty())
    {
        for (Player *player : activePlayers)
        {
            if (territories.empty())
                break;

            player->addTerritory(territories.back());
            territories.pop_back();
        }
    }

    // some printing messages
    for (Player *player : activePlayers)
    {
        int count = 0;
        std::cout << player->getPlayerName() << std::endl;

        for (Territory *terr : player->toDefend())
        {
            std::cout << terr->getTerritoryName() << std::endl;
            count++;
        }
        std::cout << "Player " << player->getPlayerName() << " has " << count << " territories." << std::endl;
    }
}

void GameEngine::randomizePlayerOrder()
{
    auto randomizer = std::default_random_engine{};
    std::shuffle(std::begin(activePlayers), std::end(activePlayers), randomizer);
}

void GameEngine::giveInitialArmies()
{
    for (Player *player : activePlayers)
    {
        player->addReinforcements(50);
    }
}

void GameEngine::drawInitialCards()
{
    for (Player *player : activePlayers)
    {
        // draw two initial cards
        player->getHand()->addToHand(deck->draw());
        player->getHand()->addToHand(deck->draw());
    }
}

CommandProcessor *GameEngine::initializeCommandProcessor()
{
    CommandProcessor *commandProcessor;
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
        std::cout << "The file name is:" << fileName << std::endl;
        FileLineReader *fileLineReader = new FileLineReader();
        commandProcessor = new FileCommandProcessorAdapter(fileLineReader, fileName);
    }
    else
    {
        commandProcessor = nullptr;
        this->state = GameEngine::State::End;
        std::cout << "Invalid input. Please enter -console or -file \"filename\" exclusively" << endl;
    }

    return commandProcessor;
}

Deck *GameEngine::getDeck()
{
    return deck;
}

vector<Player *> GameEngine::getPlayers()
{
    return activePlayers;
}

// static variable definitions
Deck *GameEngine::deck;

vector<Player *> GameEngine::activePlayers;

void GameEngine::mainGameLoop(MapLoader *mLoader)
{
    reinforcementPhase(mLoader);
    issueOrdersPhase();
    executeOrdersPhase();
}

void GameEngine::reinforcementPhase(MapLoader *mLoader)
{
    for (Player *player : activePlayers)
    {
        int reinforcements = player->calculateReinforcements(mLoader->getMap());

        player->addReinforcements(reinforcements);
    }
}

void GameEngine::issueOrdersPhase()
{
    int totalPlayersFinishedIssuingOrders = 0;
    bool issueOrdersPhaseComplete = false;
    std::cout << "Starting the order issuing round robin" << endl;
    while (!issueOrdersPhaseComplete)
    {
        std::cout << "Next order issuing round" << endl;
        for (Player *player : activePlayers)
        {
            // std::cout << "Player: " << player->getPlayerName() << " is up" << endl;
            // std::cout << "Player: " << player->getPlayerName() << " : " << player->isFinishedIssuingOrders() << endl;

            // only process the player if they're still issuing orders
            if (!player->isFinishedIssuingOrders())
            {
                cout << "----------------------------------------------------------\n"
                     << endl;
                std::cout << "Player: " << player->getPlayerName() << " is issuing their next order" << endl;

                player->issueOrder();

                // check if the player is now finished
                if (player->isFinishedIssuingOrders())
                {
                    std::cout << "Player: " << player->getPlayerName() << " is finished issuing orders" << endl;

                    totalPlayersFinishedIssuingOrders++;
                    if (totalPlayersFinishedIssuingOrders == activePlayers.size())
                    {
                        std::cout << "All players are finished issuing orders" << endl;

                        issueOrdersPhaseComplete = true;
                    }
                }
            }
        }
    }

    for (Player *player : activePlayers)
    {
        player->setIsFinishedIssuingOrders(false);
        player->setArmiesDeployedThisTurn(0);
    }
}

void GameEngine::executeOrdersPhase()
{
    // all the deploy orders must be executed before other orders
    executeDeployOrders();
    executeRemainingOrders();
}

void GameEngine::executeDeployOrders()
{
    int totalPlayersFinishedExecutingDeployOrders = 0;
    vector<bool> playersFinishedExecutingDeployOrders(activePlayers.size(), false);
    bool executingDeployOrdersComplete = false;

    // start the round robin
    std::cout << "Starting the deploy order executing round robin" << endl;

    while (!executingDeployOrdersComplete)
    {
        std::cout << "Next deploy order executing round" << endl;

        for (int i = 0; i < activePlayers.size(); i++)
        {

            // only process the player if they still have deploy orders to execute
            if (!playersFinishedExecutingDeployOrders[i])
            {
                cout << "----------------------------------------------------------\n"
                     << endl;
                std::cout << "Player: " << activePlayers[i]->getPlayerName() << " is executing their next deploy order" << endl;

                // get the next deploy for this player
                Order *nextDeploy = activePlayers[i]->nextOrder(true);

                // if the order is null, the player has no more deploy orders
                if (nextDeploy == nullptr)
                {
                    std::cout << "Player: " << activePlayers[i]->getPlayerName() << " is finished executing deploy orders" << endl;

                    playersFinishedExecutingDeployOrders[i] = true;
                    totalPlayersFinishedExecutingDeployOrders++;
                    if (totalPlayersFinishedExecutingDeployOrders == activePlayers.size())
                    {
                        std::cout << "All players are finished executing deploy orders" << endl;

                        executingDeployOrdersComplete = true;
                    }
                }
                else
                {
                    nextDeploy->execute();
                    delete nextDeploy;
                }
            }
        }
    }
}

void GameEngine::executeRemainingOrders()
{
    int totalPlayersFinishedExecutingOrders = 0;
    vector<bool> playersFinishedExecutingOrders(activePlayers.size(), false);
    bool executingOrdersComplete = false;

    std::cout << "Starting the order executing round robin" << endl;
    while (!executingOrdersComplete)
    {
        std::cout << "Next order executing round" << endl;

        for (int i = 0; i < activePlayers.size(); i++)
        {

            // only process the player if they still have orders to execute
            if (!playersFinishedExecutingOrders[i])
            {
                cout << "----------------------------------------------------------\n"
                     << endl;
                std::cout << "Player: " << activePlayers[i]->getPlayerName() << " is executing their next order" << endl;

                // get the next order for this player
                Order *nextOrder = activePlayers[i]->nextOrder();

                // if the order is null, the player has no more orders
                if (nextOrder == nullptr)
                {
                    std::cout << "Player: " << activePlayers[i]->getPlayerName() << " is finished executing orders" << endl;

                    playersFinishedExecutingOrders[i] = true;
                    totalPlayersFinishedExecutingOrders++;
                    if (totalPlayersFinishedExecutingOrders == activePlayers.size())
                    {
                        std::cout << "All players are finished executing orders" << endl;

                        executingOrdersComplete = true;
                    }
                }
                else
                {
                    nextOrder->execute();
                    delete nextOrder;
                }
            }
        }
    }
}

void GameEngine::checkForDefeats()
{
    std::cout << "Checking for loss condition" << endl;

    bool defeat = false;
    for (int i = 0; i < activePlayers.size();)
    {
        if (activePlayers[i]->getTerritories().size() == 0)
        {
            std::cout << "Player: " << activePlayers[i]->getPlayerName() << " has lost." << endl;
            activePlayers.erase(activePlayers.begin() + i);
            defeat = true;
            continue;
        }
        i++;
    }

    if (activePlayers.size() == 1)
    {
        std::cout << "Player: " << activePlayers[0]->getPlayerName() << " has won!" << endl;
        this->state = GameEngine::State::Win;
    }
    else if (!defeat)
    {
        std::cout << "No player has been defeated this turn." << endl;
    }
}

void GameEngine::checkForVictory(MapLoader *mLoader)
{
    std::cout << "Checking for victory condition" << endl;

    bool victory = false;
    for (int i = 0; i < activePlayers.size(); i++)
    {
        if (activePlayers[i]->getTerritories().size() == mLoader->getMap()->getTerritories().size())
        {
            std::cout << "Player: " << activePlayers[i]->getPlayerName() << " has won!" << endl;
            victory = true;
            this->state = GameEngine::State::Win;
        }
    }

    if (!victory)
    {
        std::cout << "No victories detected this turn." << endl;
    }
}

// GameEngine's stringToLog() method
string GameEngine::stringToLog()
{

    string stringLog = "New state is " + stateToString(getState()) + " using transition().";
    cout << stringLog << endl;
    return stringLog;
}