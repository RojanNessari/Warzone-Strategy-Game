#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include "GameEngine.h"
#include "random"
#include "algorithm"



//  Base Order
Order::Order() : description("Generic Order"), effect("None") {}
Order::~Order() {}

Order::Order(const Order &otherOrder) : description(otherOrder.description), effect(otherOrder.effect) {}

Order &Order::operator=(const Order &otherOrder)
{
    if (this != &otherOrder)
    {
        description = otherOrder.description;
        effect = otherOrder.effect;
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

//  Deploy
Deploy::Deploy() { description = "Deploy Order"; }
Deploy::Deploy(Player* p, Territory* t, int a)
    : issuer(p), target(t), armies(a) { description = "Deploy Order"; }
Deploy::~Deploy() {}

Deploy::Deploy(const Deploy &otherDeploy) : Order(otherDeploy), issuer(otherDeploy.issuer), target(otherDeploy.target), armies(otherDeploy.armies) {}

Deploy &Deploy::operator=(const Deploy &otherDeploy)
{
    if (this != &otherDeploy)
    {
        Order::operator=(otherDeploy);
        this -> issuer = otherDeploy.issuer;
        this -> target = otherDeploy.target;
        this -> armies = otherDeploy.armies;
    }
    return *this;
}

Order* Deploy::clone() const { return new Deploy(*this);}

bool Deploy::validate() { 
    if(!issuer || !target) return false;
    return target ->getOwnerId() == issuer->getId(); }
void Deploy::execute()
{
    if (!validate()) { effect = "Invalid: target not owned by issuer."; 
        return; }
int moved = issuer->takeFromReinforcement(armies);
target->addArmies(moved);

effect = "Deployed " + std::to_string(moved) + " to " + target->getName();
}

Advance::Advance() { description = "Advance Order"; }
Advance::~Advance() {}

Advance::Advance(const Advance &otherAdvance) : Order(otherAdvance) {}

Advance &Advance::operator=(const Advance &otherAdvance)
{
    if (this != &otherAdvance)
    {
        Order::operator=(otherAdvance);
    }
    return *this;
}

bool Advance::validate() { return true; }
void Advance::execute()
{
    if (validate())
    {
        effect = "Advanced armies";
    }
}

Bomb::Bomb() { description = "Bomb Order"; }
Bomb::~Bomb() {}

Bomb::Bomb(const Bomb &otherBomb) : Order(otherBomb) {}

Bomb &Bomb::operator=(const Bomb &otherBomb)
{
    if (this != &otherBomb)
    {
        Order::operator=(otherBomb);
    }
    return *this;
}

bool Bomb::validate() { return true; }
void Bomb::execute()
{
    if (validate())
    {
        effect = "Territory bombed";
    }
}

Blockade::Blockade() { description = "Blockade Order"; }
Blockade::~Blockade() {}

Blockade::Blockade(const Blockade &otherBlockade) : Order(otherBlockade) {}

Blockade &Blockade::operator=(const Blockade &otherBlockade)
{
    if (this != &otherBlockade)
    {
        Order::operator=(otherBlockade);
    }
    return *this;
}

bool Blockade::validate() { return true; }
void Blockade::execute()
{
    if (validate())
    {
        effect = "Blockade created";
    }
}

Airlift::Airlift() { description = "Airlift Order"; }
Airlift::~Airlift() {}

Airlift::Airlift(const Airlift &otherAirlift) : Order(otherAirlift) {}

Airlift &Airlift::operator=(const Airlift &otherAirlift)
{
    if (this != &otherAirlift)
    {
        Order::operator=(otherAirlift);
    }
    return *this;
}

bool Airlift::validate() { return true; }
void Airlift::execute()
{
    if (validate())
    {
        effect = "Airlifted armies";
    }
}

Negotiate::Negotiate() { description = "Negotiate Order"; }
Negotiate::~Negotiate() {}

Negotiate::Negotiate(const Negotiate &otherNegotiate) : Order(otherNegotiate) {}

Negotiate &Negotiate::operator=(const Negotiate &otherNegotiate)
{
    if (this != &otherNegotiate)
    {
        Order::operator=(otherNegotiate);
    }
    return *this;
}

bool Negotiate::validate() { return true; }
void Negotiate::execute()
{
    if (validate())
    {
        effect = "Negotiation started";
    }
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
    // For now, don't copy the orders to avoid double deletion
    // In a full implementation, you'd need proper polymorphic cloning
    // orders remains empty for safety
}

// Assignment operator
OrdersList &OrdersList::operator=(const OrdersList &other)
{
    if (this != &other)
    {
        // Delete existing orders
        for (auto o : orders)
        {
            delete o;
        }
        orders.clear();

        // Don't copy orders from other to avoid double deletion
        // In a full implementation, you'd need proper polymorphic cloning
    }
    return *this;
}

void OrdersList::add(Order *order)
{
    orders.push_back(order);
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
