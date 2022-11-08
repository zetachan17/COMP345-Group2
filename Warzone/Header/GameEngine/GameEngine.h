#pragma once

#include <iostream>

#include "Map/Map.h"
#include "Player/Player.h"

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

    void addPlayer(std::string name);
    
    //Start the game
    State StartGame(State state);

private:
    //ENUM, DOES NOT NEED TO BE A POINTER
    State state;
    vector<Player*> activePlayers;
    
    //Part 2.4.a) fairly distribute all the territories to the players 
    void distributeTerritories(MapLoader* mLoader);

    //Part 2.4.b) determine randomly the order of play of the players in the game
    void randomizPlayerOrder();
};

