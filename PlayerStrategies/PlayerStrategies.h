#ifndef PLAYERS_STRATEGY_H
#define PLAYERS_STRATEGY_H

#include <vector>
#include <string>
#include "../utils/LoggingObserver.h"

class Player;
class Territory;
class Map;

// ======================= BASE STRATEGY =======================

class PlayerStrategy : public ILoggable, public Subject {
public:
    virtual ~PlayerStrategy() = default;

    // Pure virtual behaviour
    virtual bool issueOrder(Player *player, Map *map) = 0;
    virtual std::vector<Territory *> toAttack(Player *player, Map *map) const = 0;
    virtual std::vector<Territory *> toDefend(Player *player) const = 0;

    virtual std::string getStrategyName() const = 0;

    // ILoggable
    std::string stringToLog() const override;

protected:
    std::string lastLogMessage;
};

// ======================= HUMAN STRATEGY =======================

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();
    ~HumanPlayerStrategy() override;

    bool issueOrder(Player *player, Map *map) override;
    std::vector<Territory *> toAttack(Player *player, Map *map) const override;
    std::vector<Territory *> toDefend(Player *player) const override;
    std::string getStrategyName() const override;

    std::string stringToLog() const override;

private:
    std::string lastLogMessage;
};

// ======================= AGGRESSIVE STRATEGY =======================

class AggressivePlayerStrategy : public PlayerStrategy {
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

// ======================= BENEVOLENT STRATEGY =======================

class BenevolentPlayerStrategy : public PlayerStrategy {
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

// ======================= NEUTRAL STRATEGY =======================

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy();
    ~NeutralPlayerStrategy() override;

    bool issueOrder(Player *player, Map *map) override;
    std::vector<Territory *> toAttack(Player *player, Map *map) const override;
    std::vector<Territory *> toDefend(Player *player) const override;
    std::string getStrategyName() const override;
};

// ======================= CHEATER STRATEGY =======================

class CheaterPlayerStrategy : public PlayerStrategy {
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
    void instantConquerCheat(const std::vector<Territory *> &toConquer, Player *player, Map *map);
    void duplicateReinforcementsCheat(Player *player);
};

#endif // PLAYERS_STRATEGY_H
