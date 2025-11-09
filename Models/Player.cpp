
#include "Player.h"
#include <iostream>
#include "Orders.h"
#include "Cards.h"
#include "Map.h"
using namespace std;

// Default constructor
Player::Player() : territories(), handOfCards(new Hand()), orders(new OrdersList())
{
    cout << "Player created." << endl;
}

// Copy constructor
Player::Player(const Player &other)
{
    // Deep copy territories pointers (shallow copy, you may want to deep copy if needed)
    territories = other.territories;
    // Deep copy Hand
    handOfCards = new Hand(*other.handOfCards);
    // Deep copy OrdersList
    orders = new OrdersList(*other.orders);
    cout << "Player copied." << endl;
}

// Assignment operator
Player &Player::operator=(const Player &other)
{
    if (this != &other)
    {
        territories = other.territories;
        if (handOfCards)
            delete handOfCards;
        handOfCards = new Hand(*other.handOfCards);
        if (orders)
            delete orders;
        orders = new OrdersList(*other.orders);
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
int Player::getId() const { return id; }
void Player::setId(int pid) { id = pid; }

int Player::takeFromReinforcement(int n) {
    int take = std::max(0, std::min(n, reinforcementPool));
    reinforcementPool -= take;
    return take;
}
void Player::addToReinforcement(int n) { reinforcementPool += std::max(0, n); }

bool Player::ownsTerritoryId(int tid) const {
    for (const Territory* t : territories) {
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
