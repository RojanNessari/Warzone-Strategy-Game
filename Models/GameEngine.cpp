#include "GameEngine.h"
#include "Player.h"
#include <utility>
using namespace std;

//A State has a name and a set of outgoing transitions (command -> next State)
State::State(const std::string& name)
    : name_(new string(name)), transitions_(new map<string, State*>) {}

State::State(const State& other)
    : name_(new string(*other.name_)), transitions_(new map<string, State*>) {
    // Leave transitions_ empty: edges depend on other State instances and are re-wired by GameEngine.
}

State& State::operator=(const State& other) {
    if (this != &other) {
        *name_ = *other.name_;
        transitions_->clear(); //Remove existing transitions. They are reconnected by GameEngine when the graph is cloned.

    }
    return *this;
}

State::~State() { //Cleanup owned resources to avoid memory leaks
    delete name_;
    delete transitions_;
}

void State::addTransition(const string& cmd, State* next) {
    (*transitions_)[cmd] = next; //'cmd' from this state goes to 'next'
}

State* State::nextState(const string& cmd) const { //Look up the next state for a given command; nullptr means "invalid command from here".
    auto it = transitions_->find(cmd);
    return (it == transitions_->end()) ? nullptr : it->second;
}

const string& State::getName() const { return *name_; }//read-only access to the state's nam

const map<string, State*>& State::getTransitions() const { //Expose transitions read-only so GameEngine can clone/inspect them.
    return *transitions_;
}

// normalize order of IDs so {1,2} == {2,1}
static inline pair<int,int> normPair(Player* a, Player* b) {
    int x = a->getId(), y = b->getId();
    if (x > y) swap(x, y);
    return {x, y};
}

// record that two players have a truce for the rest of the turn
void GameEngine::addTruce(Player* a, Player* b) {
    truces.insert(normPair(a, b));
}

// check if two players currently have a truce
bool GameEngine::isTruced(Player* a, Player* b) const {
    auto p = (a->getId() < b->getId())
        ? pair<int,int>{a->getId(), b->getId()}
        : pair<int,int>{b->getId(), a->getId()};
    return truces.find(p) != truces.end();
}

// clear truces (call at the start of each new turn)
void GameEngine::clearTrucesForNewTurn() {
    truces.clear();
}


Player* GameEngine::getNeutralPlayer() {
  if (!neutralPlayer) {
    neutralPlayer = new Player();
    neutralPlayer->setId(-1);  }
  return neutralPlayer;
}

ostream& operator<<(ostream& os, const State& s) {
    os << "State(" << *s.name_ << ")"; //print a state as: State(name)
    return os;
}

//GameEngine owns all State nodes, builds the graph, and moves a 'current' pointer across states.
GameEngine::GameEngine()
    : current_(nullptr), states_(new vector<State*>) {}//starts with no states and no current

GameEngine::~GameEngine() {//Make sure all heap-allocated states are deleted, then delete the container itself
    clear();
    delete states_;
    states_ = nullptr;
}

void GameEngine::clear() { //Delete all State* we own and reset current_
    if (states_) {
        for (auto* s : *states_) delete s;
        states_->clear();
    }
    current_ = nullptr;
}

State* GameEngine::findState(const string& name) const {
    //Linear search by name
    for (auto* s : *states_) if (s->getName() == name) return s;
    return nullptr;
}

void GameEngine::buildGraph() {
    //Recreate the entire state graph matching the assignment diagram.
    clear();

    const char* names[] = {
        "start",
        "map_loaded",
        "map_validated",
        "players_added",
        "assign_reinforcement",
        "issue_orders",
        "execute_orders",
        "win"
    };

    
    // Create all State nodes and store their pointers so we can connect them and later delete them.
    for (const char* n : names) {
        State* s = new State(std::string(n)); 
        states_->push_back(s);
    }

    //find a state by name quickly while wiring transitions
    auto S = [this](const std::string& n) { 
        return findState(n); 
    };

    // Transitions as shown on the assignment diagram (last graph)
    //Startup phase
      S("start")->addTransition("loadmap", S("map_loaded"));

    S("map_loaded")->addTransition("loadmap", S("map_loaded"));
    S("map_loaded")->addTransition("validatemap", S("map_validated"));

    S("map_validated")->addTransition("addplayer", S("players_added"));

    S("players_added")->addTransition("addplayer", S("players_added"));
    S("players_added")->addTransition("assigncountries", S("assign_reinforcement"));

    // play phase
    S("assign_reinforcement")->addTransition("issueorder", S("issue_orders"));

    S("issue_orders")->addTransition("issueorder", S("issue_orders"));
    S("issue_orders")->addTransition("endissueorders", S("execute_orders"));

    S("execute_orders")->addTransition("execorder", S("execute_orders"));
    S("execute_orders")->addTransition("endexecorders", S("assign_reinforcement"));
    S("execute_orders")->addTransition("play", S("issue_orders"));
    S("execute_orders")->addTransition("win", S("win"));

    // from win: only "play" goes back into the loop
    S("win")->addTransition("play", S("assign_reinforcement"));

    current_ = S("start");
}

bool GameEngine::applyCommand(const string& cmd) {
    // Try to move from current_ via command 'cmd'. If no edge, print error
    if (!current_) return false;
    State* next = current_->nextState(cmd);
    if (!next) {
        cout << "Invalid command from state, you are currently in  " << current_->getName();
        return false;
    }
    cout << "Transition: " << current_->getName()
         << " -- " << cmd << " ==> " << next->getName() << "\n";
    current_ = next;
    return true;
}

const State* GameEngine::current() const { return current_; }

