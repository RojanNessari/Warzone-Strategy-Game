#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <iostream>

class Player;
class OrderList;

enum class CardType {
    Bomb,
    Reinforcement,
    Blockade,
    Airlift,
    Diplomacy
};

const char* CardTypeToString(CardType type);

// single definition will live in Cards.cpp
extern const bool DEBUG;
void DebugPrint(const std::string& message);

// ----------------- Card -----------------
class Deck; // forward declare for play()

class Card {
private:
    CardType type;

public:
    explicit Card(CardType t);
    Card(const Card& other);
    Card& operator=(const Card& other);
    ~Card();

    // Creates an order (Part 3) and returns this card to the deck
    void play(Player& player, OrderList& ordersList, Deck& deck);

    friend std::ostream& operator<<(std::ostream& os, const Card& card);
};

// ----------------- Hand -----------------
class Hand {
private:
    std::vector<Card*> cards;

public:
    Hand();
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);
    ~Hand();

    void addCard(Card* c);
    Card* removeAt(size_t index);

    // Remove card at index, call Card::play (which returns the card to the deck)
    void playCard(size_t index, Player& player, OrderList& ordersList, Deck& deck);

    // optional helper you were using before
    Hand& getHand() { return *this; }

    friend std::ostream& operator<<(std::ostream& os, const Hand& h);
};

// ----------------- Deck -----------------
class Deck {
private:
    std::vector<Card*> cards;

public:
    Deck();
    Deck(const Deck& other);
    Deck& operator=(const Deck& other);
    ~Deck();

    // Draw a random card from the deck and add it to the player's hand
    Card* draw(Player& player, Hand& hand);

    // When a card is played, it is returned to the deck
    void returnCard(Card* c);

    friend std::ostream& operator<<(std::ostream& os, const Deck& d);
};

#endif // CARDS_H
