#include "Orders.h"

//  Base Order 
Order::Order() : description("Generic Order"), effect("None") {}
Order::~Order() {}


std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.description;
    if (order.effect != "None") {
        os << " | Effect: " << order.effect;
    }
    return os;
}

//  Deploy 
Deploy::Deploy() { description = "Deploy Order"; }
bool Deploy::validate() { return true; }
void Deploy::execute() { effect = "Deployed armies"; }

Advance::Advance() { description = "Advance Order"; }
bool Advance::validate() { return true; }
void Advance::execute() { effect = "Advanced armies"; }

Bomb::Bomb() { description = "Bomb Order"; }
bool Bomb::validate() { return true; }
void Bomb::execute() { effect = "Territory bombed"; }

Blockade::Blockade() { description = "Blockade Order"; }
bool Blockade::validate() { return true; }
void Blockade::execute() { effect = "Blockade created"; }

Airlift::Airlift() { description = "Airlift Order"; }
bool Airlift::validate() { return true; }
void Airlift::execute() { effect = "Airlifted armies"; }

Negotiate::Negotiate() { description = "Negotiate Order"; }
bool Negotiate::validate() { return true; }
void Negotiate::execute() { effect = "Negotiation started"; }


//  OrdersList 
OrdersList::OrdersList() {}

OrdersList::~OrdersList() {
    for (auto o : orders) {
        delete o;
    }
}

void OrdersList::add(Order* order) {
    orders.push_back(order);
}

void OrdersList::print() const {
    for (size_t i = 0; i < orders.size(); i++) {
        std::cout << i << ": " << *orders[i] << std::endl;
    }
}
void OrdersList::remove(int index) {
    if (index >= 0 && index < (int)orders.size()) {
        delete orders[index];
        orders.erase(orders.begin() + index);
    }
}

void OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < (int)orders.size() &&
        toIndex >= 0 && toIndex < (int)orders.size()) {
        Order* temp = orders[fromIndex];
        orders.erase(orders.begin() + fromIndex);
        orders.insert(orders.begin() + toIndex, temp);
    }
}
size_t OrdersList::size() const {
    return orders.size();
}

Order* OrdersList::get(int index) const {
    if (index >= 0 && index < (int)orders.size()) {
        return orders[index];
    }
    return nullptr;
}


