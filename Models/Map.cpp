#include "Map.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <random>
#include <cctype>
#include "../utils/logger.h"

using namespace std;

// Territory constructor: initializes a territory with a name, id, continent ID, and coordinates.
Territory::Territory(const string &name, int id, int continentId, int x, int y)
    : name(name), id(id), continentId(continentId), ownerId(-1), armies(0), x(x), y(y) {}

Territory::Territory(const Territory &other) : name(other.name), id(other.id), continentId(other.continentId), ownerId(other.ownerId), armies(other.armies), x(other.x),
                                               y(other.y) {};

// Assigment operator:
Territory &Territory::operator=(const Territory &other)
{
    if (this != &other)
    {
        name = other.name;
        id = other.id;
        continentId = other.continentId;
        ownerId = other.ownerId;
        armies = other.armies;
        x = other.x;
        y = other.y;
        adjacentIds = other.adjacentIds;
    }
    return *this;
}

// Assignment operator
ostream &operator<<(ostream &os, const Territory &t)
{
    os << " Territory(Name: " << t.getName()
       << " ID: " << t.getId()
       << " ContinentID: " << t.getContinentId()
       << " OwnerID: " << t.getOwnerId()
       << " Armies: " << t.getArmies()
       << " X: " << t.getX()
       << ", Y: " << t.getY()
       << ")";
    return os;
}

ostream &operator<<(ostream &os, const Continent &c)
{
    os << " Continent(Name: " << c.getName()
       << " ID: " << c.getId()
       << " Bonus: " << c.getBonusValue()
       << " Territories: " << c.getTerritoryIds().size()
       << ")";
    return os;
}

ostream &operator<<(ostream &os, const Map &m)
{
    os << "Map("
       << "Continents: " << m.getContinentsSize()
       << ", Territories: " << m.getTerritoriesSize()
       << ")\n";
    m.printMapStatistics();
    return os;
}

// Deconstructor Territory
Territory::~Territory() {};

// Continent constructor: initializes a continent with a name, ID, and bonus value.
Continent::Continent(const string &name, int id, int bonusValue)
    : name(name), id(id), bonusValue(bonusValue) {}
// Copy Constructor
Continent::Continent(const Continent &other)
    : name(other.name), id(other.id), bonusValue(other.bonusValue) {};
// Assignment Operator

Continent &Continent::operator=(const Continent &other)
{
    if (this != &other)
    {
        name = other.name;
        id = other.id;
        bonusValue = other.bonusValue;
        territoryIds = other.territoryIds;
    }
    return *this;
}

// Deconstructor Continent:
Continent::~Continent() {};

// Map constructor: initializes an empty map.
Map::Map() {};

// Deconstructor map
Map::~Map() {};

// Assignment Operator:
Map &Map::operator=(const Map &other)
{
    if (this != &other)
    {
        territories = other.territories;
        continents = other.continents;
        territoryNameToId = other.territoryNameToId;
        continentIdToIndex = other.continentIdToIndex;
        continentNameToId = other.continentNameToId;
    }
    return *this;
}

Map::Map(const Map &other) : territories(other.territories), territoryNameToId(other.territoryNameToId),
                             continentIdToIndex(other.continentIdToIndex), continentNameToId(other.continentNameToId) {}

// Territory methods
string Territory::getName() const { return name; }
int Territory::getId() const { return id; }
int Territory::getContinentId() const { return continentId; }
int Territory::getOwnerId() const { return ownerId; }
int Territory::getArmies() const { return armies; }
int Territory::getX() const { return x; }
int Territory::getY() const { return y; }
unordered_set<int> &Territory::getAdjacentIds() { return adjacentIds; }
const unordered_set<int> &Territory::getAdjacentIds() const { return adjacentIds; }

void Territory::addAdjacentTerritory(int territoryId)
{
    adjacentIds.insert(territoryId);
}

void Territory::setOwner(int playerId)
{
    ownerId = playerId;
}

void Territory::setArmies(int armyCount)
{
    armies = armyCount;
}

