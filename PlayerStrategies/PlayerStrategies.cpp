#include "PlayerStrategies.h"
#include "../Models/Player.h"
#include "../Models/Map.h"
#include "../Models/Orders.h"
#include "../Models/Cards.h"
#include "../utils/logger.h"
#include <algorithm>
#include <limits>

using namespace std;

enum class StrategyName : char
{
    HUMAN,
    AGGRESSIVE,
    BENEVOLENT,
    NEUTRAL,
    CHEATER
};

const char *toStringStrategy(StrategyName n)
{
    switch (n)
    {
    case StrategyName::HUMAN:
        return "Human";
    case StrategyName::AGGRESSIVE:
        return "Aggressive";
    case StrategyName::BENEVOLENT:
        return "Benevolent";
    case StrategyName::NEUTRAL:
        return "Neutral";
    case StrategyName::CHEATER:
        return "Cheater";
    default:
        return "Human";
    }
}

// ==================== HUMAN PLAYER STRATEGY ====================

HumanPlayerStrategy::HumanPlayerStrategy() {}
HumanPlayerStrategy::~HumanPlayerStrategy() {}

string HumanPlayerStrategy::getStrategyName() const
{
    return toStringStrategy(StrategyName::HUMAN);
}

bool HumanPlayerStrategy::issueOrder(Player *player, Map *map)
{
    lastLogMessage = player->getPlayerName() + "'s Turn (" + getStrategyName() + " Strategy)";
    Notify(this, "INFO");
    lastLogMessage = "Reinforcement pool: " + to_string(player->getReinforcementPool());
    Notify(this, "INVENTORY");


    cout << "\nWhat would you like to do?" << endl;
    cout << "1. Deploy armies" << endl;
    cout << "2. Advance armies" << endl;
    cout << "3. Play a card" << endl;
    cout << "4. Done issuing orders" << endl;
 
    try
    {
        int choice;
           lastLogMessage="user log the input"+ to_string(choice);
         Notify(this, "INPUT");
        if (!(cin >> choice))
        {
            lastLogMessage = "Invalid Input. Proceeding with option 4";
            Notify(this, "WARNING");
            
            throw runtime_error("Invalid Input. Proceeding with option number 4");
        }
        if (choice < 1 || choice > 4)
        {
            lastLogMessage = "You entered incorrect number. Proceeding with option 4";
            Notify(this, "WARNING");
            
            return false;
        }
        if (choice == 4)
        {
            lastLogMessage = player->getPlayerName() + " done issuing orders."; 
            Notify(this, "INFO");
            return false; // Done issuing orders
        }

        // Processing input phase:

        if (choice == 1)
        {
            vector<Territory *> territories = player->getTerritories();
            for (size_t i = 0; i < territories.size(); i++)
            {
                cout << i + 1 << ". " << territories[i]->getName()
                     << " (armies: " << territories[i]->getArmies() << ")" << endl;
            }
            lastLogMessage="user log the input for territory choice";
            Notify(this, "INPUT");
    
            int territoryChoice;
            cin >> territoryChoice;
            if (territoryChoice < 1 || territoryChoice > territories.size())
            {
                lastLogMessage = "Invalid choice";
                Notify(this, "WARNING");
                
                return true;
            }
            lastLogMessage="user log the input for armies to deploy";
            Notify(this, "INPUT");
            int armies;
            cin >> armies;
            lastLogMessage="user log the input for armies to deploy: "+ to_string(armies);
            Notify(this, "INPUT");
            if (armies > player->getReinforcementPool() || armies < 1)
            {
                lastLogMessage = "Invalid number of armies!";
                Notify(this, "WARNING");    
                
                return true;
            }

            Territory *target = territories[territoryChoice - 1];
            player->getOrdersList()->add(new Deploy(player, target, armies));
            player->takeFromReinforcement(armies);
            lastLogMessage = "Deploy order created!";
            Notify(this, "INFO");   
          
        }
        else if (choice == 2)
        {
            // Advance Order
            vector<Territory *> territories = player->getTerritories();
            lastLogMessage  = "Select source territory:";
            Notify(this, "INPUT");
          
            for (size_t i = 0; i < territories.size(); i++)
            {
                cout << i + 1 << ". " << territories[i]->getName()
                     << " (armies: " << territories[i]->getArmies() << ")" << endl;
            }
         
            int sourceChoice;
            cin >> sourceChoice;

            if (sourceChoice < 1 || sourceChoice > territories.size())
            {
                lastLogMessage  = "Invalid choice";         
                Notify(this, "WARNING");
              
                return true;
            }
            Territory *source = territories[sourceChoice - 1];
            vector<Territory *> neighbors = map->getNeighborsOf(source);
            lastLogMessage  = "Select target territory:";
            Notify(this, "INPUT");
            
            for (size_t i = 0; i < neighbors.size(); i++)
            {
                cout << i + 1 << ". " << neighbors[i]->getName()
                     << " (armies: " << neighbors[i]->getArmies() << ")" << endl;
            }

            
            int targetChoice;
            cin >> targetChoice;

            if (targetChoice < 1 || targetChoice > neighbors.size())
            {
                lastLogMessage = "Invalid choice";
                Notify(this, "WARNING");    
               
                return true;
            }

            int armies;
            lastLogMessage = "Enter number of armies to advance:"+ to_string(armies);
            Notify(this, "INPUT");
            cin >> armies;

            if (armies < 1 || armies > source->getArmies())
            {
                lastLogMessage ="Invalid number of armies!";
                Notify(this, "WARNING");
               
                return true;
            }

            Territory *target = neighbors[targetChoice - 1];
            player->getOrdersList()->add(new Advance(player, source, target, armies));
            lastLogMessage="Advance order created!";
            Notify(this, "INFO");   
           
        }
        else if (choice == 3)
        {
            // Play a card
            Hand *hand = player->getHandOfCards();
            lastLogMessage ="Your Cards:";
            Notify(this, "INFO");
           
            cout << *hand << endl;
        

            int cardIndex;
            cin >> cardIndex;

            if (cardIndex > 0 && cardIndex <= hand->size())
            {
                Card *card = hand->getCards()[cardIndex - 1];
                // card->play(player, map); Need to fix this

          lastLogMessage = "Played card at index " + to_string(cardIndex);
                Notify(this, "INFO");
            }
            else
            {
                lastLogMessage = "Invalid card index!";
                Notify(this, "WARNING");    
               
                return true;
            }
        }
        return true;
    }
    catch (const exception &e)
    {
        lastLogMessage = player->getPlayerName() + " encountered an error: " + e.what();    
        Notify(this, "ERROR");
      
        cin.clear();
        return false;
    }
}

