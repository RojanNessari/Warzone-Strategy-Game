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
const string NEUTRAL_NAME = "NEUTRAL_NAME";

// =====================================================
//                    STATE CLASS
// =====================================================

State::State(const string &name)
    : name_(new string(name)), transitions_(new map<string, State *>)
{
    lastLogMessage = "State created: " + name;
    Notify(this, "INFO");
}

State::State(const State &other)
    : name_(new string(*other.name_)),
      transitions_(new map<string, State *>(*other.transitions_))
{
    lastLogMessage = "State copy-constructed: " + *name_;
    Notify(this, "DEBUG");
}
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
    lastLogMessage = "State destroyed: " + *name_;
    Notify(this, "INFO");
    delete name_;
    delete transitions_;
}

void State::addTransition(const string &cmd, State *next)
{
    if (next == nullptr) {
        
        return;
    }
    (*transitions_)[cmd] = next;
}

State *State::nextState(const string &cmd) const
{
    auto it = transitions_->find(cmd);
    return (it == transitions_->end()) ? nullptr : it->second;
}

const string &State::getName() const {
    // Return a safe string if name_ was not initialized for some reason.
    static const string unnamed = "<UNNAMED_STATE>";
    return (name_ != nullptr) ? *name_ : unnamed;
}
std::string State::stringToLog()const
{
    return lastLogMessage;
}
const map<string, State *> &State::getTransitions() const
{
    return *transitions_;
}

ostream &operator<<(ostream &os, const State &s)
{
    // Use getName() (safe) instead of dereferencing name_ directly.
    os << "State(" << s.getName() << ")";
    return os;
}

// =====================================================
//                    GAME ENGINE
// =====================================================
GameEngine::GameEngine()
    : current_(nullptr), states_(new vector<State *>) {    lastLogMessage = "GameEngine constructed.";
    Notify(this, "INFO");}

GameEngine::~GameEngine()
{
    lastLogMessage = "GameEngine destructor called. Cleaning up.";
    Notify(this, "INFO");
    clear();
    delete states_;

    // Clean up dynamically allocated players
    for (auto *player : players)
    {
        delete player;
    }
    players.clear();

    // Clean up map and deck
    if (gameMap != nullptr)
    {
        delete gameMap;
        gameMap = nullptr;
    }

    if (gameDeck != nullptr)
    {
        delete gameDeck;
        gameDeck = nullptr;
    }
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
    lastLogMessage = "GameEngine::clear() — All states deleted.";
    Notify(this, "DEBUG");
}

State *GameEngine::findState(const string &name) const
{
    for (auto *s : *states_)
        if (s->getName() == name)
            return s;
    return nullptr;
}

// normalize order of IDs so {1,2} == {2,1}
static inline pair<int, int> normPair(Player *a, Player *b)
{
    int x = a->getId(), y = b->getId();
    if (x > y)
        swap(x, y);
    return {x, y};
}

// record that two players have a truce for the rest of the turn
void GameEngine::addTruce(Player *a, Player *b)
{
    truces.insert(normPair(a, b));
    lastLogMessage = "Truce added between Player " +
                     to_string(a->getId()) + " and Player " + to_string(b->getId());
    Notify(this, "INFO");
}

// check if two players currently have a truce
bool GameEngine::isTruced(Player *a, Player *b) const
{
    auto p = (a->getId() < b->getId())
                 ? pair<int, int>{a->getId(), b->getId()}
                 : pair<int, int>{b->getId(), a->getId()};
    return truces.find(p) != truces.end();
}

// clear truces (call at the start of each new turn)
void GameEngine::clearTrucesForNewTurn()
{
    lastLogMessage = "Truces cleared for new turn.";
    Notify(this, "INFO");
    truces.clear();
}

