#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <vector>

// base Order
class Order
{
public:
    // Constructor
    Order();
    // Deconstructor
    virtual ~Order();
    // Copy constructor
    Order(const Order &otherOrder);
    // Assignment Operator
    Order &operator=(const Order &otherOrder);

    virtual bool validate() = 0;
    virtual void execute() = 0;

    // For printing orders
    friend std::ostream &operator<<(std::ostream &os, const Order &order);

protected:
    std::string description;
    std::string effect;
};

// Order Subclasses
class Deploy : public Order
{
public:
    Deploy();
    ~Deploy();                         // deconstructor
    Deploy(const Deploy &otherDeploy); // copy Constructor
    Deploy &operator=(const Deploy &otherDeploy);
    bool validate() override;
    void execute() override;
};

class Advance : public Order
{
public:
    Advance();
    ~Advance();
    Advance(const Advance &otherAdvance);
    Advance &operator=(const Advance &otherAdvance);
    bool validate() override;
    void execute() override;
};

class Bomb : public Order
{
public:
    Bomb();
    ~Bomb();
    Bomb(const Bomb &otherBomb);
    Bomb &operator=(const Bomb &otherBomb);
    bool validate() override;
    void execute() override;
};

class Blockade : public Order
{
public:
    Blockade();
    ~Blockade();
    Blockade(const Blockade &otherBlockade);
    Blockade &operator=(const Blockade &otherBlockade);
    bool validate() override;
    void execute() override;
};

class Airlift : public Order
{
public:
    Airlift();
    ~Airlift();
    Airlift(const Airlift &otherAirlift);
    Airlift &operator=(const Airlift &otherAirlift);
    bool validate() override;
    void execute() override;
};

class Negotiate : public Order
{
public:
    Negotiate();
    ~Negotiate();
    Negotiate(const Negotiate &otherNegotiate);
    Negotiate &operator=(const Negotiate &otherNegotiate);
    bool validate() override;
    void execute() override;
};

// OrdersList
class OrdersList
{
private:
    std::vector<Order *> orders;

public:
    OrdersList();
    ~OrdersList();

    // Copy constructor
    OrdersList(const OrdersList &other);

    // Assignment operator
    OrdersList &operator=(const OrdersList &other);

    OrdersList &getOrders();
    const OrdersList &getOrders() const;

    void add(Order *order);
    void print() const;
    void remove(int index);
    void move(int fromIndex, int toIndex);

    size_t size() const;
    Order *get(int index) const;
};

#endif // ORDERS_H