vector<Territory *> HumanPlayerStrategy::toAttack(Player *player, Map *map) const
{
    vector<Territory *> attackList;
    vector<Territory *> ownedTerritories = player->getTerritories();
    // Get All adjacent enemy territories;
    for (Territory *territory : ownedTerritories)
    {
        vector<Territory *> neighbors = map->getNeighborsOf(territory);
        for (Territory *neighbor : neighbors)
        {
            if (!player->ownsTerritoryId(neighbor->getId()))
            {
                // check if already not in the list
                if (find(attackList.begin(), attackList.end(), neighbor) == attackList.end())
                {
                    attackList.push_back(neighbor);
                }
            }
        }
    }

    return attackList;
}

vector<Territory *> HumanPlayerStrategy::toDefend(Player *player) const
{
    return player->getTerritories();
}

// ==================== AGGRESSIVE PLAYER STRATEGY ====================

AggressivePlayerStrategy::AggressivePlayerStrategy() {};
AggressivePlayerStrategy::~AggressivePlayerStrategy() {};

string AggressivePlayerStrategy::getStrategyName() const
{
    return toStringStrategy(StrategyName::AGGRESSIVE);
}

Territory *AggressivePlayerStrategy::getStrongestTerritory(Player *player) const
{
    vector<Territory *> territories = player->getTerritories();
    if (territories.empty())
        return nullptr;

    Territory *strongest = territories[0];
    for (Territory *t : territories)
    {
        if (t->getArmies() > strongest->getArmies())
        {
            strongest = t;
        }
    }

    return strongest;
}

