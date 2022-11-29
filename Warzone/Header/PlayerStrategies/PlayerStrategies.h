#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;

class Player;
class Territory;

// Abstract player strategy class
class PlayerStrategy
{
public:
    // constructors
    PlayerStrategy();
    PlayerStrategy(Player *player, const string &type);

    // destructor
    virtual ~PlayerStrategy();

    virtual void issueOrder() = 0;

    virtual vector<Territory *> toDefend() = 0;

    virtual vector<Territory *> toAttack() = 0;

    string getStrategyType() const;

protected:
    Player *mPlayer;
    string mStrategyType;
};

// Human player strategy: requires user input to make decisions
class HumanPlayerStrategy : public PlayerStrategy
{
public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(Player *player);

    ~HumanPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack() override;
};

// Computer player that focuses on attack (deploys or advances armies on its strongest country,
// then always advances to enemy territories until it cannot do so anymore)
class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    AggressivePlayerStrategy();
    AggressivePlayerStrategy(Player *player);

    ~AggressivePlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack() override;
};

// Computer player that focuses on protecting its weak countries (deploys or advances armies
// on its weakest countries, never advances to enemy territories)
class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(Player *player);

    ~BenevolentPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack() override;
};

// Computer player that never issues any order. If attacked, it becomes an Aggressive player
class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    NeutralPlayerStrategy();
    NeutralPlayerStrategy(Player *player);

    ~NeutralPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> override;
};

// Computer player that automatically conquers all territories that are adjacent to its own
// territories (only once per turn)
class CheaterPlayerStrategy : public PlayerStrategy
{
public:
    CheaterPlayerStrategy();
    CheaterPlayerStrategy(Player *player);

    ~CheaterPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() override;

    vector<Territory *> toAttack() override;
};