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
    PlayerStrategy(Player *pl);

    // destructor
    virtual ~PlayerStrategy();

    virtual void issueOrder() = 0;

    virtual vector<Territory *> toDefend() const = 0;

    virtual vector<Territory *> toAttack() const = 0;

    virtual string getStrategyType() const = 0;

    void setPlayer(Player* pl);

protected:
    Player *player;
};

// Human player strategy: requires user input to make decisions
class HumanPlayerStrategy : public PlayerStrategy
{
public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(Player *pl);

    ~HumanPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() const override;

    vector<Territory *> toAttack() const override;

    string getStrategyType() const override;

private:
    void issueDeployOrder(const int deployedThisTurn, const int reinforcementPool);
};

// Computer player that focuses on attack (deploys or advances armies on its strongest country,
// then always advances to enemy territories until it cannot do so anymore)
class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    AggressivePlayerStrategy();
    AggressivePlayerStrategy(Player *pl);

    ~AggressivePlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() const override;

    vector<Territory *> toAttack() const override;

    string getStrategyType() const override;
};

// Computer player that focuses on protecting its weak countries (deploys or advances armies
// on its weakest countries, never advances to enemy territories)
class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(Player *pl);

    ~BenevolentPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() const override;

    vector<Territory *> toAttack() const override;

    string getStrategyType() const override;
};

// Computer player that never issues any order. If attacked, it becomes an Aggressive player
class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    NeutralPlayerStrategy();
    NeutralPlayerStrategy(Player *pl);

    ~NeutralPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() const override;

    vector<Territory *> toAttack() const override;

    string getStrategyType() const override;
};

// Computer player that automatically conquers all territories that are adjacent to its own
// territories (only once per turn)
class CheaterPlayerStrategy : public PlayerStrategy
{
public:
    CheaterPlayerStrategy();
    CheaterPlayerStrategy(Player *pl);

    ~CheaterPlayerStrategy();

    void issueOrder() override;

    vector<Territory *> toDefend() const override;

    vector<Territory *> toAttack() const override;

    string getStrategyType() const override;
};