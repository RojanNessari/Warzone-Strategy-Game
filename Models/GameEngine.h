#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

// Forward declarations
class Player;

class State
{
public:
    State(const string &name);
    State(const State &other);
    State &operator=(const State &other);
    ~State();

    void addTransition(const string &cmd, State *next);
    State *nextState(const string &cmd) const;
    const string &getName() const;
    const map<string, State *> &getTransitions() const;

    friend ostream &operator<<(ostream &os, const State &s);

private:
    string *name_;
    map<string, State *> *transitions_;
};

class GameEngine
{
public:
    GameEngine();
    GameEngine(const GameEngine &other);
    GameEngine &operator=(const GameEngine &other);
    ~GameEngine();

    void buildGraph();
    bool applyCommand(const string &cmd);
    const State *current() const;

    // Assignment 2 – Part 2
    void startupPhase();
    Player* getNeutralPlayer();
    void addTruce(Player* a, Player* b);
    bool isTruced(Player* a, Player* b) const;
    void clearTrucesForNewTurn();

    friend ostream &operator<<(ostream &os, const GameEngine &ge);

private:
    void clear();
    State *findState(const string &name) const;

    State *current_;
    vector<State *> *states_;
    vector<Player *> players;
};

#endif
