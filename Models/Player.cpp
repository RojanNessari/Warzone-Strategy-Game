
#include "Player.h"
#include "Map.h"
#include <iostream>
#include "Orders.h"
#include "Cards.h"
#include "Map.h"
#include "../utils/logger.h"
using namespace std;

// Default constructor
Player::Player(const std::string &playerName) : territories(), handOfCards(new Hand()), orders(new OrdersList()), playerName(playerName), reinforcementPool(0), strategy(nullptr)
{
    lastLogMessage = "Player created: " + playerName;
    Notify(this, "INFO");
   
}

// getPlayer Name:
string Player::getPlayerName() const
{
    return playerName;
}

// Reinforcement pool methods
int Player::getReinforcementPool() const
{
    lastLogMessage = playerName + " reinforcement pool set to " + to_string(reinforcementPool);
    Notify(this, "INVENTORY");
    return reinforcementPool;
}

void Player::setReinforcementPool(int armies)
{
    reinforcementPool = armies;
}

void Player::addReinforcements(int armies)
{
    reinforcementPool += armies;
     lastLogMessage = playerName + " gained " + to_string(armies)
                     + " reinforcements (total=" + to_string(reinforcementPool) + ")";
    Notify(this, "INVENTORY");
}

// Copy constructor
Player::Player(const Player &other)
    : territories(other.territories),
      handOfCards(other.handOfCards ? new Hand(*other.handOfCards) : nullptr),
      orders(other.orders ? new OrdersList(*other.orders) : nullptr),
      playerName(other.playerName),
      reinforcementPool(other.reinforcementPool),
      strategy(nullptr) // clone if your strategy class provides a clone method
{
    lastLogMessage = "Player copy-constructed: " + playerName;
    Notify(this, "DEBUG");
    // If Strategy has clone(): strategy = other.strategy ? other.strategy->clone() : nullptr;
}
std::string Player::stringToLog()const
{
    return lastLogMessage;
}

// Assignment operator
Player &Player::operator=(const Player &other)
{
    if (this == &other)
        return *this;

    // Delete current owned resources
    if (handOfCards) { delete handOfCards; handOfCards = nullptr; }
    if (orders) { delete orders; orders = nullptr; }
    if (strategy) { delete strategy; strategy = nullptr; }

    // Deep-copy owned resources from other
    handOfCards = other.handOfCards ? new Hand(*other.handOfCards) : nullptr;
    orders = other.orders ? new OrdersList(*other.orders) : nullptr;
    // strategy = other.strategy ? other.strategy->clone() : nullptr; // uncomment if clone exists

    // Shallow copy for territories (they are owned by Map, not Player)
    territories = other.territories;

    // copy simple fields
    playerName = other.playerName;
    reinforcementPool = other.reinforcementPool;
    // copy other scalar fields as needed (id, flags, etc.)

    lastLogMessage = "Player assigned: " + playerName;
    Notify(this, "DEBUG");
    return *this;
}

// Destructor
Player::~Player()
{
    lastLogMessage = "Player destroyed: " + playerName;
    Notify(this, "INFO");

    if (handOfCards) {
        delete handOfCards;
        handOfCards = nullptr;
    }
    if (orders) {
        delete orders;
        orders = nullptr;
    }
    // If strategy is heap allocated and owned by Player, delete it.
    // If strategy is shared/owned elsewhere, remove this delete.
    if (strategy) {
        delete strategy;
        strategy = nullptr;
    }
    // territories point to Territory objects owned by Map; do NOT delete them here.
}

Hand *Player::getHandOfCards() const { return handOfCards; }

// Methods
int Player::getId() const { return id; }
void Player::setId(int pid) { id = pid; }

int Player::takeFromReinforcement(int n)
{
    int take = std::max(0, std::min(n, reinforcementPool));
    reinforcementPool -= take;
     lastLogMessage = playerName + " took " + to_string(take)
                     + " armies from reinforcement pool.";
    Notify(this, "INVENTORY");
    return take;
}
void Player::addToReinforcement(int n) { 
    reinforcementPool += std::max(0, n);
    lastLogMessage = playerName + " reinforcement increased by " + to_string(n);
    Notify(this, "INVENTORY"); }

bool Player::ownsTerritoryId(int tid) const
{
    for (const Territory *t : territories)
    {
        if (t && t->getId() == tid)
            return true;
    }
    return false;
}

void Player::setStrategy(PlayerStrategy *newStrategy)
{
    if (strategy != nullptr)
    {
        delete strategy; // delete previous strategy
    }

    strategy = newStrategy;
    lastLogMessage = playerName + " strategy changed to " + strategy->getStrategyName();
    Notify(this, "INFO");
}
vector<Territory *> Player::toDefend()
{
    if (strategy != nullptr)
    {
        return strategy->toDefend(this);
    }
    lastLogMessage ="Strategy -> nullptr";
    Notify(this, "ERROR");  
    
    return vector<Territory *>();
    // Return a subset of territories to defend (arbitrary logic for now)
    // return territories; // Placeholder: return all territories
}

