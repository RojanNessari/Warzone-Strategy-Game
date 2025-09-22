#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

enum Section
{
    NONE,
    CONTINENTS,
    TERRITORIES,
    BORDERS
};

const std::string WHITE_SPACE = " \t\r\n";
const std::string CONTINENT_HEADER = "[Continents]";
const std::string TERRITORIES_HEADER = "[Territories]";
const std::string BORDER_HEADER = "[Borders]";

class Territory
{ // Constructor

private:
    std::string name;
    int continentId;
    int ownerId;
    int armies;
    std::vector<int> adjacentIds;

public:
    std::string getName() const;
    int getcontinentId() const;
    int getOwnerId() const;
    int getArmies() const;
    std::vector<int> &getAdjacentIds();
    Territory(const std::string &name, int continentId);
};

class Continent
{
public:
    // Constructor
    Continent(const std::string &name, int id);

private:
    std::string name;
    int id;
    std::vector<int> territoryIds;

public:
    std::string getName() const;
    int getId() const;
    std::vector<int> &getTerritoryIds();
};

class Map
{
public:
    std::vector<Territory> territories;
    std::vector<Continent> continents;
    Map();
    bool validate() const;
    bool validateContinents() const;
    bool validateTerritoryMembership() const;
};

class MapLoader
{
public:
    MapLoader();
    Map *loadMap(const std::string &filename);
    Map *handleCurrentState(Section currentState, const std::string &line, Map *map);
    Section getSectionFromHeader(const std::string &line);
};

#endif // MAP_H