bool AggressivePlayerStrategy::issueOrder(Player *player, Map *map)
{
    lastLogMessage  = player->getPlayerName() + "'s Turn (" + getStrategyName() + " Strategy)";
    Notify(this, "INFO");
   
    // First, deploy all reinforcement to strongest territory
    if (player->getReinforcementPool() > 0)
    {
        Territory *strongest = getStrongestTerritory(player);
        if (strongest)
        {
            int armies = player->getReinforcementPool();
            player->getOrdersList()->add(new Deploy(player, strongest, armies));
            player->takeFromReinforcement(armies);
            lastLogMessage = "Deployed " + to_string(armies) + " armies to " + strongest->getName();
            Notify(this, "INFO");
            
            return true;
        }
    }

    // Then advance from strongest to enemy territories
    Territory *strongest = getStrongestTerritory(player);

    if (strongest && strongest->getArmies() > 1)
    {
        vector<Territory *> neighbors = map->getNeighborsOf(strongest);
        // Find an enemy neighbor
        for (Territory *neighbor : neighbors)
        {
            if (!player->ownsTerritoryId(neighbor->getId()))
            {
                int armiesToAdvance = strongest->getArmies() - 1;
                player->getOrdersList()->add(new Advance(player, strongest, neighbor, armiesToAdvance));
                
               
                return true;
            }
        }
    }

    return false; // Done issuing
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Player *player, Map *map) const
{
    vector<Territory *> attackList;
    Territory *strongest = getStrongestTerritory(player);
    if (strongest)
    {
        vector<Territory *> neighbors = map->getNeighborsOf(strongest);
        for (Territory *neighbor : neighbors)
        {
            if (!player->ownsTerritoryId(neighbor->getId()))
            {
                attackList.push_back(neighbor);
            }
        }
    }

    return attackList;
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Player *player) const
{
    vector<Territory *> defendList;
    Territory *strongest = getStrongestTerritory(player);
    if (strongest)
    {
        defendList.push_back(strongest);
    }
    return defendList;
}

// ==================== BENEVOLENT PLAYER STRATEGY ====================

BenevolentPlayerStrategy::BenevolentPlayerStrategy() {};
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {};

string BenevolentPlayerStrategy::getStrategyName() const
{
    return toStringStrategy(StrategyName::BENEVOLENT);
}

Territory *BenevolentPlayerStrategy::getWeakestTerritory(Player *player) const
{
    vector<Territory *> territories = player->getTerritories();
    if (territories.empty())
        return nullptr;

    Territory *weakest = territories[0];
    for (Territory *t : territories)
    {
        if (t->getArmies() < weakest->getArmies())
        {
            weakest = t;
        }
    }

    return weakest;
}