vector<Territory *> Player::toAttack(Map *map)
{
    if (strategy != nullptr)
    {
        return strategy->toAttack(this, map);
    }
    lastLogMessage = "Strategy -> nullptr for toAttack()";
    Notify(this, "ERROR");
    
    return vector<Territory *>(); // fallback

    /*
    // Return neighboring enemy territories that can be attacked
    vector<Territory *> attackTargets;

    if (map == nullptr)
    {
        logMessage(WARNING, "toAttack() called with null map");
        return attackTargets;
    }

    // For each territory owned by this player
    for (Territory *ownedTerritory : territories)
    {
        if (ownedTerritory == nullptr)
            continue;

        // Get all adjacent territory IDs
        const std::unordered_set<int> &adjacentIds = ownedTerritory->getAdjacentIds();

        // Check each adjacent territory
        for (int adjId : adjacentIds)
        {
            // Look up the adjacent territory in the map
            Territory *adjacentTerritory = map->getTerritoryById(adjId);

            if (adjacentTerritory == nullptr)
                continue;

            // Check if this adjacent territory is owned by an enemy (not us)
            if (adjacentTerritory->getOwner() != this && adjacentTerritory->getOwner() != nullptr)
            {
                // This is an enemy territory adjacent to our territory
                attackTargets.push_back(adjacentTerritory);
            }
        }
    }

    return attackTargets;*/
}

OrdersList *Player::getOrdersList() const
{
    return orders;
}

bool Player::issueOrder(Map *map)
{
    if (strategy != nullptr)
    {
        return strategy->issueOrder(this, map);
    }
    logMessage(ERROR, "strategy -> nullptr for issueOrder()");
    return false; // fallback

    /*
    // Priority 1: Deploy orders while reinforcement pool has armies
    if (reinforcementPool > 0)
    {
        // Get territories to defend
        vector<Territory *> toDefendList = toDefend();

        if (toDefendList.empty())
        {
            logMessage(WARNING, playerName + " has reinforcements but no territories to defend!");
            return false;
        }

        // Deploy to the first territory in toDefend list
        Territory *targetTerritory = toDefendList[0];

        // Deploy all remaining armies (or could deploy incrementally)
        int armiesToDeploy = reinforcementPool;

        Order *deployOrder = new Deploy(this, targetTerritory, armiesToDeploy);
        orders->add(deployOrder);
        reinforcementPool -= armiesToDeploy;
        logMessage(INFO, playerName + " issues Deploy order: " +
                             std::to_string(armiesToDeploy) + " armies to " + targetTerritory->getName());

        return true; // Still has orders to issue
    }

    // Priority 2: Advance orders (after all deployments are done)
    // Issue advance orders to defend or attack
    if (!territories.empty())
    {
        // Get lists of territories to defend and attack
        vector<Territory *> defendList = toDefend();
        vector<Territory *> attackList = toAttack(map); // Pass map parameter

        // Strategy: Try to move armies to defend first, then attack

        // Option 1: Move armies between own territories for defense
        if (defendList.size() > 1)
        {
            // Find a source territory with armies to move
            for (Territory *source : territories)
            {
                if (source->getArmies() > 1)
                {
                    // Move to first territory in defend list
                    Territory *target = defendList[0];

                    // Don't move to the same territory
                    if (source != target)
                    {
                        int armiesToMove = source->getArmies() / 2; // Move half

                        if (armiesToMove > 0)
                        {
                            Order *advanceOrder = new Advance(this, source, target, armiesToMove);
                            orders->add(advanceOrder);

                            logMessage(INFO, playerName + " issues Advance order (defend): " +
                                                 std::to_string(armiesToMove) + " armies from " +
                                                 source->getName() + " to " + target->getName());

                            return true;
                        }
                    }
                }
            }
        }
    }

    // Priority 3: Play cards from hand
    if (handOfCards && handOfCards->size() > 0)
    {
        // Play the first card in hand
        // The Hand::playCard method will:
        // 1. Create appropriate order based on card type
        // 2. Add order to the orders list
        // 3. Return card to deck

        logMessage(INFO, playerName + " plays a card from hand");

        // Note: We need a Deck reference to return the card
        // For now, we'll just log that we would play a card
        // In a full implementation, this would need access to the game's Deck

        // TODO: Need to refactor to have access to Deck
        // handOfCards->playCard(0, *this, *orders, deck);

        logMessage(INFO, playerName + " has " + std::to_string(handOfCards->size()) +
                             " cards but cannot play without Deck reference");

        // Return false to indicate done issuing for this turn
        // This prevents infinite loop while card playing is incomplete
        return false;
    }

    // No more orders to issue
    logMessage(INFO, playerName + " is done issuing orders for this turn");
    return false;*/
}

// Stream insertion operator
ostream &operator<<(ostream &os, const Player &player)
{
    os << "Player with "
       << player.territories.size() << " territories, ";
    if (player.handOfCards)
        os << *(player.handOfCards);
    else
        os << "0 cards, ";
    if (player.orders)
        os << player.orders->size();
    else
        os << 0;
    os << " orders.";
    return os;
}
