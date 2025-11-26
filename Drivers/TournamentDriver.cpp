#include <iostream>
#include <vector>
#include "../Models/GameEngine.h"
#include "../utils/logger.h"
#include "../utils/LoggingObserver.h"
#include <string>
#include <vector>
using namespace std;

void testTournament(vector<string> mapFiles,
                    vector<string> playerStrategies,
                    int numGames, int maxTurns)
{
    LogObserver::getInstance();

    logMessage(EVENT, "=== Testing Tournament Mode with Player Strategies ===");

    GameEngine engine;
    engine.buildGraph();

    try
    {
        engine.runTournament(mapFiles, playerStrategies, numGames, maxTurns);
    }
    catch (const std::exception &e)
    {
        logMessage(ERROR, string("Tournament error: ") + e.what());
        exit(1);
    }

    logMessage(EVENT, "=== Tournament Test Complete ===");

    // Clean up global logger at program end
    LogObserver::destroyInstance();
}

bool argumentValidator(int argc, char *argv[],
                       vector<string> &mapFiles,
                       vector<string> &playerStrategies,
                       int &numGames,
                       int &maxTurns)
{

    // Initialize the logger
    LogObserver *logger = LogObserver::getInstance();

    if (argc < 2)
    {
        cout << "No arguments provided." << endl;
        cout << "Usage: tournament -M <mapfiles> -P <strategies> -G <games> -D <turns>" << endl;
        cout << "Note: Use quotes for paths with spaces: -M \"path with spaces.map\"" << endl;
        return false;
    }

    // Parse arguments
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];

        if (arg == "-M")
        {
            // Read all map files until next flag
            string currentPath = "";
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                string mapFile = argv[++i];
                // Remove trailing comma if present
                if (!mapFile.empty() && mapFile.back() == ',')
                    mapFile.pop_back();

                // If currentPath is not empty, this might be continuation of a space-separated path
                if (!currentPath.empty())
                {
                    currentPath += " " + mapFile;
                }
                else
                {
                    currentPath = mapFile;
                }

                // Check if this looks like a complete path (ends with .map or is followed by comma/flag)
                bool isComplete = false;
                if (currentPath.find(".map") != string::npos)
                {
                    // Check if next arg is a flag or new path
                    if (i + 1 >= argc || argv[i + 1][0] == '-' ||
                        string(argv[i + 1]).find("Tests/") == 0 ||
                        string(argv[i + 1]).find("Maps/") == 0)
                    {
                        isComplete = true;
                    }
                }

                if (isComplete && !currentPath.empty())
                {
                    mapFiles.push_back(currentPath);
                    currentPath = "";
                }
            }
            // Add any remaining path
            if (!currentPath.empty())
            {
                mapFiles.push_back(currentPath);
            }
        }
        else if (arg == "-P")
        {
            // Read all player strategies until next flag
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                string strategy = argv[++i];
                // Remove trailing comma and whitespace if present
                if (!strategy.empty() && strategy.back() == ',')
                    strategy.pop_back();
                if (!strategy.empty())
                    playerStrategies.push_back(strategy);
            }
        }
        else if (arg == "-G" && i + 1 < argc)
        {
            try
            {
                numGames = stoi(argv[++i]);
            }
            catch (const exception &e)
            {
                logMessage(ERROR, "Invalid number for -G");
                return false;
            }
        }
        else if (arg == "-D" && i + 1 < argc)
        {
            try
            {
                maxTurns = stoi(argv[++i]);
            }
            catch (const exception &e)
            {
                logMessage(ERROR, "Invalid number for -D");
                return false;
            }
        }
        else
        {
            logMessage(ERROR, "Unknown argument: " + arg);
            return false;
        }
    }

    // Validate required arguments
    if (mapFiles.empty())
    {
        logMessage(ERROR, "Error: At least one map file required (-M)");
        return false;
    }
    if (playerStrategies.empty())
    {
        logMessage(ERROR, "Error: At least one player strategy required (-P)");
        return false;
    }
    if (numGames <= 0)
    {

        logMessage(ERROR, "Error: Number of games must be positive (-G)");
        return false;
    }
    if (maxTurns <= 0)
    {
        logMessage(ERROR, "Error: Max turns must be positive (-D)");
        return false;
    }
    logMessage(INFO, "Maps:");
    for (const auto &map : mapFiles)
        cout << " " + map << endl;
    logMessage(INFO, "Players:");
    for (const auto &player : playerStrategies)
        cout << " " + player;
    cout << "\n";
    logMessage(INFO, "Games: " + to_string(numGames));
    logMessage(INFO, "Max Turn: " + to_string(maxTurns));

    // Log tournament details to file
    logger->logToFile(EVENT, "Tournament mode:");

    // Format maps
    string mapsStr = "M: ";
    for (size_t i = 0; i < mapFiles.size(); i++)
    {
        mapsStr += mapFiles[i];
        if (i < mapFiles.size() - 1)
            mapsStr += ", ";
    }
    logger->logToFile(EVENT, mapsStr);

    // Format player strategies
    string playersStr = "P: ";
    for (size_t i = 0; i < playerStrategies.size(); i++)
    {
        playersStr += playerStrategies[i];
        if (i < playerStrategies.size() - 1)
            playersStr += ", ";
    }
    logger->logToFile(EVENT, playersStr);

    // Log games and max turns
    logger->logToFile(EVENT, "G: " + to_string(numGames));
    logger->logToFile(EVENT, "D: " + to_string(maxTurns));

    return true; // All validations passed
}

/*
int main(int argc, char *argv[])
{
    vector<string> mapFiles;
    vector<string> playerStrategies;
    int numGames = 0;
    int maxTurns = 0;

    if (!argumentValidator(argc, argv, mapFiles, playerStrategies, numGames, maxTurns))
    {
        exit(1);
    }
    testTournament(mapFiles, playerStrategies, numGames, maxTurns);
    return 0;
}*/