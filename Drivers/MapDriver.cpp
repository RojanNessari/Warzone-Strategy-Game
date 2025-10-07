#include "../Models/Map.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
using namespace std;

const string TEST_DIR = "Tests";
const string SINGLE_TEST_DIR = "./Tests/AUCircles/AuCircles.map";

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

void singleMapLoadDebuger()
{
    MapLoader loader;
    string mapFile = SINGLE_TEST_DIR;

    cout << "  Testing single map: " << mapFile << endl;
    Map *map = loader.loadMap(mapFile);

    if (map)
    {
        cout << "\n Map loaded successfully!" << endl;

        // The validate() method now does comprehensive testing
        bool isValid = map->validate();

        cout << "\n Final Result: " << (isValid ? " VALID MAP" : " INVALID MAP") << endl;
        delete map;
    }
    else
    {
        cout << " Map loading failed (invalid map file)." << endl;
    }

    cout << "\n"
         << string(50, '=') << endl;
}

void testLoadMaps()
{
    cout << " COMPREHENSIVE MAP TESTING" << endl;
    cout << string(50, '=') << endl;

    vector<string> mapFiles = getAllMapFiles(TEST_DIR);
    vector<string> failedMapFiles;
    MapLoader loader;

    int validMaps = 0;
    int invalidMaps = 0;

    for (const auto &file : mapFiles)
    {
        cout << "\n Testing: " << file << endl;

        Map *map = loader.loadMap(file);

        if (map)
        {
            cout << " Map loaded successfully!" << endl;

            // Comprehensive validation
            bool isValid = map->validate();

            if (isValid)
            {
                validMaps++;
                cout << " Result:  VALID MAP" << endl;
            }
            else
            {
                invalidMaps++;
                failedMapFiles.push_back(file);
                cout << " Result:  INVALID MAP" << endl;
            }

            delete map;
        }
        else
        {
            invalidMaps++;
            failedMapFiles.push_back(file);
            cout << " Map loading failed (file format error)." << endl;
        }

        cout << string(40, '-') << endl;
    }

    cout << "\n FINAL SUMMARY:" << endl;
    cout << "   Valid maps: " << validMaps << endl;
    cout << "   Invalid maps: " << invalidMaps << endl;
    cout << "   Total tested: " << (validMaps + invalidMaps) << endl;

    if (!failedMapFiles.empty())
    {
        cout << "   Failed map files:" << endl;
        for (const auto &name : failedMapFiles)
        {
            cout << "      " << name << endl;
        }
    }
    cout << string(50, '=') << endl;
}
