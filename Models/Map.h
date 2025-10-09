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
const std::string CONTINENT_HEADER = "[continents]";
const std::string TERRITORIES_HEADER = "[territories]";

class Territory
{
private:
    // obj property
    std::string name;
    int id;
    int continentId;
    int ownerId;
    int armies;
    int x;
    int y;
    std::unordered_set<int> adjacentIds; // Using set for O(1) lookup and no duplicates

public:
    // Constructor
    Territory(const std::string &name, int id, int continentId, int x = 0, int y = 0);
    // Copy Constructory
    Territory(const Territory &other);
    // Assignment Operator
    Territory &operator=(const Territory &other);
    // deconstructor
    ~Territory();

    // stream insertion operator:
    friend std::ostream &operator<<(std::ostream &os, const Territory &t);
    // Getters
    std::string getName() const;
    int getId() const;
    int getContinentId() const;
    int getOwnerId() const;
    int getArmies() const;
    std::unordered_set<int> &getAdjacentIds();
    int getX() const;
    int getY() const;
    const std::unordered_set<int> &getAdjacentIds() const;
    // setters
    void addAdjacentTerritory(int territoryId);
    void setOwner(int playerId);
    void setArmies(int armyCount);
};

class Continent
{
private:
    // Obj property
    std::string name;
    int id;
    int bonusValue;
    std::unordered_set<int> territoryIds; // Using set for O(1) operations

public:
    // Constructor
    Continent(const std::string &name, int id, int bonusValue = 0);
    // Copy Constructor
    Continent(const Continent &other);
    // Assignment Operator
    Continent &operator=(const Continent &other);
    // Deconstructor:
    ~Continent();
    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Continent &c);
    // getters
    std::string getName() const;
    int getId() const;
    int getBonusValue() const;
    std::unordered_set<int> &getTerritoryIds();
    const std::unordered_set<int> &getTerritoryIds() const;
    // setters
    void addTerritory(int territoryId);
    void setBonusValue(int bonus);
};

class Map
{
private:
    // Obj property
    std::vector<Territory> territories;
    std::vector<Continent> continents;

    // Hash maps for O(1) lookup performance
    std::unordered_map<std::string, int> territoryNameToId; // territory name -> territory index
    std::unordered_map<int, int> continentIdToIndex;        // continent id -> continent index
    std::unordered_map<std::string, int> continentNameToId; // continent name -> continent id

public:
    // Constructor
    Map();
    // Copy Constructor
    Map(const Map &other);
    // Deconstructor:
    ~Map();
    // Assignment operator
    Map &operator=(const Map &other);
    // assignment insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Map &m);
    // Territory operations
    void addTerritory(const Territory &t);
    // Getters
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
    // Constructor
    MapLoader();
    ~MapLoader();
    // copy Constructor
    MapLoader(const MapLoader &other);
    Map *loadMap(const std::string &filename);
    Map *handleCurrentState(Section currentState, const std::string &line, Map *map);
    Section getSectionFromHeader(const std::string &line);
};

#endif // MAP_H