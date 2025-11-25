#include <iostream>
#include <vector>
#include <string>
#include "Map.h" 
#include "MapLoader.h" 

using std::cout;
using std::endl;
using std::string;
using std::vector;

void testMapValidity() {
    cout << "\n========================================" << endl;
    cout << "      STARTING MAP VALIDITY TEST        " << endl;
    cout << "========================================" << endl;

    MapLoader* loader = new MapLoader();
    Map* testMap = nullptr;

    // TEST CASE 1: File Does Not Exist 
    cout << "\n[TEST 1] Loading non-existent file 'ghost_map.map'..." << endl;
    try {
        testMap = loader->loadMap("ghost_map.map");

        if (testMap == nullptr) {
            cout << "[PASS] Loader correctly returned nullptr for missing file." << endl;
        }
        else {
            cout << "[FAIL] Loader created a map object for a file that doesn't exist!" << endl;
            delete testMap; // Cleanup if fail
            testMap = nullptr;
        }
    }
    catch (...) {
        cout << "[PASS] Loader threw an exception for missing file (acceptable behavior)." << endl;
    }

    // TEST CASE 2: Invalid Map
    cout << "\n[TEST 2] Loading invalid map 'invalid_graph.map' (Disconnected)..." << endl;
    testMap = loader->loadMap("invalid_graph.map");

    if (testMap != nullptr) {
        cout << "Validating map structure..." << endl;
        bool isValid = testMap->validate();

        if (!isValid) {
            cout << "[PASS] Map::validate() correctly identified the map is invalid." << endl;
        }
        else {
            cout << "[FAIL] Map::validate() claimed the disconnected map was valid!" << endl;
        }

        // CLEANUP 
        delete testMap;
        testMap = nullptr;
    }
    else {
        cout << "[WARNING] Could not load 'invalid_graph.map'. Ensure file exists." << endl;
    }

    // TEST CASE 3: Valid Map 
    cout << "\n[TEST 3] Loading valid map 'valid_map.map'..." << endl;
    testMap = loader->loadMap("valid_map.map");

    if (testMap != nullptr) {
        if (testMap->validate()) {
            cout << "[PASS] Map loaded and validated successfully." << endl;
        }
        else {
            cout << "[FAIL] A known good map failed validation." << endl;
        }
        delete testMap;
        testMap = nullptr;
    }
    else {
        cout << "[WARNING] Could not load 'valid_map.map'. Ensure file exists." << endl;
    }

    delete loader; // Clean up 
    cout << "\n[DONE] Map Validity Test Completed." << endl;
}
