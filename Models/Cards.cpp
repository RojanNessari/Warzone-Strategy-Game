#include "Cards.h"
#include "Player.h"
#include "Orders.h"
#include <iostream>
#include <random>
#include <sstream>
#include <algorithm>

using namespace std;


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
 lastLogMessage = string("Card created: ") + CardTypeToString(t);
    Notify(this, "INFO");
}

Card::Card(const Card &other) : type(other.type)
{
}

Card &Card::operator=(const Card &other)
{
    if (this != &other)
    {
        type = other.type;
    }
    return *this;
}

Card::~Card()
{
    lastLogMessage = string("Card destroyed: ") + CardTypeToString(type);
    Notify(this, "INFO");
}

ostream &operator<<(ostream &os, const Card &card)
{
    os << "Card(Type: " << CardTypeToString(card.type) << ")";
    return os;
}

void Card::play(Player &player, OrdersList &ordersList, Deck &deck)
{
    lastLogMessage=string("playing cards:")+CardTypeToString(type);
    Notify(this, "INFO");

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
        newOrder = new Bomb();
        lastLogMessage= "order created: Bomb";
        Notify(this,"COMBAT");
        break;
    case CardType::Reinforcement:
        newOrder = new Deploy();
          lastLogMessage = "Order created: Deploy (Reinforcement)";
        Notify(this, "INVENTORY");
        break;
    case CardType::Blockade:
        newOrder = new Blockade();
        lastLogMessage = "Order created: Blockade";
        Notify(this, "COMBAT");
        break;
    case CardType::Airlift:
       
        newOrder = new Airlift();
       lastLogMessage = "Order created: Airlift";
        Notify(this, "COMBAT");
        break;
    case CardType::Diplomacy:
        
        newOrder = new Negotiate();
           lastLogMessage = "Order created: Diplomacy (Negotiate)";
        Notify(this, "INFO");
        break;

    }

    if (newOrder)
        ordersList.add(newOrder);
   

    // After playing, return this card to the deck.
    deck.returnCard(this);
    lastLogMessage = "Card returned to deck.";
    Notify(this, "INFO");
}

std::string Card::stringToLog()const
{
    return lastLogMessage;
}



// ----------------- Hand -----------------
Hand::Hand()
{
    lastLogMessage = "Hand constructed (0 cards)";
    Notify(this, "INFO");
}

size_t Hand::size() const { return cards.size(); } // Returns length of number of cards in hands

Hand::Hand(const Hand &other)
{
    for (auto *c : other.cards)
    {
        cards.push_back(new Card(*c));
    }
    lastLogMessage = "Hand copy-constructed with " + to_string(cards.size()) + " cards";
    Notify(this, "DEBUG");
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
         lastLogMessage = "Hand assigned with " + to_string(cards.size()) + " cards";
        Notify(this, "DEBUG");
    }
    return *this;
}

Hand::~Hand()
{
    for (auto *c : cards)
        delete c;
     lastLogMessage = "Hand destroyed (all cards deleted)";
    Notify(this, "INFO");
}

void Hand::addCard(Card *c)
{
    cards.push_back(c);
    ostringstream oss;
    oss << *c;
    lastLogMessage = "Hand::addCard -> Added: " + oss.str();
    Notify(this, "INFO");
}

Card *Hand::removeAt(size_t index)
{
    if (index >= cards.size())
    {
        lastLogMessage = "Hand::removeAt -> Invalid index.";
        Notify(this, "WARNING");
        return nullptr;
    }
    Card *c = cards[index];
    cards.erase(cards.begin() + index);
    ostringstream oss;
    oss << *c;
    lastLogMessage = "Hand::removeAt -> Removed: " + oss.str();
    Notify(this, "INFO");
    return c;
}

void Hand::playCard(size_t index, Player &player, OrdersList &ordersList, Deck &deck)
{
    if (index >= cards.size())
    {
        lastLogMessage = "Hand::playCard invalid index";
        Notify(this, "WARNING");
        return;
    }
    Card *c = removeAt(index);
    // Card::play() returns the card to the deck; do not delete c here.
    c->play(player, ordersList, deck);
}
std::string Hand::stringToLog()const
{
    return lastLogMessage;
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
       lastLogMessage = "Deck constructed with " + to_string(cards.size()) + " cards.";
    Notify(this, "INFO");
}

Deck::Deck(const Deck &other)
{
    for (auto *c : other.cards)
        cards.push_back(new Card(*c));
   
    lastLogMessage = "Deck copy-constructed with " + to_string(cards.size()) + " cards";
    Notify(this, "DEBUG");
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
        lastLogMessage = "Deck assigned with " + to_string(cards.size()) + " cards";
        Notify(this, "DEBUG");
    }
    return *this;
}

Deck::~Deck()
{
    for (auto *c : cards)
        delete c;
     lastLogMessage = "Deck destroyed (all cards deleted)";
    Notify(this, "INFO");
}

Card *Deck::draw(Player & /*player*/, Hand &hand)
{
    if (cards.empty())
    {
        lastLogMessage = "Deck::draw empty deck!";
        Notify(this, "WARNING");
        return nullptr;
    }
    uniform_int_distribution<size_t> dist(0, cards.size() - 1);
    size_t idx = dist(rng);

    Card *c = cards[idx];
    cards.erase(cards.begin() + idx);

    hand.addCard(c);

    ostringstream oss;
    oss << *c;
    lastLogMessage = "Deck::draw -> gave: " + oss.str();
    Notify(this, "INFO");
    return c;
}

void Deck::returnCard(Card *c)
{
    cards.push_back(c);
    ostringstream oss;
    oss << *c;
    lastLogMessage = "Deck::returnCard received " + oss.str();
    Notify(this, "INFO");
}
std::string Deck::stringToLog()const
{
    return lastLogMessage;
}
ostream &operator<<(ostream &os, const Deck &d)
{
    os << "Deck with " << d.cards.size() << " cards.";
    return os;
}
