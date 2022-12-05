#pragma once
#include <iostream>
#include <vector>
#include "LoggingObserver/LoggingObserver.h"

class MapLoader;
class Player;
class Deck;
class Card;

class CommandProcessor;

class GameEngine : public Subject, public ILoggable
{
public:
    // all game states
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

    GameEngine();                              // default constructor
    GameEngine(const GameEngine &game);        // copy constructor
    GameEngine &operator=(const GameEngine &); // assignment operator
    virtual ~GameEngine();                     // destructor

    // stream insertion operator
    friend std::ostream &operator<<(std::ostream &out, const GameEngine &g);
    friend std::istream &operator>>(std::istream &in, GameEngine &g);
    friend class FileCommandProcessorAdapter;

    void addPlayer(std::string name);

    static void addPlayer(Player *player);

    static std::vector<Player *> getPlayers();

    // returns pointer to game deck
    static Deck *getDeck();

    // Start the game
    State startupPhase(State state, CommandProcessor *commandProcessor);

    // methods
    State getState();
    std::string stateToString(State state);

    CommandProcessor *initializeCommandProcessor();
    friend class GameProcessor;

    // contains logic for the game loop after initial setup
    void mainGameLoop(MapLoader *mLoader);

    // stringToLog
    string stringToLog();

private:
    // ENUM, DOES NOT NEED TO BE A POINTER
    State state;
    static Deck *deck;
    static std::vector<Player *> activePlayers;
    int turnLimit;
    int currentTurn;
    //std::ofstream tournamentFile;

    // Part 2.4.a) fairly distribute all the territories to the players
    void distributeTerritories(MapLoader *mLoader);

    // Part 2.4.b) determine randomly the order of play of the players in the game
    void randomizePlayerOrder();

    // Part 2.4.c) give 50 initial army units to the players
    void giveInitialArmies();

    // Part 2.4.d) let each player draw 2 initial cards from the deck using the deck's draw() method
    void drawInitialCards();

    // Part 3.1
    void reinforcementPhase(MapLoader *mLoader);

    // Part 3.2
    void issueOrdersPhase();

    // Part 3.3
    void executeOrdersPhase();

    // Part 3.3.a
    void executeDeployOrders();

    // Part 3.3.b
    void executeRemainingOrders();

    void checkForDefeats();

    bool checkForVictory(MapLoader* mLoader);

    bool checkForDraw();

    //convert userinput to corresponding strategy
    void createStrategyPlayer(string userinput);
};
