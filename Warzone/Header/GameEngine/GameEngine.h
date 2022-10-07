#include <iostream>
#pragma once

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

    //
    State StartGame(State state);

private:
    //ENUM, DOES NOT NEED TO BE A POINTER
    State state;
};