ostream& operator<<(ostream& os, const GameEngine& ge) {
    os << "GameEngine {current = " << (ge.current_ ? ge.current_->getName() : "<null>") << "}"; //Print as: GameEngine {current = <state_name>}
    return os;
}

//Deep copy semantics: copying a GameEngine should produce an independent graph (no shared nodes)
GameEngine::GameEngine(const GameEngine& other)
    : current_(nullptr), states_(new vector<State*>) {

    //clone states by name
    for (auto* st : *other.states_) {
        states_->push_back(new State(st->getName()));
    }
    auto findLocal = [&](const string& nm)->State* {
        return this->findState(nm);
    };

    //rebuild transitions to local clones
    //for each edge (cmd -> next) in 'other', connect the corresponding local nodes
    for (auto* stOther : *other.states_) {
        State* local = findLocal(stOther->getName());
        for (const auto& kv : stOther->getTransitions()) {
            const string& cmd = kv.first;
            State* otherNext = kv.second;
            State* localNext = findLocal(otherNext->getName());
            local->addTransition(cmd, localNext);
        }
    }

    // 3) set current
    if (other.current_) current_ = findLocal(other.current_->getName());
}

GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        //Old data is cleaned up by temp's destructor
        GameEngine tmp(other);               // copy-ctor then swap
        std::swap(current_, tmp.current_);
        std::swap(states_,  tmp.states_);
    }
    return *this;
}
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
#include "../utils/logger.h"
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
        logMessage(ERROR, string("Invalid command from state '") + current_->getName() + "'");
        return false;
    }
    logMessage(INFO, string("Transition: ") + current_->getName() + " -- " + cmd + " ==> " + next->getName());
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
    logMessage(INFO, "====================================");
    logMessage(INFO, "GAME STARTUP PHASE");
    logMessage(INFO, "====================================");
    while (true)
    {
        logMessage(INFO, "Enter your command:");
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
                logMessage(ERROR, "Usage: loadmap <filename>");
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
                logMessage(ERROR, "Error: Failed to load map.");
                logMessage(DEBUG, argument);
                continue;
            }

            logMessage(INFO, string("Map '") + argument + "' loaded successfully.");
            mapLoaded = true;
            applyCommand("loadmap");
        }

        else if (command == "validatemap")
        {
            if (!mapLoaded || map == nullptr)
            {
                logMessage(ERROR, "You must load a map first.");
                continue;
            }

            bool isMapValidated = map->validate();
            if (isMapValidated)
            {
                logMessage(INFO, "Map validated successfully.");
                mapValidated = true;
                applyCommand("validatemap");
            }
            else
            {
                logMessage(ERROR, "Map validation failed. Please check the map file.");
            }
        }

        else if (command == "addplayer")
        {
            if (!mapLoaded)
            {
                logMessage(ERROR, "You must load a map first.");
                continue;
            }
            if (!mapValidated)
            {
                logMessage(ERROR, "You must validate the map.");
                continue;
            }
            if (argument.empty())
            {
                logMessage(ERROR, "Usage: addplayer <playername>");
                continue;
            }

            // Check if player name already exists
            auto it = find_if(players.begin(), players.end(),
                              [&argument](const Player *p)
                              { return p->getPlayerName() == argument; });

            if (it != players.end())
            {
                logMessage(ERROR, string("Player '") + argument + "' already added.");
                continue;
            }
            if (players.size() > 6)
            {
                logMessage(ERROR, "Ease up lil bro, you can only add 2-6 Players.");
                continue;
            }
            // Create new player and add to list
            Player *newPlayer = new Player(argument);
            players.push_back(newPlayer);
            logMessage(INFO, string("Player '") + newPlayer->getPlayerName() + "' added.");
            applyCommand("addplayer");
        }
        else if (command == "gamestart")
        {
            if (players.size() < 2)
            {
                logMessage(ERROR, "You need between 2 and 6 players before starting.");
                continue;
            }
            logMessage(INFO, string("=== Starting the game with ") + to_string(players.size()) + " players ===");
            for (const auto *p : players)
                logMessage(INFO, string("  - ") + p->getPlayerName());

            // 4a. Distribute territories fairly among players
            logMessage(INFO, "4a) Distributing territories equally among players...");
            map->distributeTerritories(players);

            // 4b. Determine random order of play
            logMessage(INFO, "4b) Determining random order of play...");
            random_device rd;
            mt19937 g(rd());
            shuffle(players.begin(), players.end(), g);
            logMessage(INFO, "Order of play:");
            for (size_t i = 0; i < players.size(); ++i)
                logMessage(INFO, string("  ") + to_string(i + 1) + ". " + players[i]->getPlayerName());

            // 4c. Give 50 initial army units to each player's reinforcement pool
            logMessage(INFO, "4c) Assigning 50 army units to each player's reinforcement pool...");
            for (auto *p : players)
            {
                p->setReinforcementPool(50);
                logMessage(INFO, string("  ") + p->getPlayerName() + " receives 50 armies.");
            }

            // 4d. Let each player draw 2 initial cards from the deck
            logMessage(INFO, "4d) Each player draws 2 initial cards from the deck...");
            for (auto *p : players)
            {
                gameDeck.draw(*p, *(p->getHandOfCards()));
                gameDeck.draw(*p, *(p->getHandOfCards()));
                logMessage(INFO, string("  ") + p->getPlayerName() + " drew 2 cards.");
            }

            // 4e. Switch to play phase
            logMessage(INFO, "4e) Switching to play phase!");
            applyCommand("assigncountries");
            logMessage(INFO, "Transitioned to assign_reinforcement state.");
            logMessage(INFO, "Play phase started! (Next valid command: 'issueorder')");
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
