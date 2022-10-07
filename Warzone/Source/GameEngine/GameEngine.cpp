#include <iostream>
#include "GameEngine/GameEngine.h"

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
}

std::ostream& operator<<(std::ostream& out, const GameEngine& g)
{
    return out;
}

std::istream& operator>>(std::istream& in, GameEngine& g)
{
    return in;
}


GameEngine::State GameEngine::StartGame(GameEngine::State state)
{
    std::string userInput;
    
    switch (state)
    {
    case GameEngine::State::Start:
        std::cout << "Welcome to Warzone!" << std::endl;
        std::cout << "Please enter \"loadmap\" to load map" << std::endl;
        std::cin >> userInput;
        if (userInput == "loadmap")
        {
            //loadMap();
            state = GameEngine::State::MapLoaded;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::MapLoaded:
        std::cout << "Map Loaded! "<< std::endl;
        std::cout << "Do you want to load another map? Y/N" << std::endl;
        std::cin >> userInput;
        if (userInput == "Y" || userInput == "y")
        {
            //loadMap();
            break;    
        }
        else if (userInput == "N" || userInput == "n")
        {
            //validateMap();
            state = GameEngine::State::MapValidated;
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
            //addPlayer();
            state = GameEngine::State::PlayersAdded;
            break;
        }
        else if (userInput == "N" || userInput == "n")
        {
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
            state = GameEngine::State::MapValidated;
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
        std::cout << "Please enter \"issueorder\" to order issue" << std::endl;
        std::cin >> userInput;

        if (userInput == "issueorder")
        {
            //issueOrder();
            state = GameEngine::State::IssueOrders;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case GameEngine::State::IssueOrders:
        std::cout << "Enter \"issueorder\" to order more issues or \"endissueorders\" to execute orders" << std::endl;
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
        std::cout << "Want to play again? Y/N" << std::endl;
        std::cin >> userInput;

        if (userInput == "Y" || userInput == "y")
        {
            state = GameEngine::State::Start;
            break;
        }
        else if (userInput == "N" || userInput == "n")
        {
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

