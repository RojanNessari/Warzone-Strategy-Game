#include "Cards.h"
#include "Player.h"
#include "Orders.h"   
#include <iostream>
#include <random>
#include <sstream>
#include <algorithm>

using namespace std;


const bool DEBUG = true;

void DebugPrint(const string& message) {
    if (DEBUG) cout << "[DEBUG] " << message << '\n';
}

const char* CardTypeToString(CardType type) {
    switch (type) {
        case CardType::Bomb:          return "Bomb";
        case CardType::Reinforcement: return "Reinforcement";
        case CardType::Blockade:      return "Blockade";
        case CardType::Airlift:       return "Airlift";
        case CardType::Diplomacy:     return "Diplomacy";
        default:                      return "Unknown";
    }
}


static std::random_device rd;
static std::mt19937 rng(rd());

// ----------------- Card -----------------
Card::Card(CardType t) : type(t) {
    DebugPrint(std::string("Card created: ") + std::string(CardTypeToString(t)));
}

Card::Card(const Card& other) : type(other.type) {
    DebugPrint("Card copy-constructed");
}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        type = other.type;
        DebugPrint("Card assigned");
    }
    return *this;
}

Card::~Card() {
    DebugPrint(std::string("Card destroyed: ") + std::string(CardTypeToString(type)));
}

ostream& operator<<(ostream& os, const Card& card) {
    os << "Card(Type: " << CardTypeToString(card.type) << ")";
    return os;
}

void Card::play(Player& player, OrderList& ordersList, Deck& deck) {
    cout << "Playing card: " << CardTypeToString(type) << '\n';

    // ---- Create the appropriate Order (Part 3) ----
    // If Part 3 isn't ready, you can comment this block and keep the stub log.
    Order* newOrder = nullptr;
    switch (type) {
        case CardType::Bomb:
            cout << " -> Creating Bomb order.\n";
            newOrder = new BombOrder(&player);
            break;
        case CardType::Reinforcement:
            cout << " -> Creating Reinforcement order.\n";
            newOrder = new ReinforcementOrder(&player);
            break;
        case CardType::Blockade:
            cout << " -> Creating Blockade order.\n";
            newOrder = new BlockadeOrder(&player);
            break;
        case CardType::Airlift:
            cout << " -> Creating Airlift order.\n";
            newOrder = new AirliftOrder(&player);
            break;
        case CardType::Diplomacy:
            cout << " -> Creating Negotiate/Diplomacy order.\n";
            newOrder = new NegotiateOrder(&player);
            break;
        default:
            cout << "❌ Unknown card type, no order created.\n";
            break;
    }

    if (newOrder) {
        // Use your OrderList API. If it's different, adjust here.
        ordersList.add(newOrder);
        cout << "✅ Order added to the order list.\n";
    } else {
        cout << "⚠️  (Stub) No order created because Part 3 classes may be missing.\n";
    }

    // After playing, card goes back into the deck
    deck.returnCard(this);
    cout << " -> Card returned to deck.\n";
}

// ----------------- Hand -----------------
Hand::Hand() {
    DebugPrint("Hand constructed (0 cards)");
}

Hand::Hand(const Hand& other) {
    for (auto* c : other.cards) {
        cards.push_back(new Card(*c));
    }
    DebugPrint("Hand copy-constructed with " + to_string(cards.size()) + " cards");
}

Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        for (auto* c : cards) delete c;
        cards.clear();
        for (auto* c : other.cards) cards.push_back(new Card(*c));
        DebugPrint("Hand assigned with " + to_string(cards.size()) + " cards");
    }
    return *this;
}

Hand::~Hand() {
    for (auto* c : cards) delete c;
    DebugPrint("Hand destroyed (all cards deleted)");
}

void Hand::addCard(Card* c) {
    cards.push_back(c);
    ostringstream oss; oss << *c;
    DebugPrint("Added to hand: " + oss.str());
}

Card* Hand::removeAt(size_t index) {
    if (index >= cards.size()) {
        DebugPrint("Hand::removeAt invalid index");
        return nullptr;
    }
    Card* c = cards[index];
    cards.erase(cards.begin() + index);
    ostringstream oss; oss << *c;
    DebugPrint("Removed from hand: " + oss.str());
    return c;
}

void Hand::playCard(size_t index, Player& player, OrderList& ordersList, Deck& deck) {
    if (index >= cards.size()) {
        DebugPrint("Hand::playCard invalid index");
        return;
    }
    Card* c = removeAt(index);
    // Card::play() returns the card to the deck; do not delete c here.
    c->play(player, ordersList, deck);
}

ostream& operator<<(ostream& os, const Hand& h) {
    os << "Hand with " << h.cards.size() << " cards: ";
    for (auto* c : h.cards) os << *c << ' ';
    return os;
}

// ----------------- Deck -----------------
Deck::Deck() {
   for (int i = 0; i < 10; ++i) {
    cards.push_back(new Card(CardType::Bomb));
    cards.push_back(new Card(CardType::Reinforcement));
    cards.push_back(new Card(CardType::Blockade));
    cards.push_back(new Card(CardType::Airlift));
    cards.push_back(new Card(CardType::Diplomacy));
    }
    std::shuffle(cards.begin(), cards.end(), rng);
    DebugPrint("Deck constructed with " + to_string(cards.size()) + " cards");
}

Deck::Deck(const Deck& other) {
    for (auto* c : other.cards) cards.push_back(new Card(*c));
    DebugPrint("Deck copy-constructed with " + to_string(cards.size()) + " cards");
}

Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        for (auto* c : cards) delete c;
        cards.clear();
        for (auto* c : other.cards) cards.push_back(new Card(*c));
        DebugPrint("Deck assigned with " + to_string(cards.size()) + " cards");
    }
    return *this;
}

Deck::~Deck() {
    for (auto* c : cards) delete c;
    DebugPrint("Deck destroyed (all cards deleted)");
}

Card* Deck::draw(Player& player, Hand& hand) {
    if (cards.empty()) {
        DebugPrint("Deck::draw on empty deck");
        return nullptr;
    }
    size_t idx = rand() % cards.size();
    Card* c = cards[idx];
    cards.erase(cards.begin() + idx);

    hand.addCard(c);

    DebugPrint("Deck::draw gave " + oss.str() + " to player's hand");
    return c;
}

void Deck::returnCard(Card* c) {
    cards.push_back(c);

    DebugPrint("Deck::returnCard received " + oss.str());
}

ostream& operator<<(ostream& os, const Deck& d) {
    os << "Deck with " << d.cards.size() << " cards.";
    return os;
}