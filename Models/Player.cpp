
#include "Player.h"
#include "Map.h"
#include <iostream>
#include "Orders.h"
#include "Cards.h"
#include "Map.h"
#include "../utils/logger.h"
using namespace std;

// Default constructor
Player::Player(const std::string &playerName) : territories(), handOfCards(new Hand()), orders(new OrdersList()), playerName(playerName), reinforcementPool(0)
{
    logMessage(INFO, "Player created.");
}

// getPlayer Name:
string Player::getPlayerName() const
{
    return playerName;
}

// Reinforcement pool methods
int Player::getReinforcementPool() const
{
    return reinforcementPool;
}

void Player::setReinforcementPool(int armies)
{
    reinforcementPool = armies;
}

void Player::addReinforcements(int armies)
{
    reinforcementPool += armies;
}

// Copy constructor
Player::Player(const Player &other)
{
    playerName = other.playerName;
    reinforcementPool = other.reinforcementPool;

    // Deep copy territories
    for (auto *territory : other.territories)
    {
        territories.push_back(new Territory(*territory)); // Create a new Territory object for each
    }

    // Deep copy Hand
    handOfCards = new Hand(*other.handOfCards);

    // Deep copy OrdersList
    orders = new OrdersList(*other.orders);

    logMessage(DEBUG, "Player copied.");
}

// Assignment operator
Player &Player::operator=(const Player &other)
{
    if (this != &other) // Check for self-assignment
    {
        // Deep copy territories
        for (auto *territory : territories)
        {
            delete territory; // Clean up existing territories
        }
        territories.clear();
        for (auto *territory : other.territories)
        {
            territories.push_back(new Territory(*territory)); // Deep copy each territory
        }

        // Deep copy handOfCards
        if (handOfCards)
            delete handOfCards;
        handOfCards = other.handOfCards ? new Hand(*other.handOfCards) : nullptr;

        // Deep copy orders
        if (orders)
            delete orders;
        orders = other.orders ? new OrdersList(*other.orders) : nullptr;

        playerName = other.playerName;
        reinforcementPool = other.reinforcementPool;
    }
    logMessage(DEBUG, "Player assigned.");
    return *this;
}

// Destructor
Player::~Player()
{
    if (handOfCards)
        delete handOfCards;
    if (orders)
        delete orders;
    logMessage(INFO, "Player destroyed.");
}

Hand *Player::getHandOfCards() const { return handOfCards; }

// Methods
int Player::getId() const { return id; }
void Player::setId(int pid) { id = pid; }

int Player::takeFromReinforcement(int n)
{
    int take = std::max(0, std::min(n, reinforcementPool));
    reinforcementPool -= take;
    return take;
}
void Player::addToReinforcement(int n) { reinforcementPool += std::max(0, n); }

bool Player::ownsTerritoryId(int tid) const
{
    for (const Territory *t : territories)
    {
        if (t && t->getId() == tid)
            return true;
    }
    return false;
}

vector<Territory *> Player::toDefend() const
{
    // Return a subset of territories to defend (arbitrary logic for now)
    return territories; // Placeholder: return all territories
}

vector<Territory *> &Player::toDefend()
{
    // Non-const version for modifying territories
    return territories;
}

vector<Territory *> Player::toAttack() const
{
    // Return a subset of territories to attack (arbitrary logic for now)
    return territories; // Placeholder: return all territories
}

OrdersList *Player::getOrdersList() const
{
    return orders;
}

bool Player::issueOrder()
{
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

        logMessage(INFO, playerName + " issues Deploy order: " +
                             std::to_string(armiesToDeploy) + " armies to " + targetTerritory->getName());

        return true; // Still has orders to issue
    }

    // Priority 2: Advance orders (after all deployments are done)
    // For simplicity, we'll issue one advance order if we have territories
    if (!territories.empty() && territories[0]->getArmies() > 1)
    {
        // Try to find an adjacent territory to attack or defend
        Territory *source = territories[0];

        // Get adjacent territories
        const std::unordered_set<int> &adjacentIds = source->getAdjacentIds();

        if (!adjacentIds.empty())
        {
            // Just take the first adjacent territory
            int targetId = *adjacentIds.begin();

            // For now, we need to find the actual Territory object
            // This is a simplified approach - in real implementation you'd need access to the Map
            // For demonstration, we'll create an advance order to move armies between own territories

            if (territories.size() > 1)
            {
                // Move armies between our own territories
                Territory *target = territories[1];
                int armiesToMove = source->getArmies() / 2;

                if (armiesToMove > 0)
                {
                    Order *advanceOrder = new Advance(this, source, target, armiesToMove);
                    orders->add(advanceOrder);

                    logMessage(INFO, playerName + " issues Advance order: " +
                                         std::to_string(armiesToMove) + " armies from " +
                                         source->getName() + " to " + target->getName());

                    return true;
                }
            }
        }
    }

    // Priority 3: Play cards from hand
    if (handOfCards && handOfCards->size() > 0)
    {
        // For demonstration, we could play a card
        // This is simplified - in full implementation, you'd use specific card logic
        // For now, we'll skip this and just say we're done
        logMessage(INFO, playerName + " has cards but choosing not to play them this turn");
    }

    // No more orders to issue
    logMessage(INFO, playerName + " is done issuing orders for this turn");
    return false;
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
