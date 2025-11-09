#include "GameEngine.h"
#include <utility>
using namespace std;

// A State has a name and a set of outgoing transitions (command -> next State)
State::State(const std::string &name)
    : name_(new string(name)), transitions_(new map<string, State *>) {}

State::State(const State &other)
    : name_(new string(*other.name_)), transitions_(new map<string, State *>)
{
    // Leave transitions_ empty: edges depend on other State instances and are re-wired by GameEngine.
}

State &State::operator=(const State &other)
{
    if (this != &other)
    {
        *name_ = *other.name_;
        transitions_->clear(); // Remove existing transitions. They are reconnected by GameEngine when the graph is cloned.
    }
    return *this;
}

State::~State()
{ // Cleanup owned resources to avoid memory leaks
    delete name_;
    delete transitions_;
}

void State::addTransition(const string &cmd, State *next)
{
    (*transitions_)[cmd] = next; //'cmd' from this state goes to 'next'
}

State *State::nextState(const string &cmd) const
{ // Look up the next state for a given command; nullptr means "invalid command from here".
    auto it = transitions_->find(cmd);
    return (it == transitions_->end()) ? nullptr : it->second;
}

const string &State::getName() const { return *name_; } // read-only access to the state's nam

const map<string, State *> &State::getTransitions() const
{ // Expose transitions read-only so GameEngine can clone/inspect them.
    return *transitions_;
}

ostream &operator<<(ostream &os, const State &s)
{
    os << "State(" << *s.name_ << ")"; // print a state as: State(name)
    return os;
}

// GameEngine owns all State nodes, builds the graph, and moves a 'current' pointer across states.
GameEngine::GameEngine()
    : current_(nullptr), states_(new vector<State *>) {} // starts with no states and no current

GameEngine::~GameEngine()
{ // Make sure all heap-allocated states are deleted, then delete the container itself
    clear();
    delete states_;
    states_ = nullptr;
}

void GameEngine::clear()
{ // Delete all State* we own and reset current_
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
    // Linear search by name
    for (auto *s : *states_)
        if (s->getName() == name)
            return s;
    return nullptr;
}

void GameEngine::buildGraph()
{
    // Recreate the entire state graph matching the assignment diagram.
    clear();

    const char *names[] = {
        "start",
        "map_loaded",
        "map_validated",
        "players_added",
        "assign_reinforcement",
        "issue_orders",
        "execute_orders",
        "win"};

    // Create all State nodes and store their pointers so we can connect them and later delete them.
    for (const char *n : names)
    {
        State *s = new State(std::string(n));
        states_->push_back(s);
    }

    // find a state by name quickly while wiring transitions
    auto S = [this](const std::string &n)
    {
        return findState(n);
    };

    // Transitions as shown on the assignment diagram (last graph)
    // Startup phase
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

bool GameEngine::applyCommand(const string &cmd)
{
    // Try to move from current_ via command 'cmd'. If no edge, print error
    if (!current_)
        return false;
    State *next = current_->nextState(cmd);
    if (!next)
    {
        cout << "Invalid command from state, you are currently in  " << current_->getName() << endl;

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
    os << "GameEngine {current = " << (ge.current_ ? ge.current_->getName() : "<null>") << "}"; // Print as: GameEngine {current = <state_name>}
    return os;
}

// Deep copy semantics: copying a GameEngine should produce an independent graph (no shared nodes)
GameEngine::GameEngine(const GameEngine &other)
    : current_(nullptr), states_(new vector<State *>)
{

    // clone states by name
    for (auto *st : *other.states_)
    {
        states_->push_back(new State(st->getName()));
    }
    auto findLocal = [&](const string &nm) -> State *
    {
        return this->findState(nm);
    };

    // rebuild transitions to local clones
    // for each edge (cmd -> next) in 'other', connect the corresponding local nodes
    for (auto *stOther : *other.states_)
    {
        State *local = findLocal(stOther->getName());
        for (const auto &kv : stOther->getTransitions())
        {
            const string &cmd = kv.first;
            State *otherNext = kv.second;
            State *localNext = findLocal(otherNext->getName());
            local->addTransition(cmd, localNext);
        }
    }

    // 3) set current
    if (other.current_)
        current_ = findLocal(other.current_->getName());
}

GameEngine &GameEngine::operator=(const GameEngine &other)
{
    if (this != &other)
    {
        // Old data is cleaned up by temp's destructor
        GameEngine tmp(other); // copy-ctor then swap
        std::swap(current_, tmp.current_);
        std::swap(states_, tmp.states_);
    }
    return *this;
}