bool BenevolentPlayerStrategy::issueOrder(Player *player, Map *map)
{
    lastLogMessage  = player->getPlayerName() + "'s Turn (" + getStrategyName() + " Strategy)";
    Notify(this, "INFO");
    

    // Deploy all reinforcement to weak territory
    if (player->getReinforcementPool() > 0)
    {
        Territory *weakest = getWeakestTerritory(player);
        if (weakest)
        {
            int armies = player->getReinforcementPool();
            player->getOrdersList()->add(new Deploy(player, weakest, armies));
            player->takeFromReinforcement(armies);
            lastLogMessage ="Deployed " + to_string(armies) + " armies to weakest territory: " + weakest->getName();
            Notify(this, "INFO");
            
            return true;
        }
    }
    // Advance armies from stronger territories to weaker ones
    vector<Territory *> territories = player->getTerritories();
    if (territories.size() > 1)
    {
        Territory *weakest = getWeakestTerritory(player);
        // Find a neighboring friendly territory with more armies;
        for (Territory *owned : territories)
        {
            if (owned != weakest && owned->getArmies() > weakest->getArmies())
            {
                vector<Territory *> neighbors = map->getNeighborsOf(owned);
                for (Territory *neighbor : neighbors)
                {
                    if (neighbor == weakest)
                    {
                        int armiesToMove = (owned->getArmies() - weakest->getArmies()) / 2;
                        if (armiesToMove > 0)
                        {
                            player->getOrdersList()->add(new Advance(player, owned, weakest, armiesToMove));
                            lastLogMessage ="Moving " + to_string(armiesToMove) + " armies from " + owned->getName() + " to " + weakest->getName();
                            Notify(this, "INFO");
                         
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false; // Done issuing orders
}

vector<Territory *> BenevolentPlayerStrategy::toAttack(Player *player, Map *map) const
{
    // non-aggressive agent
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend(Player *player) const
{
    vector<Territory *> toDefentList = player->getTerritories();
    // sort by army count (weakest territory first)
    std::sort(toDefentList.begin(), toDefentList.end(),
              [](Territory *a, Territory *b)
              {
                  // return true if a should go before b
                  return a->getArmies() < b->getArmies();
              });

    return toDefentList;
}

// ==================== NEUTRAL PLAYER STRATEGY ====================
NeutralPlayerStrategy::NeutralPlayerStrategy() {};
NeutralPlayerStrategy::~NeutralPlayerStrategy() {};

string NeutralPlayerStrategy::getStrategyName() const
{
    return toStringStrategy(StrategyName::NEUTRAL);
}

bool NeutralPlayerStrategy::issueOrder(Player *player, Map *map)
{
    lastLogMessage  = player->getPlayerName() + "'s Turn (" + getStrategyName() + " Strategy)";
    Notify(this, "INFO");
    
    return false; // never does anything
}

vector<Territory *> NeutralPlayerStrategy::toAttack(Player *player, Map *map) const
{
    return vector<Territory *>();
}
vector<Territory *> NeutralPlayerStrategy::toDefend(Player *player) const
{
    return vector<Territory *>();
}

// ==================== CHEATER PLAYER STRATEGY ====================
CheaterPlayerStrategy::CheaterPlayerStrategy() {};
CheaterPlayerStrategy::~CheaterPlayerStrategy() {};

string CheaterPlayerStrategy::getStrategyName() const
{
    return toStringStrategy(StrategyName::CHEATER);
}

void CheaterPlayerStrategy::infinitArmyCheat(Territory *territory)
{
    const int ARMY_MULTIPLIER = 2;
    territory->setArmies(territory->getArmies() * ARMY_MULTIPLIER);
}

void CheaterPlayerStrategy::instantConquerCheat(const vector<Territory *> &toConquer, Player *player, Map *map)
{
    for (Territory *territory : toConquer)
    {
        territory->setOwner(player);
    }
}

void CheaterPlayerStrategy::duplicateReinforcementsCheat(Player *player)
{
    const int REINFORCEMENT_THRESHOLD = 15;

    int currentReinforcements = player->getReinforcementPool();
    if (currentReinforcements < REINFORCEMENT_THRESHOLD)
    {
        player->addReinforcements(currentReinforcements);
    }
}

bool CheaterPlayerStrategy::issueOrder(Player *player, Map *map)
{
    lastLogMessage = player->getPlayerName() + "'s Turn (" + getStrategyName() + " Strategy)";
    Notify(this, "INFO");
    
    if (hasConqueredThisTurn)
    {
        hasConqueredThisTurn = false; // reset for next round;
        return false;
    }

    int currentReinforcement = player->getReinforcementPool();

    duplicateReinforcementsCheat(player);
    if (currentReinforcement != player->getReinforcementPool())
    {
        lastLogMessage = "Reinforcements duplicated from " + to_string(currentReinforcement) + " to " + to_string(player->getReinforcementPool()) + ".";
        Notify(this, "INVENTORY");
    }

    vector<Territory *> territories = player->getTerritories();
    vector<Territory *> toConquer;

    for (Territory *territory : territories)
    {
        int previousArmi = territory->getArmies();
        infinitArmyCheat(territory);
        if (previousArmi != territory->getArmies())
        {
            lastLogMessage = "Infinit armies cheat applied on " + territory->getName() + ": " + to_string(previousArmi) + " -> " + to_string(territory->getArmies()) + ".";
            Notify(this, "INVENTORY");
           
        }
    }

    // automatically conqure all adjacent enemy territories
    int totalNeighborsChecked = 0;
    int enemyNeighborsFound = 0;

    for (Territory *territory : territories)
    {
        vector<Territory *> neighbors = map->getNeighborsOf(territory);
        totalNeighborsChecked += neighbors.size();

        for (Territory *neighbor : neighbors)
        {
            if (!player->ownsTerritoryId(neighbor->getId()))
            {
                enemyNeighborsFound++;
                if (find(toConquer.begin(), toConquer.end(), neighbor) == toConquer.end())
                {
                    toConquer.push_back(neighbor);
                }
            }
        }
    }
lastLogMessage = "Cheater Strategy checked " + to_string(totalNeighborsChecked) + " neighbors, found " + to_string(enemyNeighborsFound) + " enemy neighbors.";
    Notify(this, "DEBUG");
    

    // Log territory conquests before actually conquering them
    if (!toConquer.empty())
    {
        lastLogMessage = "Conquering " + to_string(toConquer.size()) + " territories.";
        Notify(this, "INFO");
        
        instantConquerCheat(toConquer, player, map);
    }
    else
    {
        lastLogMessage = "No adjacent enemy territories to conquer.";
        Notify(this, "WARNING");
     
    }

    hasConqueredThisTurn = true;
    return false;
}

vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player, Map *map) const
{
    return vector<Territory *>();
}

vector<Territory *> CheaterPlayerStrategy::toDefend(Player *player) const
{
    return player->getTerritories();
}
std::string PlayerStrategy::stringToLog() const
{
    return lastLogMessage;
}
std::string HumanPlayerStrategy::stringToLog() const
{
    return lastLogMessage;
}