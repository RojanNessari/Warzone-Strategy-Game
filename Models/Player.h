#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <iostream>
class Territory;
class OrdersList;
class Hand;

class Player
{
public:
    // Constructor
    Player();

    // Copy constructor
    Player(const Player &other);

    // Assignment operator
    Player &operator=(const Player &other);

    // Destructor
    ~Player();

    // Required functions
    std::vector<Territory *> toDefend() const; // Return a collection of Territories to be defended
    std::vector<Territory *> toAttack() const; // Return a collection of Territories to be attacked
    void issueOrder();

    // Stream insertion operator overload
    friend std::ostream &operator<<(std::ostream &os, const Player &player);

    Hand *getHandOfCards() const;

private:
    std::vector<Territory *> territories; // List of territories owned by the player (as pointers)
    Hand *handOfCards;                    // List of cards owned by the player
    OrdersList *orders;                   // List of orders issued by the player (as pointer)
};

#endif
