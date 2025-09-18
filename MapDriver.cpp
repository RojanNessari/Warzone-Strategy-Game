#include "Models/Map.h"
#include <iostream>
#include <vector>
#include <string>

void testLoadMaps()
{
    // TODO: Implement loading and testing of map files
    std::vector<std::string> mapFiles = {
        "valid_map1.map",
        "valid_map2.map",
        "invalid_map1.map",
        "invalid_map2.map"};
    MapLoader loader;
    for (const auto &file : mapFiles)
    {
        std::cout << "Loading map: " << file << std::endl;
        Map *map = loader.loadMap(file);
        if (map)
        {
            std::cout << "Map loaded successfully!" << std::endl;
            std::cout << "Map connectivity: " << (map->validate() ? "PASS" : "FAIL") << std::endl;
            std::cout << "Continent connectivity: " << (map->validateContinents() ? "PASS" : "FAIL") << std::endl;
            std::cout << "Territory membership: " << (map->validateTerritoryMembership() ? "PASS" : "FAIL") << std::endl;
            delete map;
        }
        else
        {
            std::cout << "Map loading failed (invalid map file)." << std::endl;
        }
        std::cout << "-----------------------------" << std::endl;
    }
}

int main()
{
    testLoadMaps();
    return 0;
}
