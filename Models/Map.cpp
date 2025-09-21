#include "Map.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

enum Section
{
    NONE,
    CONTINENT,
    TERRITORIES,
    BORDERS
};

Territory::Territory(const std::string &name, int continentId)
    : name(name), continentId(continentId), ownerId(-1), armies(0) {}

Continent::Continent(const std::string &name, int id)
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

Map *MapLoader::loadMap(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return nullptr;
    }

    Map *map = new Map();
    std::string line;
    enum Section
    {
        NONE,
        CONTINENTS,
        TERRITORIES,
        BORDERS
    };
    Section currentSection = NONE;

    while (std::getline(file, line))
    {
        // Remove whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty() || line[0] == ';')
            continue; // Skip comments and empty lines

        if (line == "[Continents]")
        {
            currentSection = CONTINENTS;
            continue;
        }
        else if (line == "[Territories]")
        {
            currentSection = TERRITORIES;
            continue;
        }
        else if (line == "[Borders]")
        {
            currentSection = BORDERS;
            continue;
        }

        if (currentSection == CONTINENTS)
        {
            // Format: ContinentName=BonusValue
            size_t eq = line.find('=');
            if (eq != std::string::npos)
            {
                std::string name = line.substr(0, eq);
                int id = map->continents.size();
                map->continents.push_back(Continent(name, id));
            }
        }
        else if (currentSection == TERRITORIES)
        {
            // Format: TerritoryName,x,y,ContinentName
            std::stringstream ss(line);
            std::string name, xStr, yStr, continentName;
            std::getline(ss, name, ',');
            std::getline(ss, xStr, ',');
            std::getline(ss, yStr, ',');
            std::getline(ss, continentName, ',');

            // Find continent id
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
                std::cerr << "Error: Unknown continent " << continentName << std::endl;
                delete map;
                return nullptr;
            }
            map->territories.push_back(Territory(name, continentId));
            map->continents[continentId].territoryIds.push_back(map->territories.size() - 1);
        }
        else if (currentSection == BORDERS)
        {
            // Format: TerritoryName,Adjacent1,Adjacent2,...
            std::stringstream ss(line);
            std::string territoryName;
            std::getline(ss, territoryName, ',');
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
                continue; // Unknown territory

            std::string adjName;
            while (std::getline(ss, adjName, ','))
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
    }

    file.close();
    return map;
}
