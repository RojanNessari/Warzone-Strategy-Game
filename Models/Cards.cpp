#include "Cards.h"
#include "Player.h"
#include "Orders.h"
#include <iostream>
#include <random>
#include <sstream>
#include <algorithm>
#include "../utils/logger.h"
using namespace std;

// ---------- Debug helper (internal) ----------
static constexpr bool DEBUG_CARDS = false;
static inline void DebugPrint(const string &msg)
{
    if (DEBUG_CARDS)
    {
        logMessage(DEBUG, msg);
    }
}

// ---------- Utilities ----------
const char *CardTypeToString(CardType type)
{
    switch (type)
    {
    case CardType::Bomb:
        return "Bomb";
    case CardType::Reinforcement:
        return "Reinforcement";
    case CardType::Blockade:
        return "Blockade";
    case CardType::Airlift:
        return "Airlift";
    case CardType::Diplomacy:
        return "Diplomacy";
    default:
        return "Unknown";
    }
}

// One RNG for the TU
static random_device rd;
static mt19937 rng(rd());

// ----------------- Card -----------------
Card::Card(CardType t) : type(t)
{
    DebugPrint(string("Card created: ") + CardTypeToString(t));
}

Card::Card(const Card &other) : type(other.type)
{
    DebugPrint("Card copy-constructed");
}

Card &Card::operator=(const Card &other)
{
    if (this != &other)
    {
        type = other.type;
        DebugPrint("Card assigned");
    }
    return *this;
}

Card::~Card()
{
    DebugPrint(string("Card destroyed: ") + CardTypeToString(type));
}

ostream &operator<<(ostream &os, const Card &card)
{
    os << "Card(Type: " << CardTypeToString(card.type) << ")";
    return os;
}

void Card::play(Player &player, OrdersList &ordersList, Deck &deck)
{
    logMessage(COMBAT, string("Playing card: ") + CardTypeToString(type));
    Notify(this, COMBAT, string("Playing card: ") + CardTypeToString(type));

    // Create the appropriate Order and add it to OrdersList.
    // Mapping:
    //   Bomb          -> Bomb
    //   Reinforcement -> Deploy   (typical Warzone mapping)
    //   Blockade      -> Blockade
    //   Airlift       -> Airlift
    //   Diplomacy     -> Negotiate
    Order *newOrder = nullptr;
    switch (type)
    {
    case CardType::Bomb:
        logMessage(INVENTORY, " -> Creating Bomb order.");
        Notify(this, INVENTORY, " -> Creating Bomb order.");
        newOrder = new Bomb();
        break;
    case CardType::Reinforcement:
        logMessage(INVENTORY, " -> Creating Deploy order (from Reinforcement).");
        Notify(this, INVENTORY, " -> Creating Deploy order (from Reinforcement).");
        newOrder = new Deploy();
        break;
    case CardType::Blockade:
        logMessage(DEBUG, " -> Creating Blockade order.");
        Notify(this, INVENTORY, " -> Creating Blockade order.");
        newOrder = new Blockade();
        break;
    case CardType::Airlift:
        logMessage(INVENTORY, " -> Creating Airlift order.");
        Notify(this, INVENTORY, " -> Creating Airlift order.");
        newOrder = new Airlift();
        break;
    case CardType::Diplomacy:
        logMessage(INVENTORY, " -> Creating Negotiate order.");
        Notify(this, INVENTORY, " -> Creating Negotiate order.");
        newOrder = new Negotiate();
        break;
    default:
        logMessage(ERROR, "Unknown card type, no order created.");
        Notify(this, ERROR, "Unknown card type, no order created.");
        break;
    }

    if (newOrder)
    {
        ordersList.add(newOrder);
        logMessage(INVENTORY, "Order added to OrdersList.");
        Notify(this, INVENTORY, "Order added to OrdersList.");
    }
    else
    {
        logMessage(WARNING, "No order created.");
        Notify(this, WARNING, "No order created.");
    }

    // After playing, return this card to the deck.
    deck.returnCard(this);
    logMessage(INVENTORY, " -> Card returned to deck.");
    Notify(this, INVENTORY, " -> Card returned to deck.");
}

// ----------------- Hand -----------------
Hand::Hand()
{
    DebugPrint("Hand constructed (0 cards)");
}

