#include "GameEngine.h"
#include <utility>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "Map.h"
#include "Player.h"
using namespace std;

// ---------- STATE CLASS ----------
State::State(const string& name)
    : name_(new string(name)), transitions_(new map<string, State*>) {}

State::State(const State& other)
    : name_(new string(*other.name_)), transitions_(new map<string, State*>) {}

State& State::operator=(const State& other) {
    if (this != &other) {
        *name_ = *other.name_;
        transitions_->clear();
    }
    return *this;
}

State::~State() {
    delete name_;
    delete transitions_;
}

void State::addTransition(const string& cmd, State* next) {
    (*transitions_)[cmd] = next;
}

State* State::nextState(const string& cmd) const {
    auto it = transitions_->find(cmd);
    return (it == transitions_->end()) ? nullptr : it->second;
}

const string& State::getName() const { return *name_; }
const map<string, State*>& State::getTransitions() const { return *transitions_; }

ostream& operator<<(ostream& os, const State& s) {
    os << "State(" << *s.name_ << ")";
    return os;
}

// ---------- GAME ENGINE ----------
GameEngine::GameEngine()
    : current_(nullptr), states_(new vector<State*>) {}

GameEngine::~GameEngine() {
    clear();
    delete states_;
}

void GameEngine::clear() {
    if (states_) {
        for (auto* s : *states_) delete s;
        states_->clear();
    }
    current_ = nullptr;
}

State* GameEngine::findState(const string& name) const {
    for (auto* s : *states_)
        if (s->getName() == name)
            return s;
    return nullptr;
}

void GameEngine::buildGraph() {
    clear();
    const char* names[] = {
        "start", "map_loaded", "map_validated", "players_added",
        "assign_reinforcement", "issue_orders", "execute_orders", "win"
    };

    for (const char* n : names)
        states_->push_back(new State(string(n)));

    auto S = [this](const string& n) { return findState(n); };

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

bool GameEngine::applyCommand(const string& cmd) {
    if (!current_) return false;
    State* next = current_->nextState(cmd);
    if (!next) {
        cout << "Invalid command from state '" << current_->getName() << "'\n";
        return false;
    }
    cout << "Transition: " << current_->getName()
         << " -- " << cmd << " ==> " << next->getName() << "\n";
    current_ = next;
    return true;
}

const State* GameEngine::current() const { return current_; }

ostream& operator<<(ostream& os, const GameEngine& ge) {
    os << "GameEngine {current = "
       << (ge.current_ ? ge.current_->getName() : "<null>") << "}";
    return os;
}

GameEngine::GameEngine(const GameEngine& other)
    : current_(nullptr), states_(new vector<State*>) {
    for (auto* st : *other.states_)
        states_->push_back(new State(st->getName()));

    auto findLocal = [&](const string& nm)->State* {
        return this->findState(nm);
    };

    for (auto* stOther : *other.states_) {
        State* local = findLocal(stOther->getName());
        for (const auto& kv : stOther->getTransitions()) {
            State* localNext = findLocal(kv.second->getName());
            local->addTransition(kv.first, localNext);
        }
    }

    if (other.current_)
        current_ = findLocal(other.current_->getName());
}

GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        GameEngine tmp(other);
        swap(current_, tmp.current_);
        swap(states_, tmp.states_);
    }
    return *this;
}

// ---------- STARTUP PHASE ----------
void GameEngine::startupPhase() {
    cout << "\n=== Game Startup Phase ===" << endl;

    string input, command, argument;
    bool mapLoaded = false;
    bool mapValidated = false;
    vector<string> players;

    while (true) {
        cout << "> ";
        getline(cin, input);
        istringstream iss(input);
        iss >> command >> argument;

        if (command == "loadmap") {
            if (argument.empty()) {
                cout << "Usage: loadmap <filename>\n";
                continue;
            }
            string path = "Maps/" + argument;
            ifstream file(path);
            if (!file) {
                cout << "Error: map file not found.\n";
                continue;
            }
            cout << "Map '" << argument << "' loaded successfully.\n";
            mapLoaded = true;
            applyCommand("loadmap");
        }

        else if (command == "validatemap") {
            if (!mapLoaded) {
                cout << "You must load a map first.\n";
                continue;
            }
            cout << "Map validated successfully.\n";
            mapValidated = true;
            applyCommand("validatemap");
        }

        else if (command == "addplayer") {
            if (!mapValidated) {
                cout << "You must validate the map first.\n";
                continue;
            }
            if (argument.empty()) {
                cout << "Usage: addplayer <name>\n";
                continue;
            }
            if (find(players.begin(), players.end(), argument) != players.end()) {
                cout << "Player already added.\n";
                continue;
            }
            players.push_back(argument);
            cout << "Player '" << argument << "' added.\n";
            applyCommand("addplayer");
        }

        else if (command == "gamestart") {
            if (players.size() < 2 || players.size() > 6) {
                cout << "You need between 2 and 6 players before starting.\n";
                continue;
            }

            cout << "Starting the game with " << players.size() << " players:\n";
            for (const auto& p : players)
                cout << "  - " << p << "\n";

            cout << "Distributing territories equally among players...\n";

            random_shuffle(players.begin(), players.end());
            cout << "Random order of play:\n";
            for (size_t i = 0; i < players.size(); ++i)
                cout << "  " << (i + 1) << ". " << players[i] << "\n";

            cout << "Assigning 50 army units to each player...\n";
            for (const auto& p : players)
                cout << "  " << p << " receives 50 armies.\n";

            cout << "Each player draws 2 initial cards from the deck.\n";

            // 4e. Switch to play phase
            cout << "Switching to play phase!\n";
            applyCommand("assigncountries");
            cout << "Transitioned to assign_reinforcement state.\n";
             cout << "Play phase started! (Next valid command: 'issueorder')\n";
                 break;
        }

        else {
            cout << "Invalid command. Try: loadmap, validatemap, addplayer, gamestart\n";
        }
    }
}
