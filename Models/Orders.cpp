#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include "GameEngine.h"
#include <random>
#include <algorithm>
#include <string>

//  Base Order
Order::Order(Player *issuingPlayer) : description("Generic Order"), effect("None"), executed(false), issuer(issuingPlayer) {}
Order::Order() : description("Generic Order"), effect("None"), executed(false), issuer(nullptr) {}

Order::~Order() {}

Order::Order(const Order &otherOrder) : description(otherOrder.description), effect(otherOrder.effect), executed(otherOrder.executed), issuer(otherOrder.issuer) {}

Order &Order::operator=(const Order &otherOrder)
{
    if (this != &otherOrder)
    {
        description = otherOrder.description;
        effect = otherOrder.effect;
        executed = otherOrder.executed;
        issuer = otherOrder.issuer;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Order &order)
{
    os << order.description;
    if (order.effect != "None")
    {
        os << " | Effect: " << order.effect;
    }
    return os;
}

std::string Order::stringToLog()
{
    return "Order: " + description + ", Effect: " + effect;
}

//  Deploy
Deploy::Deploy()
{
    description = "Deploy Order";
}
Deploy::Deploy(Player *p, Territory *t, int a)
    : issuer(p), target(t), armies(a) { description = "Deploy Order"; }
Deploy::~Deploy() {}

Deploy::Deploy(const Deploy &otherDeploy) : Order(otherDeploy), issuer(otherDeploy.issuer), target(otherDeploy.target), armies(otherDeploy.armies) {}

Deploy &Deploy::operator=(const Deploy &otherDeploy)
{
    if (this != &otherDeploy)
    {
        Order::operator=(otherDeploy);
        this->issuer = otherDeploy.issuer;
        this->target = otherDeploy.target;
        this->armies = otherDeploy.armies;
    }
    return *this;
}

Order *Deploy::clone() const { return new Deploy(*this); }

bool Deploy::validate()
{
    if (!issuer || !target)
        return false;
    if (target->getOwner() != issuer)
        return false;
    if (armies > issuer->getReinforcementPool())
        return false;
    return true;
}
void Deploy::execute()
{
    if (!validate())
    {
        effect = "Invalid: target not owned by issuer.";
        Notify(this);
        return;
    }
    if (issuer->getReinforcementPool() < armies)
    {
        effect = "Invalid: not enough reinforcement armies.";
        Notify(this);
        return;
    }

    issuer->setReinforcementPool(issuer->getReinforcementPool() - armies);
    target->addArmies(armies);

    effect = "Deployed " + std::to_string(armies) + " to " + target->getName();
    executed = true;
    Notify(this);
}

Advance::Advance() { description = "Advance Order"; }
Advance::Advance(Player *p, Territory *s, Territory *t, int a)
    : issuer(p), source(s), target(t), armies(a) { description = "Advance Order"; }
Advance::~Advance() {}

Advance::Advance(const Advance &otherAdvance) : Order(otherAdvance), issuer(otherAdvance.issuer), source(otherAdvance.source), target(otherAdvance.target), armies(otherAdvance.armies) {}

Advance &Advance::operator=(const Advance &otherAdvance)
{
    if (this != &otherAdvance)
    {
        Order::operator=(otherAdvance);
        issuer = otherAdvance.issuer;
        source = otherAdvance.source;
        target = otherAdvance.target;
        armies = otherAdvance.armies;
    }
    return *this;
}

Order *Advance::clone() const { return new Advance(*this); }

bool Advance::validate()
{
    if (!issuer || !source || !target)
        return false;
    if (source->getOwner() != issuer)
        return false; // source has to be yours
    if (source->getArmies() <= 0)
        return false;
    if (!source->isAdjacentTo(target->getId()))
        return false;

    return true;
}
void Advance::execute()
{
    if (!validate())
    {
        effect = "Invalid: source not owned or not adjacent.";
        Notify(this); // Notify observers
        return;
    }
    if (target->getOwner() == issuer)
    {
        // friendly move
        int src = source->getArmies();
        int moved = std::max(0, std::min(armies, src));
        source->setArmies(src - moved);
        target->setArmies(target->getArmies() + moved);
        effect = "Moved " + std::to_string(moved) + " to defend " + target->getName();
        Notify(this);
        return;
    }

    // enemy: battle simulation 60% / 70%
    int src = source->getArmies();
    int atk = std::max(0, std::min(armies, src));
    source->setArmies(src - atk);
    int def = target->getArmies();

    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution atkHit(0.6), defHit(0.7);

    int a = atk, d = def;
    while (a > 0 && d > 0)
    {
        int aKills = 0, dKills = 0;
        for (int i = 0; i < a; ++i)
            if (atkHit(rng))
                aKills++;
        for (int i = 0; i < d; ++i)
            if (defHit(rng))
                dKills++;
        d = std::max(0, d - aKills);
        a = std::max(0, a - dKills);
    }

    target->setArmies(d);
    if (d == 0 && a > 0)
    {
        // capture territory; survivors occupy
        Player *prevOwner = target->getOwner();
        (void)prevOwner; // if unused
        target->setOwner(issuer);
        target->setArmies(a);
        issuer->markConqueredThisTurn();
        effect = "Conquered " + target->getName() + " with " + std::to_string(a) + " survivors.";
    }
    else
    {
        effect = "Attack ended. Defender left " + std::to_string(d) + ", attackers left " + std::to_string(a) + ".";
    }
    executed = true;
    Notify(this); // Notify Observers
}

Bomb::Bomb() { description = "Bomb Order"; }
Bomb::Bomb(Player *p, Territory *t, Map *m)
    : issuer(p), target(t), map(m) { description = "Bomb Order"; }
Bomb::~Bomb() {}

Bomb::Bomb(const Bomb &otherBomb) : Order(otherBomb), issuer(otherBomb.issuer), target(otherBomb.target), map(otherBomb.map) {}

Bomb &Bomb::operator=(const Bomb &otherBomb)
{
    if (this != &otherBomb)
    {
        Order::operator=(otherBomb);
        issuer = otherBomb.issuer;
        target = otherBomb.target;
        map = otherBomb.map;
    }
    return *this;
}

Order *Bomb::clone() const { return new Bomb(*this); }

bool Bomb::validate()
{
    if (!issuer || !target)
        return false;

    if (target->getOwner() == issuer)
        return false; // Cannot bomb own territory

    // Must be adjacent to at least one of the issuer's territories
    bool adjacent = false;
    for (auto *myT : issuer->toDefend())
    {
        if (myT && myT->isAdjacentTo(target->getId()))
        {
            adjacent = true;
            break;
        }
    }
    return adjacent;
}
void Bomb::execute()
{
    if (!validate())
    {
        effect = "Invalid: target is not enemy or not adjacent to issuer territories.";
        Notify(this);
        return;
    }
    int before = target->getArmies();
    int after = before / 2;
    target->setArmies(after);
    effect = "Bombed " + target->getName() + " from " + std::to_string(before) + " to " + std::to_string(after);
    executed = true;
    Notify(this);
}

Blockade::Blockade() { description = "Blockade Order"; }
Blockade::Blockade(Player *p, Territory *t, GameEngine *eng)
    : issuer(p), target(t), engine(eng) { description = "Blockade Order"; }
Blockade::~Blockade() {}

Blockade::Blockade(const Blockade &otherBlockade) : Order(otherBlockade), issuer(otherBlockade.issuer), target(otherBlockade.target), engine(otherBlockade.engine) {}

Blockade &Blockade::operator=(const Blockade &otherBlockade)
{
    if (this != &otherBlockade)
    {
        Order::operator=(otherBlockade);
        issuer = otherBlockade.issuer;
        target = otherBlockade.target;
        engine = otherBlockade.engine;
    }
    return *this;
}

Order *Blockade::clone() const { return new Blockade(*this); }

bool Blockade::validate()
{
    if (!issuer || !target || !engine)
        return false;
    return target->getOwner() == issuer; // must be own territory
}

void Blockade::execute()
{
    if (!validate())
    {
        effect = "Invalid: target must be owned by issuer.";
        return;
        Notify(this);
    }
    target->setArmies(target->getArmies() * 2);
    Player *neutral = engine->getNeutralPlayer();
    target->setOwner(neutral);
    effect = "Blockade: doubled armies and transferred " + target->getName() + " to Neutral.";
    executed = true;
    Notify(this); // Notify Observers
}

Airlift::Airlift() { description = "Airlift Order"; }
Airlift::Airlift(Player *p, Territory *s, Territory *t, int a)
    : issuer(p), source(s), target(t), armies(a) { description = "Airlift Order"; }
Airlift::~Airlift() {}

Airlift::Airlift(const Airlift &otherAirlift) : Order(otherAirlift), issuer(otherAirlift.issuer), source(otherAirlift.source), target(otherAirlift.target), armies(otherAirlift.armies) {}
Airlift &Airlift::operator=(const Airlift &otherAirlift)
{
    if (this != &otherAirlift)
    {
        Order::operator=(otherAirlift);
        issuer = otherAirlift.issuer;
        source = otherAirlift.source;
        target = otherAirlift.target;
        armies = otherAirlift.armies;
    }
    return *this;
}

Order *Airlift::clone() const { return new Airlift(*this); }

bool Airlift::validate()
{
    if (!issuer || !source || !target)
        return false;
    return source->getOwner() == issuer && target->getOwner() == issuer;
}
void Airlift::execute()
{
    if (!validate())
    {
        effect = "Invalid: source/target must both be owned by issuer.";
        Notify(this); // Notify Observers
        return;
    }
    int src = source->getArmies();
    int moved = std::max(0, std::min(armies, src));
    source->setArmies(src - moved);
    target->setArmies(target->getArmies() + moved);
    effect = "Airlifted " + std::to_string(moved) + " from " + source->getName() + " to " + target->getName();
    executed = true;
    Notify(this);
}

Negotiate::Negotiate() { description = "Negotiate Order"; }
Negotiate::Negotiate(Player *p, Player *o, GameEngine *eng)
    : issuer(p), other(o), engine(eng) { description = "Negotiate (Diplomacy) Order"; }
Negotiate::~Negotiate() {}

Negotiate::Negotiate(const Negotiate &otherNegotiate) : Order(otherNegotiate), issuer(otherNegotiate.issuer), other(otherNegotiate.other), engine(otherNegotiate.engine) {}

Negotiate &Negotiate::operator=(const Negotiate &otherNegotiate)
{
    if (this != &otherNegotiate)
    {
        Order::operator=(otherNegotiate);
        issuer = otherNegotiate.issuer;
        other = otherNegotiate.other;
        engine = otherNegotiate.engine;
    }
    return *this;
}

Order *Negotiate::clone() const { return new Negotiate(*this); }

bool Negotiate::validate()
{
    if (!issuer || !other || !engine)
        return false;
    return issuer != other;
}
void Negotiate::execute()
{
    if (!validate())
    {
        effect = "Invalid: cannot negotiate with self.";
        Notify(this);
        return;
    }
    engine->addTruce(issuer, other); // store truce for rest of this turn
    effect = "Negotiation: attacks between players blocked for this turn.";
    executed = true;
    Notify(this);
}

//  OrdersList
OrdersList::OrdersList() {}

OrdersList::~OrdersList()
{
    for (auto o : orders)
    {
        delete o;
    }
}

// Copy constructor
OrdersList::OrdersList(const OrdersList &other)
{
    orders.reserve(other.orders.size());
    for (Order *o : other.orders)
        orders.push_back(o ? o->clone() : nullptr);
}

// Assignment operator
OrdersList &OrdersList::operator=(const OrdersList &other)
{
    if (this != &other)
    {
        for (auto *o : orders)
            delete o;
        orders.clear();
        orders.reserve(other.orders.size());
        for (Order *o : other.orders)
            orders.push_back(o ? o->clone() : nullptr);
    }
    return *this;
}

void OrdersList::clear()
{
    for (auto *o : orders)
    {
        delete o;
    }
    orders.clear();
}

void OrdersList::add(Order *order)
{
    if (order != nullptr)
    {
        orders.push_back(order);
        Notify(this); // Notify observer
    }
}

void OrdersList::print() const
{
    for (size_t i = 0; i < orders.size(); i++)
    {
        std::cout << i << ": " << *orders[i] << std::endl;
    }
}
void OrdersList::remove(int index)
{
    if (index >= 0 && index < (int)orders.size())
    {
        delete orders[index];
        orders.erase(orders.begin() + index);
    }
}

void OrdersList::move(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < (int)orders.size() &&
        toIndex >= 0 && toIndex < (int)orders.size())
    {
        Order *temp = orders[fromIndex];
        orders.erase(orders.begin() + fromIndex);
        orders.insert(orders.begin() + toIndex, temp);
    }
}
size_t OrdersList::size() const
{
    return orders.size();
}

OrdersList &OrdersList::getOrders() { return *this; }
const OrdersList &OrdersList::getOrders() const { return *this; }

Order *OrdersList::get(int index) const
{
    if (index >= 0 && index < (int)orders.size())
    {
        return orders[index];
    }
    return nullptr;
}

std::string OrdersList::stringToLog()
{
    std::string log = "OrdersList: ";
    if (orders.empty())
    {
        log += "Empty";
    }
    else
    {
        log += std::to_string(orders.size()) + " order(s)";
    }
    return log;
}
