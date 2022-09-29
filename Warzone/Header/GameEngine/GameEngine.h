#pragma once

enum State
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

class GameEngine
{
public:
    static State StartGame(State state);
};

