#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <vector>

// base Order
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

// Order Subclasses 
class Deploy : public Order {
public:
    Deploy();
    bool validate() override;
    void execute() override;
};

class Advance : public Order {
public:
    Advance();
    bool validate() override;
    void execute() override;
};

class Bomb : public Order {
public:
    Bomb();
    bool validate() override;
    void execute() override;
};

class Blockade : public Order {
public:
    Blockade();
    bool validate() override;
    void execute() override;
};

class Airlift : public Order {
public:
    Airlift();
    bool validate() override;
    void execute() override;
};

class Negotiate : public Order {
public:
    Negotiate();
    bool validate() override;
    void execute() override;
};

// OrdersList 
class OrdersList {
private:
    std::vector<Order*> orders;

public:
    OrdersList();
    ~OrdersList();

    void add(Order* order);
    void print() const;
    void remove(int index);
    void move(int fromIndex, int toIndex);


    size_t size() const;
    Order* get(int index) const;
};

#endif // ORDERS_H