// Continent methods
string Continent::getName() const { return name; }
int Continent::getId() const { return id; }
int Continent::getBonusValue() const { return bonusValue; }
unordered_set<int> &Continent::getTerritoryIds() { return territoryIds; }
const unordered_set<int> &Continent::getTerritoryIds() const { return territoryIds; }

void Continent::addTerritory(int territoryId)
{
    territoryIds.insert(territoryId);
}

void Continent::setBonusValue(int bonus)
{
    bonusValue = bonus;
}

// Validates the entire map: connectivity, continent validity, and territory membership
bool Map::validate() const
{
    logMessage(INFO, "=== MAP VALIDATION ===");

    bool isValid = true;

    // 1. Check if map is a connected graph
    bool connected = isConnectedGraph();
    logMessage(INFO, string("1. Map connectivity: ") + (connected ? "PASSED" : "FAILED"));
    isValid &= connected;

    // 2. Check if continents are connected subgraphs
    bool continentsValid = validateContinents();
    logMessage(INFO, string("2. Continent connectivity: ") + (continentsValid ? "PASSED" : "FAILED"));
    isValid &= continentsValid;

    // 3. Check territory membership (each territory belongs to exactly one continent)
    bool membershipValid = validateTerritoryMembership();
    logMessage(INFO, string("3. Territory membership: ") + (membershipValid ? "PASSED" : "FAILED"));
    isValid &= membershipValid;
    logMessage(INFO, string("Overall map validation: ") + (isValid ? "VALID" : "INVALID"));
    logMessage(INFO, "======================");

    return isValid;
}

// Check if the map is a connected graph using BFS
bool Map::isConnectedGraph() const
{
    if (territories.empty())
    {
        return false;
    }

    unordered_set<int> visited;
    queue<int> q;

    // Start BFS from territory 0
    q.push(0);
    visited.insert(0);

    while (!q.empty())
    {
        int currentId = q.front();
        q.pop();

        // Visit all adjacent territories
        for (int adjId : territories[currentId].getAdjacentIds())
        {
            if (visited.find(adjId) == visited.end())
            {
                visited.insert(adjId);
                q.push(adjId);
            }
        }
    }

    return visited.size() == territories.size();
}

// Validates that each continent is a connected subgraph
bool Map::validateContinents() const
{
    for (const auto &continent : continents)
    {
        const auto &territoryIds = continent.getTerritoryIds();

        if (territoryIds.empty())
        {
            logMessage(DEBUG, string("Continent '") + continent.getName() + "' has no territories");
            continue;
        }

        // BFS to check if all territories in this continent are connected
        unordered_set<int> visited;
        queue<int> q;

        // Start from any territory in the continent
        int startTerritory = *territoryIds.begin();
        q.push(startTerritory);
        visited.insert(startTerritory);

        while (!q.empty())
        {
            int currentId = q.front();
            q.pop();

            // Check all adjacent territories
            for (int adjId : territories[currentId].getAdjacentIds())
            {
                // Only consider territories in the same continent
                if (territories[adjId].getContinentId() == continent.getId() &&
                    visited.find(adjId) == visited.end())
                {
                    visited.insert(adjId);
                    q.push(adjId);
                }
            }
        }

        // Check if all territories in this continent were visited
        if (visited.size() != territoryIds.size())
        {
            logMessage(ERROR, string("Continent '") + continent.getName() + "' is not connected: " + to_string(visited.size()) + "/" + to_string(territoryIds.size()) + " territories reachable");
            return false;
        }
    }
    return true;
}

