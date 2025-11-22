#include "../PlayerStrategies/PlayerStrategies.h"
#include "../Models/Player.h"
#include "../Models/Map.h"
#include "../utils/logger.h"
#include "../utils/LoggingObserver.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// const string MAP_FILE = "Maps/alberta.map";
const string MAP_FILE = "Tests/003_I72_Fairchild T-31/003_I72_Fairchild T-31.map";
void testPlayerStrategies()
{
    logMessage(INFO, "========== TESTING PLAYER STRATEGIES ==========");

    // Initialize global logger (singleton pattern)
    // All Subject objects will auto-attach to it in their constructor
    LogObserver::getInstance();

    MapLoader *mapLoader = new MapLoader();
    Map *gameMap = mapLoader->loadMap(MAP_FILE);

    if (gameMap == nullptr)
    {
        logMessage(ERROR, "Map loading failed.");
        exit(1);
    }

    if (gameMap->validate())
    {
        logMessage(PROGRESSION, "Map Validation Completed");
        gameMap->printMapStatistics();
    }
    else
    {
        logMessage(ERROR, "Map Validation Failed. Exiting ...");
        exit(1);
    }

    logMessage(PROGRESSION, "Part 1: Creating players with different strategies");

    Player *humanPlayer = new Player("n00b69");
    humanPlayer->setStrategy(new HumanPlayerStrategy());

    Player *aggressivePlayer = new Player("PostalDude77");
    aggressivePlayer->setStrategy(new AggressivePlayerStrategy());

    Player *benevolentPlayer = new Player("Jalla");
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy());

    Player *neutralPlayer = new Player("xRizz");
    neutralPlayer->setStrategy(new NeutralPlayerStrategy());

    Player *cheaterPlayer = new Player("niCe");
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy());

    vector<Player *> _players;
    _players.push_back(humanPlayer);
    _players.push_back(aggressivePlayer);
    _players.push_back(benevolentPlayer);
    _players.push_back(neutralPlayer);
    _players.push_back(cheaterPlayer);

    gameMap->distributeTerritories(_players);

    // Give each player some reinforcements to work with
    for (Player *player : _players)
    {
        player->addReinforcements(10);
        logMessage(DEBUG, player->getPlayerName() + " has " + to_string(player->getReinforcementPool()) + " reinforcements");
        logMessage(DEBUG, player->getPlayerName() + " owns " + to_string(player->getTerritories().size()) + " territories");

        // Show first few territories owned
        if (player->getTerritories().size() > 0)
        {
            logMessage(DEBUG, "  First territory: " + player->getTerritories()[0]->getName() + " with " + to_string(player->getTerritories()[0]->getArmies()) + " armies");
        }
    }

    logMessage(INFO, "All players created with different strategy.");

    logMessage(PROGRESSION, "Part 2: Demonstrating dynamic strategy change");

    logMessage(DEBUG, "Neutral player is attacked! Becoming aggressive. . .");
    neutralPlayer->setStrategy(new AggressivePlayerStrategy());

    logMessage(DEBUG, "Benevolent player changes to neutral strategy. . .");
    benevolentPlayer->setStrategy(new NeutralPlayerStrategy());

    logMessage(PROGRESSION, "Part 3: Demonstrating different behaviors");

    // Give each territory some initial armies for better demonstration
    for (int i = 0; i < gameMap->getTerritoriesSize(); i++)
    {
        Territory *t = gameMap->getTerritoryByIndex(i);
        if (t)
        {
            t->setArmies(5); // Start each territory with 5 armies
        }
    }

    logMessage(INFO, "\n=== AGGRESSIVE PLAYER (PostalDude77) ===");
    logMessage(INFO, "Behavior: Deploys to strongest territory, then attacks from it");
    bool continuePlaying = true;
    int orderCount = 0;
    while (continuePlaying && orderCount < 5)
    { // Allow up to 5 orders
        continuePlaying = aggressivePlayer->issueOrder(gameMap);
        orderCount++;
    }
    logMessage(INFO, "Total orders issued: " + to_string(aggressivePlayer->getOrdersList()->size()));

    // Show the orders
    cout << "\nOrders list for " << aggressivePlayer->getPlayerName() << ":" << endl;
    for (size_t i = 0; i < aggressivePlayer->getOrdersList()->size(); i++)
    {
        Order *order = aggressivePlayer->getOrdersList()->get(i);
        cout << "  - " << *order << endl;
    }

    logMessage(INFO, "\n=== NEUTRAL PLAYER (Jalla - changed from Benevolent) ===");
    logMessage(INFO, "Behavior: Does nothing");
    continuePlaying = true;
    orderCount = 0;
    while (continuePlaying && orderCount < 3)
    {
        continuePlaying = benevolentPlayer->issueOrder(gameMap);
        orderCount++;
    }
    logMessage(INFO, "Total orders issued: " + to_string(benevolentPlayer->getOrdersList()->size()));

    logMessage(INFO, "\n=== AGGRESSIVE PLAYER (xRizz - changed from Neutral) ===");
    logMessage(INFO, "Behavior: Shows dynamic strategy change - now aggressive");
    continuePlaying = true;
    orderCount = 0;
    while (continuePlaying && orderCount < 5)
    {
        continuePlaying = neutralPlayer->issueOrder(gameMap);
        orderCount++;
    }
    logMessage(INFO, "Total orders issued: " + to_string(neutralPlayer->getOrdersList()->size()));

    // Show the orders
    cout << "\nOrders list for " << neutralPlayer->getPlayerName() << " (changed to Aggressive):" << endl;
    for (size_t i = 0; i < neutralPlayer->getOrdersList()->size(); i++)
    {
        Order *order = neutralPlayer->getOrdersList()->get(i);
        cout << "  - " << *order << endl;
    }

    logMessage(INFO, "\n=== CHEATER PLAYER (niCe) ===");
    logMessage(INFO, "Behavior: Automatically doubles armies in owned territories");

    // Count territories and armies before
    int territoriesBefore = cheaterPlayer->getTerritories().size();
    int totalArmiesBefore = 0;
    for (Territory *t : cheaterPlayer->getTerritories())
    {
        totalArmiesBefore += t->getArmies();
    }

    continuePlaying = true;
    orderCount = 0;
    while (continuePlaying && orderCount < 3)
    {
        continuePlaying = cheaterPlayer->issueOrder(gameMap);
        orderCount++;
    }

    // Count after
    int totalArmiesAfter = 0;
    for (Territory *t : cheaterPlayer->getTerritories())
    {
        totalArmiesAfter += t->getArmies();
    }

    logMessage(INFO, "Territories owned: " + to_string(territoriesBefore));
    logMessage(INFO, "Total armies before cheating: " + to_string(totalArmiesBefore));
    logMessage(INFO, "Total armies after cheating: " + to_string(totalArmiesAfter));
    logMessage(INFO, "Total orders issued: " + to_string(cheaterPlayer->getOrdersList()->size()));

    logMessage(INFO, "\n>> Human player requires user interaction (skipped in automated test)");

    for (Player *player : _players)
    {
        delete player;
    }

    _players.clear();
    delete gameMap;
    delete mapLoader;

    // Clean up global logger at program end
    LogObserver::destroyInstance();

    logMessage(INFO, "PLAYER STRATEGIES TEST COMPLETE");
}

int main()
{

    testPlayerStrategies();

    return 0;
}