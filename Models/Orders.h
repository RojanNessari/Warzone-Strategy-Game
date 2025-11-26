#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <vector>
#include "../utils/LoggingObserver.h"

class GameEngine;
class Player;
class Territory;
class Map;

// base Order
class Order : public Subject, public ILoggable
{
public:
    // Constructor
    Order(Player *issuingPlayer);
    Order();
    // Deconstructor
    virtual ~Order();
    // Copy constructor
    Order(const Order &otherOrder);
    // Assignment Operator
    Order &operator=(const Order &otherOrder);

    virtual bool validate() = 0;
    virtual void execute() = 0;
    virtual Order *clone() const = 0;

    std::string getEffect() const { return effect; }
    bool isExecuted() const { return executed; }
    Player *getIssuer() const { return issuer; }

    // For printing orders
    friend std::ostream &operator<<(std::ostream &os, const Order &order);

protected:
    std::string description;
    std::string effect;
    bool executed;
    Player *issuer;
};

// Order Subclasses
class Deploy : public Order
{
public:
    Deploy(Player *issuer, Territory *target, int armies);
    Deploy();
    ~Deploy();                         // deconstructor
    Deploy(const Deploy &otherDeploy); // copy Constructor
    Deploy &operator=(const Deploy &otherDeploy);
    bool validate() override;
    void execute() override;
    Order *clone() const override;

private:
    Player *issuer = nullptr;
    Territory *target = nullptr;
    int armies = 0;
};

class Advance : public Order
{
public:
    Advance(Player *issuer, Territory *source, Territory *target, int armies);
    Advance();
    ~Advance();
    Advance(const Advance &otherAdvance);
    Advance &operator=(const Advance &otherAdvance);
    bool validate() override;
    void execute() override;
    Order *clone() const override;

private:
    Player *issuer = nullptr;
    Territory *source = nullptr;
    Territory *target = nullptr;
    int armies = 0;
};

class Bomb : public Order
{
public:
    Bomb(Player *issuer, Territory *target, Map *map);
    Bomb();
    ~Bomb();
    Bomb(const Bomb &otherBomb);
    Bomb &operator=(const Bomb &otherBomb);
    bool validate() override;
    void execute() override;
    Order *clone() const override;

private:
    Player *issuer = nullptr;
    Territory *target = nullptr;
    Map *map = nullptr;
};

class Blockade : public Order
{
public:
    Blockade(Player *issuer, Territory *target, GameEngine *engine);
    Blockade();
    ~Blockade();
    Blockade(const Blockade &otherBlockade);
    Blockade &operator=(const Blockade &otherBlockade);
    bool validate() override;
    void execute() override;
    Order *clone() const override;

private:
    Player *issuer = nullptr;
    Territory *target = nullptr;
    GameEngine *engine = nullptr;
};

class Airlift : public Order
{
public:
    Airlift(Player *issuer, Territory *source, Territory *target, int armies);
    Airlift();
    ~Airlift();
    Airlift(const Airlift &otherAirlift);
    Airlift &operator=(const Airlift &otherAirlift);
    bool validate() override;
    void execute() override;
    Order *clone() const override;

private:
    Player *issuer = nullptr;
    Territory *source = nullptr;
    Territory *target = nullptr;
    int armies = 0;
};

class Negotiate : public Order
{
public:
    Negotiate(Player *issuer, Player *other, GameEngine *engine);
    Negotiate();
    ~Negotiate();
    Negotiate(const Negotiate &otherNegotiate);
    Negotiate &operator=(const Negotiate &otherNegotiate);
    bool validate() override;
    void execute() override;
    Order *clone() const override;

private:
    Player *issuer = nullptr;
    Player *other = nullptr;
    GameEngine *engine = nullptr;
};

// OrdersList
class OrdersList : public Subject, public ILoggable
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
    void clear();

    size_t size() const;
    Order *get(int index) const;
};

#endif // ORDERS_H