// Validates that each territory belongs to exactly one continent
bool Map::validateTerritoryMembership() const
{
    // Check each territory has a valid continent
    for (size_t i = 0; i < territories.size(); ++i)
    {
        const Territory &territory = territories[i];
        int continentId = territory.getContinentId();

        // Find the continent
        bool foundContinent = false;
        for (const auto &continent : continents)
        {
            if (continent.getId() == continentId)
            {
                foundContinent = true;
                // Check if this territory is listed in the continent
                if (continent.getTerritoryIds().find(i) == continent.getTerritoryIds().end())
                {
                    cout << " Territory '" << territory.getName()
                         << "' not found in continent '" << continent.getName() << "'" << endl;
                    return false;
                }
                break;
            }
        }

        if (!foundContinent)
        {
            cout << " Territory '" << territory.getName()
                 << "' has invalid continent ID: " << continentId << endl;
            return false;
        }
    }

    // Check each continent's territory list is valid
    for (const auto &continent : continents)
    {
        for (int territoryId : continent.getTerritoryIds())
        {
            if (territoryId < 0 || territoryId >= static_cast<int>(territories.size()))
            {
                cout << " Continent '" << continent.getName()
                     << "' has invalid territory ID: " << territoryId << endl;
                return false;
            }

            if (territories[territoryId].getContinentId() != continent.getId())
            {
                cout << " Territory '" << territories[territoryId].getName()
                     << "' continent mismatch" << endl;
                return false;
            }
        }
    }

    return true;
}

// Adds a territory to the map with hash map indexing
void Map::addTerritory(const Territory &t)
{
    int index = territories.size();
    territories.push_back(t);
    territoryNameToId[t.getName()] = index;
}

// Adds a continent to the map with hash map indexing
void Map::addContinent(const Continent &c)
{
    int index = continents.size();
    continents.push_back(c);
    continentIdToIndex[c.getId()] = index;
    continentNameToId[c.getName()] = c.getId();
}

// Territory access methods with O(1) performance
Territory *Map::getTerritoryByName(const string &name)
{
    auto it = territoryNameToId.find(name);
    return (it != territoryNameToId.end()) ? &territories[it->second] : nullptr;
}

Territory *Map::getTerritoryById(int id)
{
    return (id >= 0 && id < static_cast<int>(territories.size())) ? &territories[id] : nullptr;
}

Territory *Map::getTerritoryByIndex(int idx)
{
    return getTerritoryById(idx); // Same thing in this implementation
}

int Map::getTerritoriesSize() const
{
    return territories.size();
}

// Continent access methods with O(1) performance
Continent *Map::getContinentById(int id)
{
    auto it = continentIdToIndex.find(id);
    return (it != continentIdToIndex.end()) ? &continents[it->second] : nullptr;
}

Continent *Map::getContinentByName(const string &name)
{
    auto it = continentNameToId.find(name);
    if (it != continentNameToId.end())
    {
        return getContinentById(it->second);
    }
    return nullptr;
}

Continent *Map::getContinentByIndex(int idx)
{
    return (idx >= 0 && idx < static_cast<int>(continents.size())) ? &continents[idx] : nullptr;
}

int Map::getContinentsSize() const
{
    return continents.size();
}

// Helper method to print map statistics
void Map::printMapStatistics() const
{
    cout << "\n=== MAP STATISTICS ===" << endl;
    cout << "Territories: " << territories.size() << endl;
    cout << "Continents: " << continents.size() << endl;

    for (const auto &continent : continents)
    {
        cout << "  - " << continent.getName() << " (ID: " << continent.getId()
             << ", Bonus: " << continent.getBonusValue()
             << ", Territories: " << continent.getTerritoryIds().size() << ")" << endl;
    }
    cout << "=====================\n"
         << endl;
}

