#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <vector>

//  base Order
class Order {
public:
    Order();
    virtual ~Order();

    virtual bool validate() = 0;
    virtual void execute() = 0;

    // For printing orders
    friend std::ostream& operator<<(std::ostream& os, const Order& order);

protected:
    std::string description;
    std::string effect;
};

//  deploy Order
class Deploy : public Order {
public:
    Deploy();
    bool validate() override;
    void execute() override;
};

//  OrdersList 
class OrdersList {
private:
    std::vector<Order*> orders;

public:
    OrdersList();
    ~OrdersList();

    void add(Order* order);
    void print() const;
};

#endif 
