#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <iostream>

class Player
{

public:
    // constructor
    Player();

    // copy constructor
    Player(const Player &other0bject);

    // Assighment operator
    Player &operator=(const Player &other);

    // destructor
    ~Player();

    // Required functions
    std::vector<std::string> toDefend();
    std::vector<std::string> toAttack();
    void issueOrder();

    // stream insertion operator overload
    friend std::ostream &operator<<(std::ostream &os, const Player &player);

private:
    std::vector<std::string> *territories; // Pointer: list of territories
    std::vector<std::string> *handOfCards; // Pointer: list of cards
    std::vector<std::string> *orders;
};

#endif
