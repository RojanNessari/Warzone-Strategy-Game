#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <map>
#include <set>

class Territory
{
public:
    std::string name;
    int continentId;
    int ownerId;
    int armies;
    std::vector<int> adjacentIds;
    Territory(const std::string &name, int continentId);
};

class Continent
{
public:
    std::string name;
    int id;
    std::vector<int> territoryIds;
    Continent(const std::string &name, int id);
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
};

#endif // MAP_H
