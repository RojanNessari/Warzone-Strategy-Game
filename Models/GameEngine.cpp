#include "GameEngine.h"
#include <utility>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
using namespace std;

// ---------- STATE CLASS ----------
State::State(const string &name)
    : name_(new string(name)), transitions_(new map<string, State *>) {}

State::State(const State &other)
    : name_(new string(*other.name_)), transitions_(new map<string, State *>) {}

State &State::operator=(const State &other)
{
    if (this != &other)
    {
        *name_ = *other.name_;
        transitions_->clear();
    }
    return *this;
}

State::~State()
{
    delete name_;
    delete transitions_;
}

void State::addTransition(const string &cmd, State *next)
{
    (*transitions_)[cmd] = next;
}

State *State::nextState(const string &cmd) const
{
    auto it = transitions_->find(cmd);
    return (it == transitions_->end()) ? nullptr : it->second;
}

const string &State::getName() const { return *name_; }
const map<string, State *> &State::getTransitions() const { return *transitions_; }

ostream &operator<<(ostream &os, const State &s)
{
    os << "State(" << *s.name_ << ")";
    return os;
}

// ---------- GAME ENGINE ----------
GameEngine::GameEngine()
    : current_(nullptr), states_(new vector<State *>) {}

GameEngine::~GameEngine()
{
    clear();
    delete states_;

    // Clean up dynamically allocated players
    for (auto *player : players)
    {
        delete player;
    }
    players.clear();
}

void GameEngine::clear()
{
    if (states_)
    {
        for (auto *s : *states_)
            delete s;
        states_->clear();
    }
    current_ = nullptr;
}

State *GameEngine::findState(const string &name) const
{
    for (auto *s : *states_)
        if (s->getName() == name)
            return s;
    return nullptr;
}

void GameEngine::buildGraph()
{
    clear();
    const char *names[] = {
        "start", "map_loaded", "map_validated", "players_added",
        "assign_reinforcement", "issue_orders", "execute_orders", "win"};

    for (const char *n : names)
        states_->push_back(new State(string(n)));

    auto S = [this](const string &n)
    { return findState(n); };

    S("start")->addTransition("loadmap", S("map_loaded"));
    S("map_loaded")->addTransition("loadmap", S("map_loaded"));
    S("map_loaded")->addTransition("validatemap", S("map_validated"));
    S("map_validated")->addTransition("addplayer", S("players_added"));
    S("players_added")->addTransition("addplayer", S("players_added"));
    S("players_added")->addTransition("assigncountries", S("assign_reinforcement"));
    S("assign_reinforcement")->addTransition("issueorder", S("issue_orders"));
    S("issue_orders")->addTransition("issueorder", S("issue_orders"));
    S("issue_orders")->addTransition("endissueorders", S("execute_orders"));
    S("execute_orders")->addTransition("execorder", S("execute_orders"));
    S("execute_orders")->addTransition("endexecorders", S("assign_reinforcement"));
    S("execute_orders")->addTransition("play", S("issue_orders"));
    S("execute_orders")->addTransition("win", S("win"));
    S("win")->addTransition("play", S("assign_reinforcement"));

    current_ = S("start");
}

bool GameEngine::applyCommand(const string &cmd)
{
    if (!current_)
        return false;
    State *next = current_->nextState(cmd);
    if (!next)
    {
        cout << "Invalid command from state '" << current_->getName() << "'\n";
        return false;
    }
    cout << "Transition: " << current_->getName()
         << " -- " << cmd << " ==> " << next->getName() << "\n";
    current_ = next;
    return true;
}

const State *GameEngine::current() const { return current_; }

ostream &operator<<(ostream &os, const GameEngine &ge)
{
    os << "GameEngine {current = "
       << (ge.current_ ? ge.current_->getName() : "<null>") << "}";
    return os;
}

GameEngine::GameEngine(const GameEngine &other)
    : current_(nullptr), states_(new vector<State *>)
{
    for (auto *st : *other.states_)
        states_->push_back(new State(st->getName()));

    auto findLocal = [&](const string &nm) -> State *
    {
        return this->findState(nm);
    };

    for (auto *stOther : *other.states_)
    {
        State *local = findLocal(stOther->getName());
        for (const auto &kv : stOther->getTransitions())
        {
            State *localNext = findLocal(kv.second->getName());
            local->addTransition(kv.first, localNext);
        }
    }

    if (other.current_)
        current_ = findLocal(other.current_->getName());

    // Deep copy players
    for (auto *player : other.players)
    {
        players.push_back(new Player(*player));
    }
}

GameEngine &GameEngine::operator=(const GameEngine &other)
{
    if (this != &other)
    {
        GameEngine tmp(other);
        swap(current_, tmp.current_);
        swap(states_, tmp.states_);
    }
    return *this;
}

