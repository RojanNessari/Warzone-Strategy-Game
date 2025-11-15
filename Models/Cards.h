#ifndef CARDS_H
#define CARDS_H

#include <vector>
#include <iosfwd>

class Player;
class OrdersList;
class Deck;
class Card;
// ----------------- Card Types -----------------
enum class CardType
{
    Bomb,
    Reinforcement,
    Blockade,
    Airlift,
    Diplomacy
};

const char *CardTypeToString(CardType type);

// ----------------- Card -----------------

class Card
{
private:
    CardType type;

public:
    explicit Card(CardType t);
    Card(const Card &other);
    Card &operator=(const Card &other);
    ~Card();

    // Play creates an Order (adds to OrdersList) then returns this card to the Deck
    void play(Player &player, OrdersList &ordersList, Deck &deck);

    friend std::ostream &operator<<(std::ostream &os, const Card &card);
};

// ----------------- Hand -----------------
class Hand
{
private:
    std::vector<Card *> cards;

public:
    Hand();
    Hand(const Hand &other);
    Hand &operator=(const Hand &other);
    ~Hand();

    size_t size() const;
    std::vector<Card *> getCards() const;

    void addCard(Card *c);
    Card *removeAt(std::size_t idx);

    // Remove card at index, call Card::play (which returns the card to the deck)
    void playCard(std::size_t index, Player &player, OrdersList &ordersList, Deck &deck);

    friend std::ostream &operator<<(std::ostream &os, const Hand &h);
};

// ----------------- Deck -----------------
class Deck
{
private:
    std::vector<Card *> cards;

public:
    Deck();
    Deck(const Deck &other);
    Deck &operator=(const Deck &other);
    ~Deck();

    // Draw a random card from the deck and add it to the player's hand.
    Card *draw(Player &player, Hand &hand);

    // When a card is played, it is returned to the deck.
    void returnCard(Card *c);

    friend std::ostream &operator<<(std::ostream &os, const Deck &d);
};

#endif // CARDS_H
