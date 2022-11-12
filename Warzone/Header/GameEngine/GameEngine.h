#pragma once
#include <iostream>
#include "Map/Map.h"
#include "Player/Player.h"

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

    void addPlayer(std::string name);
    
    //Start the game
    State startupPhase(State state, CommandProcessor*commandProcessor);

    //methods
    State getState();
    std::string stateToString(State state);

    CommandProcessor* initializeCommandProcessor();
    friend class GameProcessor;

private:
    //ENUM, DOES NOT NEED TO BE A POINTER
    State state;
    Deck* deck;
    vector<Player*> activePlayers;
    
    //Part 2.4.a) fairly distribute all the territories to the players 
    void distributeTerritories(MapLoader* mLoader);

    //Part 2.4.b) determine randomly the order of play of the players in the game
    void randomizePlayerOrder();

    //Part 2.4.c) give 50 initial army units to the players
    void giveInitialArmies();

    // Part 2.4.d) let each player draw 2 initial cards from the deck using the deck��s draw() method
    void drawInitialCards();
};

