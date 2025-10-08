#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>

class State {
public:
    //Construct a state with a given name
    State(const std::string& name);
    //Copy construct
    State(const State& other);
    State& operator=(const State& other);
    ~State();//Destructor

    void addTransition(const std::string& command, State* next); //Add an outgoing edge named 'command' to 'next'.
    State* nextState(const std::string& command) const;//Return the next State for 'command', or nullptr if invalid from this state
    const std::string& getName() const;

    // read-only access so GameEngine can deep-copy transitions
    const std::map<std::string, State*>& getTransitions() const;

    friend std::ostream& operator<<(std::ostream& os, const State& s);

private:
    std::string* name_;                          // pointer
    std::map<std::string, State*>* transitions_; // pointer 
};

class GameEngine {
public:
    GameEngine();
    //Deep copy: clones nodes and re-wires edges among the clones
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);
    ~GameEngine();

    void buildGraph();                      // create states and connect transitions
    bool applyCommand(const std::string&);  // validate + transition
    const State* current() const;

    friend std::ostream& operator<<(std::ostream& os, const GameEngine& ge);

private:
    State* current_;                // pointer
    std::vector<State*>* states_;   // owns all states

    State* findState(const std::string& name) const;
    void clear();                   // delete all allocated states
};
