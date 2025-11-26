#include "../PlayerStrategies/PlayerStrategies.h"
#include "../Models/Player.h"
#include "../Models/Map.h"
#include "../Models/Cards.h"
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
    }
    else
    {
        logMessage(ERROR, "Map Validation Failed. Exiting ...");
        exit(1);
    }

    // Create a deck for the game
    Deck *gameDeck = new Deck();
    logMessage(DEBUG, "Game deck created");

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

    logMessage(INFO, "All players created with different strategy.");

    logMessage(DEBUG, "Added players to Troups");
    vector<Player *> _players;
    _players.push_back(humanPlayer);
    _players.push_back(aggressivePlayer);
    _players.push_back(benevolentPlayer);
    _players.push_back(neutralPlayer);
    _players.push_back(cheaterPlayer);

    logMessage(INFO, "Distributiing Territories...");
    gameMap->distributeTerritories(_players);
    cout << endl;
    // Give each player some refinforcements and cards to get the game going
    for (Player *player : _players)
    {

        player->addReinforcements(10);
        logMessage(COMBAT, "Added " + to_string(player->getReinforcementPool()) + " Reinforcements to " + player->getPlayerName());

        // Give each player 3 starting cards:
        for (int i = 0; i < 3; i++)
        {
            gameDeck->draw(*player, *player->getHandOfCards());
        }

        logMessage(DEBUG, player->getPlayerName() + " has " + to_string(player->getReinforcementPool()) + " reinforcements");
        logMessage(DEBUG, player->getPlayerName() + " owns " + to_string(player->getTerritories().size()) + " territories");
        logMessage(DEBUG, player->getPlayerName() + " has " + to_string(player->getHandOfCards()->size()) + " cards");

        // Display first territories owned by player:
        if (player->getTerritories().size() > 0)
        {
            logMessage(DEBUG, "First territory: " + player->getTerritories()[0]->getName() + " with " + to_string(player->getTerritories()[0]->getArmies()) + " armies");
        }

        cout << endl;
    }
    logMessage(PROGRESSION, "Part 2: Demonstrating dynamic strategy change");
    logMessage(DEBUG, "Neutral player is attacked! Becoming aggressive. . .");
    neutralPlayer->setStrategy(new AggressivePlayerStrategy());
    logMessage(DEBUG, "Benevolent player changes to neutral strategy. . .");
    benevolentPlayer->setStrategy(new NeutralPlayerStrategy());

    cout << endl;

    logMessage(PROGRESSION, "Part 3: Demonstrating different behaviors");

    logMessage(COMBAT, "Setting 7 armies to " + to_string(gameMap->getTerritoriesSize()) + " territories of the GameMap");
    for (int i = 0; i < gameMap->getTerritoriesSize(); i++)
    {
        Territory *t = gameMap->getTerritoryByIndex(i);
        if (t)
        {
            t->setArmies(7); // setting 7 armies to the territories;
        }
    }
    bool continuePlaying = true;
    int orderCount = 0;
    const int MAX_COUNT = 2; // Number of rounds
    bool isCheaterPlayer = false;
    int territoriesBefore;
    int totalArmiesBefore;

    while (orderCount < MAX_COUNT)
    {
        for (Player *player : _players)
        { // turn by turn
            logMessage(INFO, "=== " + player->getPlayerStrategyName() + " Player (" + player->getPlayerName() + ") ===");
            // dynamic casting during gameplay:
            if (dynamic_cast<HumanPlayerStrategy *>(player->getStrategy()) == nullptr)
            { // its not human player
                logMessage(INFO, "Current Player Strategy: " + player->getPlayerStrategyName());
                logMessage(INPUT, "Would you like to change player strategy [C/A/N]? Hit 'Enter' to continue.");
                cout << " C: Cheater" << endl;
                cout << " A: Aggressive" << endl;
                cout << " N: Neutral" << endl;

                string changeStrategy;
                getline(cin, changeStrategy);

                if (changeStrategy.empty())
                {
                    goto SKIP_PROCESS;
                }
                // user gave input → process it
                if (changeStrategy == "C" || changeStrategy == "c")
                    player->setStrategy(new CheaterPlayerStrategy());
                else if (changeStrategy == "A" || changeStrategy == "a")
                    player->setStrategy(new AggressivePlayerStrategy());
                else if (changeStrategy == "N" || changeStrategy == "n")
                    player->setStrategy(new NeutralPlayerStrategy());
            SKIP_PROCESS:;
            }

            // Special Case for the cheater player:
            if (dynamic_cast<CheaterPlayerStrategy *>(player->getStrategy()) != nullptr)
            {
                isCheaterPlayer = true;
                logMessage(INFO, "Behavior: Automatically doubles armies in owned territories");
                territoriesBefore = player->getTerritories().size();
                totalArmiesBefore = 0;
                for (Territory *t : player->getTerritories())
                {
                    totalArmiesBefore += t->getArmies();
                }
            }
            continuePlaying = player->issueOrder(gameMap, gameDeck);

            if (isCheaterPlayer)
            {
                int totalArmiesAfter = 0;
                for (Territory *t : player->getTerritories())
                {
                    totalArmiesAfter += t->getArmies();
                }
                logMessage(ANTICHEAT, "Territories owned: " + to_string(territoriesBefore));
                logMessage(ANTICHEAT, "Total armies before cheating: " + to_string(totalArmiesBefore));
                logMessage(ANTICHEAT, "Total armies after cheating: " + to_string(totalArmiesAfter));
                isCheaterPlayer = false;
            }

            logMessage(INFO, "Total orders issued: " + to_string(player->getOrdersList()->size()));
            // Show Order list
            if (player->getOrdersList()->size() > 0)
            {
                logMessage(INFO, "Order list for player: " + player->getPlayerName());
                for (size_t t = 0; t < player->getOrdersList()->size(); t++)
                {
                    Order *order = player->getOrdersList()->get(t);
                    cout << "  - " << *order << endl; // Use stream operator instead
                }
            }

            // logMessage(PROGRESSION, "\nExecuting all orders for turn " + to_string(turn));
            OrdersList *orders = player->getOrdersList();
            while (orders->size() > 0)
            {
                Order *order = orders->get(0);
                order->execute();
                orders->remove(0);
            }
        }
        orderCount++;
    }

    // clear memory
    for (Player *player : _players)
    {
        delete player;
    }
    _players.clear();
    delete gameMap;
    delete mapLoader;
    delete gameDeck;

    // Clean up global logger at program end
    LogObserver::destroyInstance();

    logMessage(INFO, "PLAYER STRATEGIES TEST COMPLETE");
}
/*
int main()
{
    testPlayerStrategies();

    return 0;
}*/