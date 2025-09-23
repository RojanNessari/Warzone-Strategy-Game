#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

enum Section
{
    NONE,
    CONTINENTS,
    TERRITORIES
};

const std::string WHITE_SPACE = " \t\r\n";
const std::string CONTINENT_HEADER = "[Continents]";
const std::string TERRITORIES_HEADER = "[Territories]";

class Territory
{
private:
    std::string name;
    int id;
    int continentId;
    int ownerId;
    int armies;
    std::unordered_set<int> adjacentIds;  // Using set for O(1) lookup and no duplicates

public:
    Territory(const std::string &name, int id, int continentId);
    std::string getName() const;
    int getId() const;
    int getContinentId() const;
    int getOwnerId() const;
    int getArmies() const;
    std::unordered_set<int> &getAdjacentIds();
    const std::unordered_set<int> &getAdjacentIds() const;
    void addAdjacentTerritory(int territoryId);
    void setOwner(int playerId);
    void setArmies(int armyCount);
};

class Continent
{
private:
    std::string name;
    int id;
    int bonusValue;
    std::unordered_set<int> territoryIds;  // Using set for O(1) operations

public:
    Continent(const std::string &name, int id, int bonusValue = 0);
    std::string getName() const;
    int getId() const;
    int getBonusValue() const;
    std::unordered_set<int> &getTerritoryIds();
    const std::unordered_set<int> &getTerritoryIds() const;
    void addTerritory(int territoryId);
    void setBonusValue(int bonus);
};

class Map
{
private:
    std::vector<Territory> territories;
    std::vector<Continent> continents;
    
    // Hash maps for O(1) lookup performance
    std::unordered_map<std::string, int> territoryNameToId;  // territory name -> territory index
    std::unordered_map<int, int> continentIdToIndex;        // continent id -> continent index
    std::unordered_map<std::string, int> continentNameToId; // continent name -> continent id

public:
    Map();
    
    // Territory operations
    void addTerritory(const Territory &t);
    Territory *getTerritoryByName(const std::string &name);
    Territory *getTerritoryById(int id);
    Territory *getTerritoryByIndex(int idx);
    int getTerritoriesSize() const;
    
    // Continent operations  
    void addContinent(const Continent &c);
    Continent *getContinentById(int id);
    Continent *getContinentByName(const std::string &name);
    Continent *getContinentByIndex(int idx);
    int getContinentsSize() const;
    
    // Graph connectivity validation
    bool validate() const;
    bool isConnectedGraph() const;
    bool validateContinents() const;
    bool validateTerritoryMembership() const;
    
    // Helper methods
    void printMapStatistics() const;
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

// To do:
/*
    - Create TerritoryNode() class
        {
         Private Property:
         string name;
         int id;

         public:
          getName;
          getId
        }


    - Create Territoy class
        {
        Private Property:
        TerritoryNode territory;
        vector< TerritoryNode> adjacentNodes;

        public:
        add adjacent Territory( name, id )
        }
    - Create Continent Class
        {
        private properties:
        string name;
        int id;
        vector<Territories> territories that belong to the continent
        public:
        getContinentName;
        getContinentId
        AddTerritoryToContinent
        }
*/