size_t Hand::size() const { return cards.size(); } // Returns length of number of cards in hands

Hand::Hand(const Hand &other)
{
    for (auto *c : other.cards)
    {
        cards.push_back(new Card(*c));
    }
    DebugPrint("Hand copy-constructed with " + to_string(cards.size()) + " cards");
}

vector<Card *> Hand::getCards() const
{
    return cards;
}

Hand &Hand::operator=(const Hand &other)
{
    if (this != &other)
    {
        for (auto *c : cards)
            delete c;
        cards.clear();
        for (auto *c : other.cards)
        {
            cards.push_back(new Card(*c));
        }
        DebugPrint("Hand assigned with " + to_string(cards.size()) + " cards");
    }
    return *this;
}

Hand::~Hand()
{
    for (auto *c : cards)
        delete c;
    DebugPrint("Hand destroyed (all cards deleted)");
}

void Hand::addCard(Card *c)
{
    cards.push_back(c);
    ostringstream oss;
    oss << *c;
    DebugPrint("Added to hand: " + oss.str());
}

Card *Hand::removeAt(size_t index)
{
    if (index >= cards.size())
    {
        DebugPrint("Hand::removeAt invalid index");
        return nullptr;
    }
    Card *c = cards[index];
    cards.erase(cards.begin() + index);
    ostringstream oss;
    oss << *c;
    DebugPrint("Removed from hand: " + oss.str());
    return c;
}

void Hand::playCard(size_t index, Player &player, OrdersList &ordersList, Deck &deck)
{
    if (index >= cards.size())
    {
        DebugPrint("Hand::playCard invalid index");
        return;
    }
    Card *c = removeAt(index);
    // Card::play() returns the card to the deck; do not delete c here.
    c->play(player, ordersList, deck);
}

ostream &operator<<(ostream &os, const Hand &h)
{
    os << "Hand with " << h.cards.size() << " cards: ";
    for (auto *c : h.cards)
        os << *c << ' ';
    return os;
}

// ----------------- Deck -----------------
Deck::Deck()
{
    for (int i = 0; i < 10; ++i)
    {
        cards.push_back(new Card(CardType::Bomb));
        cards.push_back(new Card(CardType::Reinforcement));
        cards.push_back(new Card(CardType::Blockade));
        cards.push_back(new Card(CardType::Airlift));
        cards.push_back(new Card(CardType::Diplomacy));
    }
    shuffle(cards.begin(), cards.end(), rng);
    DebugPrint("Deck constructed with " + to_string(cards.size()) + " cards");
}

Deck::Deck(const Deck &other)
{
    for (auto *c : other.cards)
        cards.push_back(new Card(*c));
    DebugPrint("Deck copy-constructed with " + to_string(cards.size()) + " cards");
}

Deck &Deck::operator=(const Deck &other)
{
    if (this != &other)
    {
        for (auto *c : cards)
            delete c;
        cards.clear();
        for (auto *c : other.cards)
            cards.push_back(new Card(*c));
        DebugPrint("Deck assigned with " + to_string(cards.size()) + " cards");
    }
    return *this;
}

Deck::~Deck()
{
    for (auto *c : cards)
        delete c;
    DebugPrint("Deck destroyed (all cards deleted)");
}

Card *Deck::draw(Player & /*player*/, Hand &hand)
{
    if (cards.empty())
    {
        DebugPrint("Deck::draw on empty deck");
        Notify(this, INFO, "Deck::draw on empty deck");
        return nullptr;
    }
    uniform_int_distribution<size_t> dist(0, cards.size() - 1);
    size_t idx = dist(rng);

    Card *c = cards[idx];
    cards.erase(cards.begin() + idx);

    hand.addCard(c);

    ostringstream oss;
    oss << *c;
    DebugPrint("Deck::draw gave " + oss.str() + " to player's hand");
    Notify(this, INFO, "Deck::draw gave " + oss.str() + " to player's hand");
    return c;
}

void Deck::returnCard(Card *c)
{
    cards.push_back(c);
    ostringstream oss;
    oss << *c;
    DebugPrint("Deck::returnCard received " + oss.str());
}

ostream &operator<<(ostream &os, const Deck &d)
{
    os << "Deck with " << d.cards.size() << " cards.";
    return os;
}
