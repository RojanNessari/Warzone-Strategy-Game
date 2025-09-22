#include "Map.h"

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

Territory::Territory(const string &name, int continentId)
    : name(name), continentId(continentId), ownerId(-1), armies(0) {}

Continent::Continent(const string &name, int id)
    : name(name), id(id) {}

Map::Map() {}

bool Map::validate() const
{
    // TODO: Implement map connectivity check
    return true;
}

bool Map::validateContinents() const
{
    // TODO: Implement continent connectivity check
    return true;
}

bool Map::validateTerritoryMembership() const
{
    // TODO: Implement territory membership check
    return true;
}

MapLoader::MapLoader() {}

Map *MapLoader::handleCurrentState(Section currentState, const string &line, Map *map)
{
    if (currentState == CONTINENTS)
    {
        size_t eq = line.find('=');
        if (eq != string::npos)
        {
            string name = line.substr(0, eq);
            int id = map->continents.size();
            map->continents.push_back(Continent(name, id));
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
        for (size_t i = 0; i < map->continents.size(); ++i)
        {
            if (map->continents[i].name == continentName)
            {
                continentId = map->continents[i].id;
                break;
            }
        }
        if (continentId == -1)
        {
            cerr << "Error: Unknown continent " << continentName << endl;
            delete map;
            return nullptr;
        }
        map->territories.push_back(Territory(name, continentId));
        map->continents[continentId].territoryIds.push_back(map->territories.size() - 1);
    }
    else if (currentState == BORDERS)
    {
        stringstream ss(line);
        string territoryName;
        getline(ss, territoryName, ',');
        int territoryId = -1;
        for (size_t i = 0; i < map->territories.size(); ++i)
        {
            if (map->territories[i].name == territoryName)
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
            for (size_t j = 0; j < map->territories.size(); ++j)
            {
                if (map->territories[j].name == adjName)
                {
                    map->territories[territoryId].adjacentIds.push_back(j);
                    break;
                }
            }
        }
    }
    return map;
}

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