// MapLoader constructor: initializes a map loader.
MapLoader::MapLoader() {}
MapLoader::MapLoader(const MapLoader &other) : MapLoader() {}
MapLoader::~MapLoader() {};
// Handles the current parsing state and updates the map accordingly.
Map *MapLoader::handleCurrentState(Section currentState, const string &line, Map *map)
{
    if (currentState == CONTINENTS)
    {
        size_t eq = line.find('=');
        if (eq != string::npos)
        {
            string name = line.substr(0, eq);
            string bonusStr = line.substr(eq + 1);
            int bonusValue = 0;
            try
            {
                bonusValue = stoi(bonusStr);
            }
            catch (const exception &)
            {
                cout << "Warning: Invalid bonus value for continent " << name << endl;
            }

            int id = map->getContinentsSize();
            map->addContinent(Continent(name, id, bonusValue));
            cout << "[DEBUG] Added continent: " << name << " (ID: " << id << ", Bonus: " << bonusValue << ")" << endl;
        }
    }
    else if (currentState == TERRITORIES)
    {
        stringstream ss(line);
        string name, xStr, yStr, continentName;

        // Parse basic territory info
        if (!getline(ss, name, ','))
            return map;
        if (!getline(ss, xStr, ','))
            return map;
        if (!getline(ss, yStr, ','))
            return map;
        if (!getline(ss, continentName, ','))
            return map;

        // Find continent using O(1) lookup
        Continent *continent = map->getContinentByName(continentName);

        if (!continent)
        {
            logMessage(ERROR, string("Error: Unknown continent '") + continentName + "' for territory '" + name + "'");
            delete map;
            return nullptr;
        }

        // Create territory with unique ID and coordinates
        int territoryId = map->getTerritoriesSize();
        int x = 0, y = 0;
        try
        {
            x = stoi(xStr);
            y = stoi(yStr);
        }
        catch (const exception &)
        {
            logMessage(WARNING, string("Warning: Invalid coordinates for territory ") + name);
        }
        Territory territory(name, territoryId, continent->getId(), x, y);

        // Parse adjacent territories
        string adjName;
        vector<string> adjacentNames;
        while (getline(ss, adjName, ','))
        {
            // Remove whitespace
            adjName.erase(0, adjName.find_first_not_of(WHITE_SPACE));
            adjName.erase(adjName.find_last_not_of(WHITE_SPACE) + 1);
            if (!adjName.empty())
            {
                adjacentNames.push_back(adjName);
            }
        }

        logMessage(DEBUG, string("Territory: ") + name + " -> " + continentName + " (Adjacent: " + to_string(adjacentNames.size()) + ")");

        // Add territory to map
        map->addTerritory(territory);
        continent->addTerritory(territoryId);

        // Store adjacent names for later processing (after all territories are loaded)
        // We'll do a second pass to establish adjacencies
        if (!adjacentNames.empty())
        {
            // Store this info temporarily - we'll handle it in loadMap
            Territory *addedTerritory = map->getTerritoryById(territoryId);
            // For now, we can't resolve adjacencies until all territories are loaded
        }
    }
    return map;
}

// Returns the Section enum value based on the header line.
Section MapLoader::getSectionFromHeader(const string &line)
{
    string lowercasedString = line; // create a copy of original string
    transform(lowercasedString.begin(), lowercasedString.end(), lowercasedString.begin(),
              [](unsigned char c)
              { return tolower(c); });

    if (lowercasedString == CONTINENT_HEADER) // [BUG-FIX] check if equal [Continent] to lower case
        return CONTINENTS;
    if (lowercasedString == TERRITORIES_HEADER) // [BUG-FIX] check if equal [Territories] to lower case
        return TERRITORIES;
    return NONE;
}

const char *sectionToString(Section section)
{
    switch (section)
    {
    case CONTINENTS:
        return "CONTINENTS";
    case TERRITORIES:
        return "TERRITORIES";
    case NONE:
        return "NONE";
    default:
        return "UNKNOWN";
    }
}

