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
