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
    Player *p = new Player();
    // p->name = name;
    activePlayers.push_back(p);
}

// TODO: THIS IS SO UGLY, NEEDS TO BE FIXED
MapLoader *mLoader = new MapLoader;

GameEngine::State GameEngine::startupPhase(GameEngine::State state)
{
    std::string userInput;
    CommandProcessor *cmdP = new CommandProcessor;

    switch (state)
    {
    case GameEngine::State::Start:
        std::cout << "Welcome to Warzone!" << std::endl;
        cmdP->getCommand();
        // validateCommand()

        // Just making sure that this is indeed the loadmap command
        if (((cmdP->listCommands[cmdP->nbCommands])->commandName).substr(0, 7) == "loadmap")
        {
            if (mLoader->readFile(((cmdP->listCommands[cmdP->nbCommands])->commandName).substr(8)))
            {
                // loadMap();
                state = GameEngine::State::MapLoaded;
                // saveEffect()
                // break;
            }
            else
            {
                break;
            }
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }

    case GameEngine::State::MapLoaded:
        std::cout << "Map Loaded! " << std::endl;
        std::cout << "Do you want to load another map? Press Y to load another map, Enter \"validatemap\" to validate current map" << std::endl;
        std::cin >> userInput;
        if (userInput == "Y" || userInput == "y")
        {
            state = GameEngine::State::Start;
            break;
        }
        else if (userInput == "validatemap")
        {
            // P2.2, validate the map
            if (mLoader->getMap()->validate())
            {
                state = GameEngine::State::MapValidated;
                break;
            }

            std::cout << "Invalid map! Please select another map!" << std::endl;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }

    case GameEngine::State::MapValidated:
        std::cout << "Add Player? Y/N" << std::endl;
        std::cin >> userInput;
        if (userInput == "Y" || userInput == "y")
        {
            // P2.3 add players
            // TODO: find a better place to add players
            Player *newPlayer = new Player("P1");
            activePlayers.push_back(newPlayer);

            state = GameEngine::State::PlayersAdded;
            break;
        }
        else if (userInput == "N" || userInput == "n")
        {
            state = GameEngine::State::PlayersAdded;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::PlayersAdded:
        std::cout << "Player Added! Want to add another player? Press N to assign countries. Y/N" << std::endl;
        std::cin >> userInput;

        if (userInput == "Y" || userInput == "y")
        {
            // addPlayer(name);
            Player *newPlayer = new Player("P2");
            activePlayers.push_back(newPlayer);
            break;
        }
        else if (userInput == "N" || userInput == "n")
        {
            state = GameEngine::State::AssignReinforcement;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::AssignReinforcement:
        // TODO: IS THIS THE CORRECT PLACE?
        distributeTerritories(mLoader);
        giveInitialArmies();
        randomizePlayerOrder();
        // drawInitialCards();
        std::cout << "Please enter \"issueorder\" to order issue" << std::endl;
        std::cin >> userInput;

        if (userInput == "issueorder")
        {
            // issueOrder();
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
            // issueOrder();
            break;
        }
        else if (userInput == "endissueorders")
        {
            // executeOrder();
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
            // executeOrder();
            break;
        }
        else if (userInput == "endexecorders")
        {
            state = GameEngine::State::AssignReinforcement;
            break;
        }
        // temporary testing win condition
        // if (/*some win condition*/)
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
        std::cout << "Want to play again? Y/N" << std::endl;
        std::cin >> userInput;

        if (userInput == "Y" || userInput == "y")
        {
            state = GameEngine::State::Start;
            break;
        }
        else if (userInput == "N" || userInput == "n")
        {
            // TODO: DELETE MORE POINTERS
            delete mLoader->getMap();
            delete mLoader;
            mLoader = nullptr;
            state = GameEngine::State::End;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::End:

        break;
    }

    return state;
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
        std::cout << "Player " << player->getPlayerName() << " has " << count << " armies." << std::endl;
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
        player->addNumArmies(50);
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

Deck *GameEngine::getDeck()
{
    return deck;
}

// static variable definition
Deck *GameEngine::deck;
