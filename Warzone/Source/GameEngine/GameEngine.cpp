#include "GameEngine/GameEngine.h"

#include <iostream>
#include <valarray>
#include <valarray>

State GameEngine::StartGame(State state)
{
    std::string userInput;
    
    switch (state)
    {
    case Start:
        std::cout << "Welcome to Warzone!" << std::endl;
        std::cin >> userInput;
        if (userInput == "loadmap")
        {
            //loadMap();
            state = MapLoaded;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case MapLoaded:
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
            state = MapValidated;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case MapValidated:
        std::cout << "Add Player? Y/N" << std::endl;
        std::cin >> userInput;
        if (userInput == "Y" || userInput == "y")
        {
            //addPlayer();
            state = PlayersAdded;
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
    case PlayersAdded:
        std::cout << "Player Added! Want to add another player? Press N to assign countries. Y/N" << std::endl;
        std::cin >> userInput;
        
        if (userInput == "Y" || userInput == "y")
        {
            state = MapValidated;
            break;
        }
        else if (userInput == "N" || userInput == "n")
        {
            state = AssignReinforcement;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case AssignReinforcement:
        std::cout << "Please enter \"issueorder\" to order issue" << std::endl;
        std::cin >> userInput;

        if (userInput == "issueorder")
        {
            //issueOrder();
            state = IssueOrders;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case IssueOrders:
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
            state = ExecuteOrders;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    case ExecuteOrders:
        std::cout << "Enter \"execorder\" to execute more orders or \"endexecorders\" to end execute orders" << std::endl;
        std::cin >> userInput;

        if (userInput == "execorder")
        {
            //executeOrder();
            break;
        }
        else if (userInput == "endexecorders")
        {
            state = AssignReinforcement;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }

        //if (/*some win condition*/)
        {
            state = Win;
            break;
        }
    case Win:
        std::cout << "Want to play again? Y/N" << std::endl;
        std::cin >> userInput;

        if (userInput == "Y" || userInput == "y")
        {
            state = Start;
            break;
        }
        else if (userInput == "N" || userInput == "n")
        {
            state = End;
            break;
        }
        else
        {
            std::cout << "Invalid input, please try again!" << std::endl;
            break;
        }
    }
    
    return state;
}
