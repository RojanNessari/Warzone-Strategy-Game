#include "Models/Map.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
using namespace std;

const string TEST_DIR = "Tests";

vector<string> getAllMapFiles(const string &rootDir)
{
    vector<string> mapFiles;
    for (const auto &entry : filesystem::recursive_directory_iterator(rootDir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".map")
        {
            mapFiles.push_back(entry.path().string());
        }
    }
    return mapFiles;
}

void testLoadMaps()
{
    vector<string> mapFiles = getAllMapFiles(TEST_DIR);
    MapLoader loader;
    for (const auto &file : mapFiles)
    {
        cout << "🗺️ Found Map File: " << file << endl;
        cout << "⏳ Loading map..." << endl;

        Map *map = loader.loadMap(file);

        if (map)
        {
            cout << "✅ Map loaded successfully!" << endl;
            cout << "🔗 Map connectivity: " << (map->validate() ? "✅ PASS" : "❌ FAIL") << endl;
            cout << "🌍 Continent connectivity: " << (map->validateContinents() ? "✅ PASS" : "❌ FAIL") << endl;
            cout << "🏰 Territory membership: " << (map->validateTerritoryMembership() ? "✅ PASS" : "❌ FAIL") << endl;
            delete map;
        }
        else
        {
            cout << "🚫 Map loading failed (invalid map file)." << endl;
        }

        cout << "🧹-----------------------------" << endl;
    }
}

int main()
{
    testLoadMaps();
    return 0;
}
