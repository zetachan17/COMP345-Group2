#include "PlayerStrategies/PlayerStrategies.h"

PlayerStrategy::PlayerStrategy() : mPlayer(nullptr), mStrategyType("") {}

PlayerStrategy::PlayerStrategy(Player *player, const string &type) : mPlayer(player),
                                                                     mStrategyType(type) {}

PlayerStrategy::~PlayerStrategy() {}

string PlayerStrategy::getStrategyType() const
{
    return mStrategyType;
}

HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}

HumanPlayerStrategy::HumanPlayerStrategy(Player *player) : PlayerStrategy(player, "Human") {}

HumanPlayerStrategy::~HumanPlayerStrategy() {}

AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy() {}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player)
    : PlayerStrategy(player, "Aggressive") {}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player)
    : PlayerStrategy(player, "Benevolent") {}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player)
    : PlayerStrategy(player, "Neutral") {}

NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

CheaterPlayerStrategy::CheaterPlayerStrategy() : PlayerStrategy() {}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player *player)
    : PlayerStrategy(player, "Cheater") {}

CheaterPlayerStrategy::~CheaterPlayerStrategy() {}