// ---------- STARTUP PHASE ----------
void GameEngine::startupPhase()
{
    string input, command, argument;
    bool mapLoaded = false;
    bool mapValidated = false;
    // Remove local players vector, use member variable instead

    MapLoader map_loader;
    Map *map = nullptr;
    Deck gameDeck; // Create deck for the game
    cout << "====================================" << endl;
    cout << "GAME STARTUP PHASE" << endl;
    cout << "====================================" << endl;
    while (true)
    {
        cout << "Enter your command: ";
        getline(cin, input);

        // Parse command and argument properly
        istringstream iss(input);
        iss >> command;

        // Get the rest of the line as argument (handles spaces in names)
        argument.clear();
        if (iss >> ws)
        { // Skip whitespace
            getline(iss, argument);
        }

        if (command == "loadmap")
        {
            if (argument.empty())
            {
                cout << "Usage: loadmap <filename>\n";
                continue;
            }
            // Clean up old map if exists
            if (map != nullptr)
            {
                delete map;
            }

            map = map_loader.loadMap(argument); // Load map file
            if (map == nullptr)
            {
                cout << "Error: Failed to load map.\n";
                cout << argument << endl;
                continue;
            }

            cout << "Map '" << argument << "' loaded successfully.\n";
            mapLoaded = true;
            applyCommand("loadmap");
        }

        else if (command == "validatemap")
        {
            if (!mapLoaded || map == nullptr)
            {
                cout << "You must load a map first.\n";
                continue;
            }

            bool isMapValidated = map->validate();
            if (isMapValidated)
            {
                cout << "Map validated successfully.\n";
                mapValidated = true;
                applyCommand("validatemap");
            }
            else
            {
                cout << "Map validation failed. Please check the map file.\n";
            }
        }

        else if (command == "addplayer")
        {
            if (!mapLoaded)
            {
                cout << "You must load a map first.\n";
                continue;
            }
            if (!mapValidated)
            {
                cout << "You must validate the map.\n";
                continue;
            }
            if (argument.empty())
            {
                cout << "Usage: addplayer <playername>\n";
                continue;
            }

            // Check if player name already exists
            auto it = find_if(players.begin(), players.end(),
                              [&argument](const Player *p)
                              { return p->getPlayerName() == argument; });

            if (it != players.end())
            {
                cout << "Player '" << argument << "' already added.\n";
                continue;
            }
            if (players.size() > 6)
            {
                cout << "Ease up lil bro, you can only add 2-6 Players." << endl;
                continue;
            }
            // Create new player and add to list
            Player *newPlayer = new Player(argument);
            players.push_back(newPlayer);
            cout << "Player '" << newPlayer->getPlayerName() << "' added.\n";
            applyCommand("addplayer");
        }
        else if (command == "gamestart")
        {
            if (players.size() < 2)
            {
                cout << "You need between 2 and 6 players before starting.\n";
                continue;
            }

            cout << "\n=== Starting the game with " << players.size() << " players ===" << endl;
            for (const auto *p : players)
                cout << "  - " << p->getPlayerName() << "\n";

            // 4a. Distribute territories fairly among players
            cout << "\n4a) Distributing territories equally among players...\n";
            map->distributeTerritories(players);

            // 4b. Determine random order of play
            cout << "\n4b) Determining random order of play...\n";
            random_device rd;
            mt19937 g(rd());
            shuffle(players.begin(), players.end(), g);
            cout << "Order of play:\n";
            for (size_t i = 0; i < players.size(); ++i)
                cout << "  " << (i + 1) << ". " << players[i]->getPlayerName() << "\n";

            // 4c. Give 50 initial army units to each player's reinforcement pool
            cout << "\n4c) Assigning 50 army units to each player's reinforcement pool...\n";
            for (auto *p : players)
            {
                p->setReinforcementPool(50);
                cout << "  " << p->getPlayerName() << " receives 50 armies.\n";
            }

            // 4d. Let each player draw 2 initial cards from the deck
            cout << "\n4d) Each player draws 2 initial cards from the deck...\n";
            for (auto *p : players)
            {
                gameDeck.draw(*p, *(p->getHandOfCards()));
                gameDeck.draw(*p, *(p->getHandOfCards()));
                cout << "  " << p->getPlayerName() << " drew 2 cards.\n";
            }

            // 4e. Switch to play phase
            cout << "\n4e) Switching to play phase!\n";
            applyCommand("assigncountries");
            cout << "Transitioned to assign_reinforcement state.\n";
            cout << "Play phase started! (Next valid command: 'issueorder')\n";
            break;
        }

        else
        {
            cout << "Invalid command. Try: loadmap, validatemap, addplayer, gamestart\n";
        }
    }

    // Cleanup map (players are managed by GameEngine member variable)
    if (map != nullptr)
    {
        delete map;
    }
}
