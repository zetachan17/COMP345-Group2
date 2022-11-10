
#include <iostream>

#pragma once

class CommandProcessor;

class GameEngine
{
public:
    //all game states
    enum class State
    {
        Start = 0,
        MapLoaded,
        MapValidated,
        PlayersAdded,
        AssignReinforcement,
        IssueOrders,
        ExecuteOrders,
        Win,
        End
    };

    GameEngine(); //default constructor
    GameEngine(const GameEngine& game); //copy constructor
    GameEngine& operator = (const GameEngine&); //assignment operator
    ~GameEngine(); //destructor
    
    //stream insertion operator
    friend std::ostream& operator << (std::ostream& out, const GameEngine& g);
    friend std::istream& operator >> (std::istream& in, GameEngine& g);

    //Start the game
    State StartGame(State state, CommandProcessor* cmdP);

    //methods
    State getState();
    std::string stateToString(State state);

    friend class GameProcessor;

private:
    //ENUM, DOES NOT NEED TO BE A POINTER
    State state;
};

