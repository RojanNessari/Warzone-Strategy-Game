
#include "Map.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// Territory constructor: initializes a territory with a name and continent ID.
Territory::Territory(const string &name, int continentId)
    : name(name), continentId(continentId), ownerId(-1), armies(0) {}

// Continent constructor: initializes a continent with a name and ID.
Continent::Continent(const string &name, int id)
    : name(name), id(id) {}

// Map constructor: initializes an empty map.
Map::Map() {}

// Returns the name of the territory.
std::string Territory::getName() const { return name; }
// Returns the continent ID of the territory.
int Territory::getcontinentId() const { return continentId; }
// Returns the owner ID of the territory.
int Territory::getOwnerId() const { return ownerId; }
// Returns the number of armies in the territory.
int Territory::getArmies() const { return armies; }
// Returns a reference to the list of adjacent territory IDs.
std::vector<int> &Territory::getAdjacentIds() { return adjacentIds; }

// Returns the name of the continent.
std::string Continent::getName() const { return name; }
// Returns the ID of the continent.
int Continent::getId() const { return id; }
// Returns a reference to the list of territory IDs in the continent.
std::vector<int> &Continent::getTerritoryIds() { return territoryIds; }

// Validates overall map connectivity (stub).
bool Map::validate() const
{
    // TODO: Implement map connectivity check
    return true;
}

// Validates continent connectivity (stub).
bool Map::validateContinents() const
{
    // TODO: Implement continent connectivity check
    return true;
}

// Validates territory membership in continents (stub).
bool Map::validateTerritoryMembership() const
{
    // TODO: Implement territory membership check
    return true;
}

// Adds a territory to the map.
void Map::addTerritory(const Territory &t)
{
    territories.push_back(t);
}

// Adds a continent to the map.
void Map::addContinent(const Continent &c)
{
    continents.push_back(c);
}

// Returns the number of continents in the map.
int Map::getContinentsSize() const
{
    return continents.size();
}
// Returns the number of territories in the map.
int Map::getTerritoriesSize() const
{
    return territories.size();
}

// Returns a pointer to a territory by its name, or nullptr if not found.
Territory *Map::getTerritoryByName(const string &name)
{
    for (auto &t : territories)
    {
        if (t.getName() == name)
            return &t;
    }
    return nullptr;
}

// Returns a pointer to a continent by its index, or nullptr if out of bounds.
Continent *Map::getContinentByIndex(int idx)
{
    if (idx >= 0 && idx < continents.size())
        return &continents[idx];
    return nullptr;
}

// Returns a pointer to a continent by its ID, or nullptr if not found.
Continent *Map::getContinentById(int id)
{
    for (auto &c : continents)
    {
        if (c.getId() == id)
        {
            return &c;
        }
    }
    return nullptr;
}

// Returns a pointer to a territory by its index, or nullptr if out of bounds.
Territory *Map::getTerritoryByIndex(int idx)
{
    if (idx >= 0 && idx < territories.size())
        return &territories[idx];
    return nullptr;
}

// MapLoader constructor: initializes a map loader.
MapLoader::MapLoader() {}

// Handles the current parsing state and updates the map accordingly.
Map *MapLoader::handleCurrentState(Section currentState, const string &line, Map *map)
{
    if (currentState == CONTINENTS)
    {
        size_t eq = line.find('=');
        if (eq != string::npos)
        {
            string name = line.substr(0, eq);
            int id = map->getContinentsSize();
            map->addContinent(Continent(name, id));
        }
    }
    else if (currentState == TERRITORIES)
    {
        stringstream ss(line);
        string name, xStr, yStr, continentName;
        getline(ss, name, ',');
        getline(ss, xStr, ',');
        getline(ss, yStr, ',');
        getline(ss, continentName, ',');

        int continentId = -1;
        for (size_t i = 0; i < map->getContinentsSize(); ++i)
        {
            Continent *c = map->getContinentByIndex(i);
            if (c && c->getName() == continentName)
            {
                continentId = c->getId();
                break;
            }
        }
        if (continentId == -1)
        {
            cerr << "Error: Unknown continent " << continentName << endl;
            delete map;
            return nullptr;
        }
        map->addTerritory(Territory(name, continentId));
        Continent *c = map->getContinentById(continentId);
        if (c)
            c->getTerritoryIds().push_back(map->getTerritoriesSize() - 1);
    }
    else if (currentState == BORDERS)
    {
        stringstream ss(line);
        string territoryName;
        getline(ss, territoryName, ',');
        int territoryId = -1;
        for (size_t i = 0; i < map->getTerritoriesSize(); ++i)
        {
            Territory *t = map->getTerritoryByIndex(i);
            if (t && t->getName() == territoryName)
            {
                territoryId = i;
                break;
            }
        }
        if (territoryId == -1)
            return map; // Unknown territory, skip

        string adjName;
        while (getline(ss, adjName, ','))
        {
            for (size_t j = 0; j < map->getTerritoriesSize(); ++j)
            {
                Territory *tAdj = map->getTerritoryByIndex(j);
                Territory *tMain = map->getTerritoryByIndex(territoryId);
                if (tAdj && tMain && tAdj->getName() == adjName)
                {
                    tMain->getAdjacentIds().push_back(j);
                    break;
                }
            }
        }
    }
    return map;
}

// Returns the Section enum value based on the header line.
Section MapLoader::getSectionFromHeader(const string &line)
{
    if (line == CONTINENT_HEADER)
        return CONTINENTS;
    if (line == TERRITORIES_HEADER)
        return TERRITORIES;
    if (line == BORDER_HEADER)
        return BORDERS;
    return NONE;
}

// Loads a map from a file and returns a pointer to the Map object.
Map *MapLoader::loadMap(const string &filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Cannot open file " << filename << endl;
        return nullptr;
    }

    Map *map = new Map();
    string line;
    Section currentSection = NONE;

    while (getline(file, line))
    {
        // Remove whitespace
        line.erase(0, line.find_first_not_of(WHITE_SPACE));
        line.erase(line.find_last_not_of(WHITE_SPACE) + 1);

        Section newSection = getSectionFromHeader(line);
        if (newSection != NONE)
        {
            currentSection = newSection;
            continue;
        }

        map = handleCurrentState(currentSection, line, map);
        if (!map)
        {
            // Error occurred, memory already cleaned up in handleCurrentState
            return nullptr;
        }
    }

    file.close();
    return map;
}
