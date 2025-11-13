#ifndef PLAYERS_STRATEGIES_H
#define PLAYERS_STRATEGIES_H

#include "../Models/Player.h";

class PlayerStrategies
{
public:
    virtual PlayerStrategies() = 0; // Pure Virtual class

    virtual ~PlayerStrategies(); // deconstructor

    // Player Game Methods
    // These methods are purely virtual
    virtual issueOrder() = 0;
    virtual toAttack() = 0;
    virtual toDefend() = 0;

private:
    Player *p;
}

#endif