#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
#include "../PlayerStrategies/PlayerStrategies.h"
#include "../utils/LoggingObserver.h"

class Player : public Subject, public ILoggable
{
public:
    // Constructor
    Player(const std::string &playerName);

    // Copy constructor
    Player(const Player &other);

    // Assignment operator
    Player &operator=(const Player &other);

    // Destructor
    ~Player();

    int getId() const;
    void setId(int pid);

    void addTerritory(Territory *territory)
    {
        territories.push_back(territory);
    }
    void removeTerritory(Territory *territory)
    {
        territories.erase(
            std::remove(territories.begin(), territories.end(), territory),
            territories.end());
    }
    std::vector<Territory *> getTerritories() const { return territories; }
    int takeFromReinforcement(int n); // for Deploy
    void addToReinforcement(int n);
    void markConqueredThisTurn() { conqueredThisTurn = true; }
    bool hasConqueredThisTurn() const { return conqueredThisTurn; }
    void resetConqueredFlag() { conqueredThisTurn = false; }

    bool ownsTerritoryId(int tid) const;
    // getter
    std::string getPlayerName() const;

    // Reinforcement pool methods
    int getReinforcementPool() const;
    void setReinforcementPool(int armies);
    void addReinforcements(int armies);

    // Required functions
    std::vector<Territory *> toDefend();         // Return a collection of Territories to be defended
    std::vector<Territory *> toAttack(Map *map); // Return a collection of Territories to be attacked
    bool issueOrder(Map *map, Deck *deck);       // Returns false when done issuing orders
    OrdersList *getOrdersList() const;           // Get the orders list

    // Stream insertion operator overload
    friend std::ostream &operator<<(std::ostream &os, const Player &player);

    Hand *getHandOfCards() const;

    // Set player strategy:
    void setStrategy(PlayerStrategy *newStrategy);
    string getPlayerStrategyName() const;
    PlayerStrategy *getStrategy() const;

private:
    int id = -1;
    int reinforcementPool = 0;
    std::vector<Territory *> territories; // List of territories owned by the player (as pointers)
    Hand *handOfCards;                    // List of cards owned by the player
    OrdersList *orders;                   // List of orders issued by the player (as pointer)
    bool conqueredThisTurn = false;
    std::string playerName; // Player name
    PlayerStrategy *strategy;
};

#endif
