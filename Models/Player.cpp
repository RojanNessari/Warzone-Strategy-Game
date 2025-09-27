

#include "Player.h"
#include <iostream>
using namespace std;

// Default constructor
Player::Player()
{
    territories = new vector<string>();
    handOfCards = new vector<string>();
    orders = new vector<string>();
    cout << "Player created." << endl;
}

// Copy constructor
Player::Player(const Player &other)
{
    territories = new vector<string>(*other.territories);
    handOfCards = new vector<string>(*other.handOfCards);
    orders = new vector<string>(*other.orders);
    cout << "Player copied." << endl;
}

// Assignment operator
Player &Player::operator=(const Player &other)
{
    if (this != &other)
    {
        delete territories;
        delete handOfCards;
        delete orders;
        territories = new vector<string>(*other.territories);
        handOfCards = new vector<string>(*other.handOfCards);
        orders = new vector<string>(*other.orders);
    }
    cout << "Player assigned." << endl;
    return *this;
}

// Destructor
Player::~Player()
{
    delete territories;
    delete handOfCards;
    delete orders;
    cout << "Player destroyed." << endl;
}

// Methods
vector<string> Player::toDefend()
{
    // Return a simple list for now
    return {"Territory1", "Territory2"};
}

vector<string> Player::toAttack()
{
    // Return a simple list for now
    return {"Territory3", "Territory4"};
}

void Player::issueOrder()
{
    orders->push_back("Sample Order");
    cout << "Order issued." << endl;
}

// Stream insertion operator
ostream &operator<<(ostream &os, const Player &player)
{
    os << "Player with "
       << player.territories->size() << " territories, "
       << player.handOfCards->size() << " cards, and "
       << player.orders->size() << " orders.";
    return os;
}