Player *GameEngine::getNeutralPlayer()
{
    if (!neutralPlayer)
    {
        neutralPlayer = new Player(NEUTRAL_NAME);
        neutralPlayer->setId(-1);
         lastLogMessage = "Neutral Player created.";
        Notify(this, "INFO");
    }
    return neutralPlayer;
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
    S("players_added")->addTransition("gamestart", S("assign_reinforcement"));
    S("assign_reinforcement")->addTransition("issueorder", S("issue_orders"));
    S("issue_orders")->addTransition("issueorder", S("issue_orders"));
    S("issue_orders")->addTransition("endissueorders", S("execute_orders"));
    S("execute_orders")->addTransition("execorder", S("execute_orders"));
    S("execute_orders")->addTransition("endexecorders", S("assign_reinforcement"));
    S("execute_orders")->addTransition("win", S("win"));
    S("win")->addTransition("replay", S("start"));
    S("win")->addTransition("quit", S("endGame"));

    current_ = S("start");
    
}

bool GameEngine::applyCommand(const string &cmd)
{
    if (!current_)
    {
          lastLogMessage = "applyCommand failed — current state is NULL.";
        Notify(this, "ERROR");
        return false;
    }
    State *next = current_->nextState(cmd);
    if (!next)
    {
         lastLogMessage = "Invalid command '" + cmd +
                         "' from state '" + current_->getName() + "'";
        Notify(this, "ERROR");
        return false;
    }
    lastLogMessage = "STATE TRANSITION: " + current_->getName() +
                     " --" + cmd + "--> " + next->getName();
    Notify(this, "PROGRESSION");
    current_ = next;
    
    return true;
}
std::string GameEngine::stringToLog()const
{
    return lastLogMessage;
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

// ----------- Main game loop methods -------

// ----------- REINFORCEMENT PHASE -----------
void GameEngine::reinforcementPhase()
{
    lastLogMessage = "=== REINFORCEMENT PHASE START ===";
    Notify(this, "PROGRESSION");

    for (Player *player : players)
    {
        if (player->getTerritories().empty())
            continue;

        // Calculate base reinforcements: territories /3 minimum 3
        int territoriesOwned = player->getTerritories().size();
        int armies = std::max(3, territoriesOwned / 3);
        lastLogMessage = player->getPlayerName() + " owns " +
                         std::to_string(territoriesOwned) + " territories.";
        Notify(this, "INVENTORY");

        lastLogMessage = "Base reinforcements: " + std::to_string(armies);
        Notify(this, "INVENTORY");

        // Add Continent Bonuses
        if (gameMap != nullptr)
        {
            for (Continent &continent : gameMap->getContinents())
            {
                bool ownsAll = true;
                std::vector<Territory *> continentTerritories = continent.getTerritories(gameMap);

                for (Territory *territory : continentTerritories)
                {
                    if (territory->getOwner() != player)
                    {
                        ownsAll = false;
                        break;
                    }
                }

                if (ownsAll && !continentTerritories.empty())
                {
                    int bonus = continent.getBonusValue();
                    armies += bonus;
                     lastLogMessage = player->getPlayerName() +
                                     " controls continent " + continent.getName() +
                                     " (+" + std::to_string(bonus) + " bonus armies).";
                    Notify(this, "INVENTORY");
                }
            }
        }
        player->setReinforcementPool(armies);
        lastLogMessage = player->getPlayerName() + " receives " +
                         std::to_string(armies) + " total armies.";
        Notify(this, "INVENTORY");
    }
    lastLogMessage = "=== REINFORCEMENT PHASE END ===";
    Notify(this, "PROGRESSION");
}

// ---------- ISSUING ORDERS PHASE ----------

void GameEngine::issueOrdersPhase()
{
  lastLogMessage = "=== ISSUE ORDERS PHASE START ===";
    Notify(this, "PROGRESSION");
    
    std::vector<bool> playersDone(players.size(), false);
    bool allDone = false;
    while (!allDone)
    {
        allDone = true;
        for (size_t i = 0; i < players.size(); i++)
        {
            if (playersDone[i])
                continue;
            Player *player = players[i];
            // skip players with no territories
            if (player->getTerritories().empty())
            {
                playersDone[i] = true;
                     lastLogMessage = player->getPlayerName() +
                                 " has no territories — skipping issuing orders.";
                Notify(this, "WARNING");
                continue;
            }
            lastLogMessage = player->getPlayerName() + "'s turn to issue orders.";
            Notify(this, "PROGRESSION");

            bool hasMore = player->issueOrder(gameMap);  // Pass the map
            lastLogMessage = player->getPlayerName() +
                             (hasMore ? " will issue more orders." : " has no more orders to issue.");
            Notify(this, hasMore ? "PROGRESSION" : "INFO");
            if (!hasMore)
            {
                playersDone[i] = true;
               
            }
            else
            {
                allDone = false;
            }
        }
    }
    lastLogMessage = "=== ISSUE ORDERS PHASE END ===";
    Notify(this, "PROGRESSION");
}

// ---------- EXECUTE ORDERS PHASE ----------

void GameEngine::executeOrdersPhase()
{
    lastLogMessage = "=== EXECUTE ORDERS PHASE START ===";
    Notify(this, "PROGRESSION");

    for (Player *player : players)
    {
        player->resetConqueredFlag();
    }

    clearTrucesForNewTurn();

     lastLogMessage = "All players' conquered flags reset and truces cleared.";
    Notify(this, "DEBUG");

    lastLogMessage = "--- Executing Deploy Orders ---";
    Notify(this, "COMBAT");
    bool foundDeploy = true;

    while (foundDeploy)
    {
        foundDeploy = false;
        for (Player *player : players)
        {
            if (player->getTerritories().empty())
            {
                continue;
            }
            OrdersList *orderList = player->getOrdersList();
            // Find first deploy order:
            for (size_t i = 0; i < orderList->size(); i++)
            {
                Order *order = orderList->get(i);
                Deploy *deployOrder = dynamic_cast<Deploy *>(order);
                if (deployOrder != nullptr)
                {
                     lastLogMessage = "Executing " + player->getPlayerName() + "'s Deploy order.";
                    Notify(this, "COMBAT");
                    order->execute();
                    
                    lastLogMessage = "Deploy order effect: " + order->getEffect();
                    Notify(this, "COMBAT");

                    orderList->remove(i);
                    foundDeploy = true;
                    break;
                }
            }
        }
    }

     lastLogMessage = "--- Executing Non-Deploy Orders (Advance, Bomb, Airlift, etc.) ---";
    Notify(this, "COMBAT");
    bool foundOtherOrders = true;

    while (foundOtherOrders)
    {
        foundOtherOrders = false;
        for (Player *player : players)
        {
            if (player->getTerritories().empty())
            {
                continue;
            }
            OrdersList *orderList = player->getOrdersList();

            // Execute the first order in the list (which is not a Deploy since they're all gone)
            if (orderList->size() > 0)
            {
                Order *order = orderList->get(0);
                lastLogMessage = "Executing " + player->getPlayerName() + "'s order: ";
                Notify(this, "COMBAT");

                order->execute();

                lastLogMessage = "Order effect: " + order->getEffect();
                Notify(this, "COMBAT");

                orderList->remove(0);
                foundOtherOrders = true;
            }
        }
    }

    // Give cards to players who conquered at least one territory
    if (gameDeck != nullptr)
    {
        lastLogMessage = "--- Distributing cards to players who conquered territories ---";
        Notify(this, "INVENTORY");
        for (Player *player : players)
        {
            if (player->hasConqueredThisTurn())
            {
                Card *card = gameDeck->draw(*player, *(player->getHandOfCards()));
                if (card != nullptr)
                {
                    lastLogMessage = player->getPlayerName() +
                                     " conquered a territory and receives a card.";
                    Notify(this, "INVENTORY");
                }
                 else
                {
                    lastLogMessage = "No more cards available to draw for " + player->getPlayerName();
                    Notify(this, "WARNING");
                }
            }
        }
    }
     lastLogMessage = "=== EXECUTE ORDERS PHASE END ===";
    Notify(this, "PROGRESSION");
}

// ---------- MAIN GAME LOOP ----------
void GameEngine::mainGameLoop()
{
     lastLogMessage = "=== MAIN GAME LOOP START ===";
    Notify(this, "PROGRESSION");

    int turnNumber = 1;

    while (true)
    {
             lastLogMessage = "---------- TURN " + std::to_string(turnNumber) + " ----------";
        Notify(this, "PROGRESSION");
        // 1. Reinforcement Phase
        reinforcementPhase();

        // 2. Issue Orders Phase
        issueOrdersPhase();

        // 3. Execute Orders Phase
        executeOrdersPhase();

         lastLogMessage = "Checking for eliminated players...";
        Notify(this, "INFO");

        auto it = players.begin();
        while (it != players.end())
        {
            if ((*it)->getTerritories().empty())
            {
                lastLogMessage = (*it)->getPlayerName() +
                                 " has been eliminated (no territories).";
                Notify(this, "INFO");
                it = players.erase(it);
            }
            else
            {
                ++it;
            }
        }
        // 5. Check win Condition
        int playersWithTerritories = 0;
        Player *potentialPlayerWinner = nullptr;
        for (Player *player : players)
        {
            if (!player->getTerritories().empty())
            {
                playersWithTerritories++;
                potentialPlayerWinner = player;
            }
        }
        if (playersWithTerritories == 1)
        {
             lastLogMessage = "GAME OVER — " + potentialPlayerWinner->getPlayerName() + " WINS!";
            Notify(this, "PROGRESSION");
            applyCommand("win");
            break;
        }
        if (playersWithTerritories == 0)
        {
            lastLogMessage = "GAME OVER — No players remain with territories. Game ends in a draw.";
            Notify(this, "ERROR");
            break;
        }
        turnNumber++;

        if (turnNumber > 100)
        {
             lastLogMessage = "Turn limit reached (100). Ending game.";
            Notify(this, "WARNING");
            break;
        }
    }
     lastLogMessage = "=== MAIN GAME LOOP END ===";
    Notify(this, "PROGRESSION");
}

// ---------- STARTUP PHASE ----------
void GameEngine::startupPhase()
{
    string input, command, argument;
    bool mapLoaded = false;
    bool mapValidated = false;

    MapLoader map_loader;
     lastLogMessage = "=== GAME STARTUP PHASE BEGIN ===";
    Notify(this, "PROGRESSION");

    // Initialize deck if not already created
    if (gameDeck == nullptr)
    {
        gameDeck = new Deck();
    }

    while (true)
    {
         lastLogMessage = "Waiting for startup command (loadmap, validatemap, addplayer, gamestart).";
        Notify(this, "INPUT");
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
                lastLogMessage = "Invalid loadmap usage. Expected: loadmap <filename>";
                Notify(this, "ERROR");
                continue;
            }
            // Clean up old map if exists
            if (gameMap != nullptr)
            {
                delete gameMap;
                lastLogMessage = "Previous map deleted before loading new map.";
                Notify(this, "DEBUG");
            }

            gameMap = map_loader.loadMap(argument); // Load map file
            if (gameMap == nullptr)
            {
               lastLogMessage = "Failed to load map: " + argument;
                Notify(this, "ERROR");
                continue;
            }

             lastLogMessage = "Map '" + argument + "' loaded successfully.";
            Notify(this, "INFO");

            mapLoaded = true;
            applyCommand("loadmap");
        }

        else if (command == "validatemap")
        {
            if (!mapLoaded || gameMap == nullptr)
            {
                lastLogMessage = "Attempted validatemap without a loaded map.";
                Notify(this, "ERROR");
                continue;
            }

            bool isMapValidated = gameMap->validate();
            if (isMapValidated)
            {
                 lastLogMessage = "Map validated successfully.";
                Notify(this, "INFO");
                mapValidated = true;
                applyCommand("validatemap");
            }
            else
            {
               lastLogMessage = "Map validation failed.";
                Notify(this, "ERROR");
                cout << "Map validation failed. Please check the map file.\n";
            }
        }

        else if (command == "addplayer")
        {
            if (!mapLoaded)
            {
                lastLogMessage = "Attempted addplayer before loadmap.";
                Notify(this, "ERROR");
                cout << "You must load a map first.\n";
                continue;
            }
            if (!mapValidated)
            {
                lastLogMessage = "Attempted addplayer before validatemap.";
                Notify(this, "ERROR");
                cout << "You must validate the map first.\n";
                continue;
            }
            if (argument.empty())
            {
                lastLogMessage = "Invalid addplayer usage. Expected: addplayer <playername>";
                Notify(this, "ERROR");
                cout << "Usage: addplayer <playername>\n";
                continue;
            }

            // Check if player name already exists
            auto it = find_if(players.begin(), players.end(),
                              [&argument](const Player *p)
                              { return p->getPlayerName() == argument; });

            if (it != players.end())
            {
                 lastLogMessage = "Player '" + argument + "' already exists.";
                Notify(this, "ERROR");
                cout << "Player '" << argument << "' already added.\n";
                continue;
            }
            if (players.size() > 6)
            {
                lastLogMessage = "Attempted to add more than 6 players.";
                Notify(this, "ERROR");
                cout << "You can only have between 2 and 6 players.\n";
                continue;
            }
            Player *newPlayer = new Player(argument);
            players.push_back(newPlayer);

            lastLogMessage = "Player '" + newPlayer->getPlayerName() + "' added.";
            Notify(this, "INFO");

            applyCommand("addplayer");
        }
        else if (command == "gamestart")
        {
            if (players.size() < 2)
            {
                 lastLogMessage = "Invalid gamestart: players count = " + to_string(players.size());
                Notify(this, "ERROR");
                cout << "You need between 2 and 6 players before starting.\n";
                continue;
            }
             lastLogMessage = "Starting the game with " + to_string(players.size()) + " players.";
            Notify(this, "PROGRESSION");
            for (const auto *p : players)
            {
                lastLogMessage = "Player in game: " + p->getPlayerName();
                Notify(this, "INFO");
            }
            // 4a. Distribute territories fairly among players
             lastLogMessage = "Distributing territories among players...";
            Notify(this, "PROGRESSION");
            gameMap->distributeTerritories(players);

            // 4b) Random order of play
            lastLogMessage = "Determining random order of play...";
            Notify(this, "PROGRESSION");

            random_device rd;
            mt19937 g(rd());
            shuffle(players.begin(), players.end(), g);

             for (size_t i = 0; i < players.size(); ++i)
            {
                lastLogMessage = "Turn order " + to_string(i + 1) + ": " + players[i]->getPlayerName();
                Notify(this, "INFO");
            }

            // 4c) 50 initial armies each
            lastLogMessage = "Assigning 50 initial armies to each player.";
            Notify(this, "INVENTORY");

            for (auto *p : players)
            {
                p->setReinforcementPool(50);
                lastLogMessage = p->getPlayerName() + " receives 50 armies.";
                Notify(this, "INVENTORY");
            }

            // 4d. Let each player draw 2 initial cards from the deck
             lastLogMessage = "Each player draws 2 initial cards from the deck.";
            Notify(this, "INVENTORY");

            for (auto *p : players)
            {
                gameDeck->draw(*p, *(p->getHandOfCards()));
                gameDeck->draw(*p, *(p->getHandOfCards()));

                lastLogMessage = p->getPlayerName() + " drew 2 cards.";
                Notify(this, "INVENTORY");
            }

            // 4e. Switch to play phase
            lastLogMessage = "Switching to play phase. Next state: assign_reinforcement.";
            Notify(this, "PROGRESSION");

            applyCommand("gamestart");

            lastLogMessage = "Startup phase complete. Entering main game loop.";
            Notify(this, "PROGRESSION");

            break;
        }

          else
        {
            lastLogMessage = "Invalid startup command: '" + command + "'";
            Notify(this, "ERROR");
            cout << "Invalid command. Try: loadmap, validatemap, addplayer, gamestart.\n";
        }
    }

     lastLogMessage = "=== GAME STARTUP PHASE END ===";
    Notify(this, "PROGRESSION");
}
