#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <utility>
#include <iostream>
#include "../utils/LoggingObserver.h"
using namespace std;

// Forward declarations
class Player;
class Deck;
class Map;

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

class GameEngine : public Subject, public ILoggable
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
    Player *getNeutralPlayer();
    void addTruce(Player *a, Player *b);
    bool isTruced(Player *a, Player *b) const;
    void clearTrucesForNewTurn();

    // Assignment 2 – Part 3: Main Game Loop
    void mainGameLoop();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();

    friend ostream &operator<<(ostream &os, const GameEngine &ge);
    // Assignment 3 - Part 2
    void runTournament(const vector<string> &mapFiles,
                       const vector<string> &strategies,
                       int numGames,
                       int maxTurns);

    string runSingleGame(const string &mapFile,
                         const vector<string> &strategies,
                         int maxTurns);

    void generateTournamentReport(const vector<vector<string>> &results,
                                  const vector<string> &mapFiles,
                                  const vector<string> &strategies,
                                  int numGames,
                                  int maxTurns);

private:
    void clear();
    State *findState(const string &name) const;

    State *current_;
    vector<State *> *states_;
    vector<Player *> players;
    Player *neutralPlayer = nullptr;
    Map *gameMap = nullptr;
    Deck *gameDeck = nullptr;
    struct PairHash
    {
        size_t operator()(const pair<int, int> &p) const
        {
            return (static_cast<size_t>(p.first) << 32) ^ static_cast<size_t>(p.second);
        }
    };
    unordered_set<pair<int, int>, PairHash> truces;
};

#endif
