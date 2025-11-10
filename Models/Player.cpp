
#include "Player.h"
#include "Map.h"
#include <iostream>
#include "Orders.h"
#include "Cards.h"
using namespace std;

// Default constructor
Player::Player(const std::string &playerName) : territories(), handOfCards(new Hand()), orders(new OrdersList()), playerName(playerName), reinforcementPool(0)
{
    cout << "Player created." << endl;
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

    cout << "Player copied." << endl;
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
    cout << "Player assigned." << endl;
    return *this;
}

// Destructor
Player::~Player()
{
    if (handOfCards)
        delete handOfCards;
    if (orders)
        delete orders;
    cout << "Player destroyed." << endl;
}

Hand *Player::getHandOfCards() const { return handOfCards; }

// Methods
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

void Player::issueOrder()
{
    // For demonstration, create a Deploy order (you can modify this to accept parameters)
    Order *newOrder = new Deploy();
    orders->add(newOrder);
    cout << "Order issued." << endl;
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
