#ifndef PLAYERS_STRATEGY_H
#define PLAYERS_STRATEGY_H
#include <vector>
#include <string>
#include <iostream>
#include "../utils/LoggingObserver.h"
using namespace std;

class Player;
class Territory;
class Map;

class PlayerStrategy : public Subject, public ILoggable
{
public:
    virtual ~PlayerStrategy() = default; // deconstructor

    // Player Game Methods
    // These methods are purely virtual
    virtual bool issueOrder(Player *player, Map *map) = 0;
    virtual vector<Territory *> toAttack(Player *player, Map *map) const = 0;
    virtual vector<Territory *> toDefend(Player *player) const = 0;

    // Virtual method to get strategy name
    virtual string getStrategyName() const = 0;
};

// Human Player Strategy
class HumanPlayerStrategy : public PlayerStrategy
{
public:
    HumanPlayerStrategy();
    ~HumanPlayerStrategy() override;

    bool issueOrder(Player *player, Map *map) override;
    std::vector<Territory *> toAttack(Player *player, Map *map) const override;
    std::vector<Territory *> toDefend(Player *player) const override;
    std::string getStrategyName() const override;
};

// Aggressive Player Strategy
class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    AggressivePlayerStrategy();
    ~AggressivePlayerStrategy() override;

    bool issueOrder(Player *player, Map *map) override;
    std::vector<Territory *> toAttack(Player *player, Map *map) const override;
    std::vector<Territory *> toDefend(Player *player) const override;
    std::string getStrategyName() const override;

private:
    Territory *getStrongestTerritory(Player *player) const;
};

// Benevolent Player Strategy
class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
    BenevolentPlayerStrategy();
    ~BenevolentPlayerStrategy() override;

    bool issueOrder(Player *player, Map *map) override;
    std::vector<Territory *> toAttack(Player *player, Map *map) const override;
    std::vector<Territory *> toDefend(Player *player) const override;
    std::string getStrategyName() const override;

private:
    Territory *getWeakestTerritory(Player *player) const;
};

// Neutral Player Strategy
class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    NeutralPlayerStrategy();
    ~NeutralPlayerStrategy() override;

    bool issueOrder(Player *player, Map *map) override;
    std::vector<Territory *> toAttack(Player *player, Map *map) const override;
    std::vector<Territory *> toDefend(Player *player) const override;
    std::string getStrategyName() const override;
};

// Cheater Player Strategy
class CheaterPlayerStrategy : public PlayerStrategy
{
public:
    CheaterPlayerStrategy();
    ~CheaterPlayerStrategy() override;

    bool issueOrder(Player *player, Map *map) override;
    std::vector<Territory *> toAttack(Player *player, Map *map) const override;
    std::vector<Territory *> toDefend(Player *player) const override;
    std::string getStrategyName() const override;

private:
    bool hasConqueredThisTurn = false;
    void infinitArmyCheat(Territory *territory);
    void instantConquerCheat(const vector<Territory *> &toConquer, Player *player, Map *map);
    void duplicateReinforcementsCheat(Player *player);
};

#endif