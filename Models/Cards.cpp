#include "Cards.h"
#include "Player.h"
#include "Orders.h"
#include <iostream>
#include <random>
#include <sstream>
#include <algorithm>

using std::ostream;

// ---------- Debug helper (internal) ----------
static constexpr bool DEBUG_CARDS = true;
static inline void DebugPrint(const std::string &msg)
{
    if (DEBUG_CARDS)
        std::cout << "[DEBUG] " << msg << '\n';
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
static std::random_device rd;
static std::mt19937 rng(rd());

// ----------------- Card -----------------
Card::Card(CardType t) : type(t)
{
    DebugPrint(std::string("Card created: ") + CardTypeToString(t));
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
    DebugPrint(std::string("Card destroyed: ") + CardTypeToString(type));
}

ostream &operator<<(ostream &os, const Card &card)
{
    os << "Card(Type: " << CardTypeToString(card.type) << ")";
    return os;
}

void Card::play(Player &player, OrdersList &ordersList, Deck &deck)
{
    std::cout << "Playing card: " << CardTypeToString(type) << '\n';

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
        std::cout << " -> Creating Bomb order.\n";
        newOrder = new Bomb();
        break;
    case CardType::Reinforcement:
        std::cout << " -> Creating Deploy order (from Reinforcement).\n";
        newOrder = new Deploy();
        break;
    case CardType::Blockade:
        std::cout << " -> Creating Blockade order.\n";
        newOrder = new Blockade();
        break;
    case CardType::Airlift:
        std::cout << " -> Creating Airlift order.\n";
        newOrder = new Airlift();
        break;
    case CardType::Diplomacy:
        std::cout << " -> Creating Negotiate order.\n";
        newOrder = new Negotiate();
        break;
    default:
        std::cout << "❌ Unknown card type, no order created.\n";
        break;
    }

    if (newOrder)
    {
        ordersList.add(newOrder);
        std::cout << "✅ Order added to OrdersList.\n";
    }
    else
    {
        std::cout << "⚠️  No order created.\n";
    }

    // After playing, return this card to the deck.
    deck.returnCard(this);
    std::cout << " -> Card returned to deck.\n";
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
    DebugPrint("Hand copy-constructed with " + std::to_string(cards.size()) + " cards");
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
        DebugPrint("Hand assigned with " + std::to_string(cards.size()) + " cards");
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
    std::ostringstream oss;
    oss << *c;
    DebugPrint("Added to hand: " + oss.str());
}

Card *Hand::removeAt(std::size_t index)
{
    if (index >= cards.size())
    {
        DebugPrint("Hand::removeAt invalid index");
        return nullptr;
    }
    Card *c = cards[index];
    cards.erase(cards.begin() + index);
    std::ostringstream oss;
    oss << *c;
    DebugPrint("Removed from hand: " + oss.str());
    return c;
}

void Hand::playCard(std::size_t index, Player &player, OrdersList &ordersList, Deck &deck)
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
    std::shuffle(cards.begin(), cards.end(), rng);
    DebugPrint("Deck constructed with " + std::to_string(cards.size()) + " cards");
}

Deck::Deck(const Deck &other)
{
    for (auto *c : other.cards)
        cards.push_back(new Card(*c));
    DebugPrint("Deck copy-constructed with " + std::to_string(cards.size()) + " cards");
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
        DebugPrint("Deck assigned with " + std::to_string(cards.size()) + " cards");
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
        return nullptr;
    }
    std::uniform_int_distribution<std::size_t> dist(0, cards.size() - 1);
    std::size_t idx = dist(rng);

    Card *c = cards[idx];
    cards.erase(cards.begin() + idx);

    hand.addCard(c);

    std::ostringstream oss;
    oss << *c;
    DebugPrint("Deck::draw gave " + oss.str() + " to player's hand");
    return c;
}

void Deck::returnCard(Card *c)
{
    cards.push_back(c);
    std::ostringstream oss;
    oss << *c;
    DebugPrint("Deck::returnCard received " + oss.str());
}

ostream &operator<<(ostream &os, const Deck &d)
{
    os << "Deck with " << d.cards.size() << " cards.";
    return os;
}
