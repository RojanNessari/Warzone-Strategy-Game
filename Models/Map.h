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
    const std::vector<int> &getAdjacentIds() const;
    Territory(const std::string &name, int continentId);
};

class Continent
{
private:
    std::string name;
    int id;
    std::vector<int> territoryIds;

public:
    // Constructor
    Continent(const std::string &name, int id);
    std::string getName() const;
    int getId() const;
    std::vector<int> &getTerritoryIds();
    const std::vector<int> &getTerritoryIds() const;
};

class Map
{
private:
    std::vector<Territory> territories;
    std::vector<Continent> continents;

public:
    void addTerritory(const Territory &t);
    void addContinent(const Continent &c);
    int getContinentsSize() const;
    int getTerritoriesSize() const;
    Continent *getContinentById(int id);
    Territory *getTerritoryByName(const std::string &name);
    Continent *getContinentByIndex(int idx);
    Territory *getTerritoryByIndex(int idx);
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