// Loads a map from a file and returns a pointer to the Map object.
Map *MapLoader::loadMap(const string &filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        logMessage(ERROR, string("Error: Cannot open file ") + filename);
        return nullptr;
    }

    Map *map = new Map();
    string line;
    Section currentSection = NONE;

    // Store territory adjacency info for second pass
    vector<pair<string, vector<string>>> territoryAdjacencies;

    logMessage(INFO, string("Loading map from: ") + filename);

    // First pass: Load continents and territories
    while (getline(file, line))
    {
        // Remove whitespace
        line.erase(0, line.find_first_not_of(WHITE_SPACE));
        line.erase(line.find_last_not_of(WHITE_SPACE) + 1);
        logMessage(DEBUG, string("line: ") + line);

        if (line.empty())
            continue;

        Section newSection = getSectionFromHeader(line);
        if (newSection != NONE)
        {
            string strSection = sectionToString(newSection);
            logMessage(DEBUG, string("Detected Section: ") + strSection);
            currentSection = newSection;
            continue;
        }

        // Handle territories with adjacency storage
        if (currentSection == TERRITORIES)
        {
            stringstream ss(line);
            string name, xStr, yStr, continentName;

            if (!getline(ss, name, ','))
                continue;
            if (!getline(ss, xStr, ','))
                continue;
            if (!getline(ss, yStr, ','))
                continue;
            if (!getline(ss, continentName, ','))
                continue;

            // Find continent
            Continent *continent = map->getContinentByName(continentName);
            if (!continent)
            {
                cerr << "Error: Unknown continent '" << continentName << "' for territory '" << name << "'" << endl;
                delete map;
                return nullptr;
            }

            // Create territory with coordinates
            int territoryId = map->getTerritoriesSize();
            int x = 0, y = 0;
            try
            {
                x = stoi(xStr);
                y = stoi(yStr);
            }
            catch (const exception &)
            {
                cout << "Warning: Invalid coordinates for territory " << name << endl;
            }
            Territory territory(name, territoryId, continent->getId(), x, y);

            // Parse and store adjacent territories for second pass
            string adjName;
            vector<string> adjacentNames;
            while (getline(ss, adjName, ','))
            {
                adjName.erase(0, adjName.find_first_not_of(WHITE_SPACE));
                adjName.erase(adjName.find_last_not_of(WHITE_SPACE) + 1);
                if (!adjName.empty())
                {
                    adjacentNames.push_back(adjName);
                }
            }

            // Store for second pass
            territoryAdjacencies.push_back({name, adjacentNames});

            // Add territory to map
            map->addTerritory(territory);
            continent->addTerritory(territoryId);

            logMessage(DEBUG, string("Territory: ") + name + " -> " + continentName + " (Adjacent: " + to_string(adjacentNames.size()) + ")");
        }
        else
        {
            // Handle other sections normally
            map = handleCurrentState(currentSection, line, map);
            if (!map)
            {
                return nullptr;
            }
        }
    }

    file.close();

    // Second pass: Establish adjacencies
    logMessage(DEBUG, "Establishing adjacencies...");
    for (const auto &territoryAdj : territoryAdjacencies)
    {
        const string &territoryName = territoryAdj.first;
        const vector<string> &adjacentNames = territoryAdj.second;

        Territory *territory = map->getTerritoryByName(territoryName);
        if (!territory)
            continue;

        for (const string &adjName : adjacentNames)
        {
            Territory *adjTerritory = map->getTerritoryByName(adjName);
            if (adjTerritory)
            {
                territory->addAdjacentTerritory(adjTerritory->getId());
                // Make adjacency bidirectional
                adjTerritory->addAdjacentTerritory(territory->getId());
            }
            else
            {
                logMessage(WARNING, string("Adjacent territory '") + adjName + "' not found for '" + territoryName + "'");
            }
        }
    }

    logMessage(INFO, "Map loaded successfully!");
    map->printMapStatistics();

    return map;
}

// Distribute territories fairly among players
void Map::distributeTerritories(vector<Player *> &players)
{
    if (players.empty() || territories.empty())
    {
        logMessage(ERROR, "Cannot distribute territories: no players or no territories available.");
        return;
    }

    // Create a shuffled list of territory indices
    vector<int> territoryIndices;
    for (size_t i = 0; i < territories.size(); ++i)
    {
        territoryIndices.push_back(i);
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(territoryIndices.begin(), territoryIndices.end(), g);

    // Distribute territories in round-robin fashion
    size_t playerIndex = 0;
    for (int territoryIdx : territoryIndices)
    {
        Territory *territory = &territories[territoryIdx];

        // Assign territory to current player
        territory->setOwner(playerIndex);

        // Add territory pointer to player's collection
        players[playerIndex]->toDefend().push_back(territory);

        // Move to next player (round-robin)
        playerIndex = (playerIndex + 1) % players.size();
    }

    // Print distribution summary
    logMessage(INFO, "Territory distribution:");
    for (size_t i = 0; i < players.size(); ++i)
    {
        int count = 0;
        for (const auto &territory : territories)
        {
            if (territory.getOwnerId() == static_cast<int>(i))
                count++;
        }
        logMessage(INFO, string("  ") + players[i]->getPlayerName() + ": " + to_string(count) + " territories");
    }
}
