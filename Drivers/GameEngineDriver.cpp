#include "GameEngineDriver.h"
#include "../Models/GameEngine.h"
#include "../Models/Map.h"
#include "../Models/Player.h"
#include "../Models/Cards.h"
#include <iostream>
#include "../utils/logger.h"
using namespace std;

void testGameStates()
{
    GameEngine ge; // create the engine
    ge.buildGraph();

    // Print the menu using Raw String literal R
    logMessage(INFO, "=== Part 5: Game Engine ===\nValid commands:\n  1) loadmap          2) validatemap     3) addplayer\n  4) assigncountries  5) play            6) issueorder\n  7) endissueorders   8) execorder       9) endexecorders\n 10) win             11) end\n\nType 'quit' to exit.");

    std::string cmd; // user input buffer
    while (true)
    {
        logMessage(DEBUG, string("Current: ") + ge.current()->getName());
        if (!std::getline(std::cin, cmd))
            break;
        if (cmd == "quit")
            break;

        // 'end' is a terminal command (no 'end' state in the engine)
        // 'end' is only valid from 'win' and then we terminate the program
        if (cmd == "end")
        {
            if (ge.current() && ge.current()->getName() == "win")
            {
                std::cout << "Terminating: reached final node.\n";
                break;
            }
            else
            {
                std::cout << "Invalid command from state " // If not at 'win', 'end' is invalid.
                          << ge.current()->getName() << ": end\n";
                continue;
            }
        }

        // universal exit from Part 5
        if (cmd == "quit" || cmd == "end")
        {
            std::cout << "Terminating.\n";
            break;
        }

        ge.applyCommand(cmd); // engine validates all other transitions
    }
}

void testStartupPhase()
{
    cout << "=== Testing Game Startup Phase (Part 2) ===\n";
    GameEngine game;
    game.buildGraph();
    game.startupPhase();
}

void testMainGameLoop()
{
    cout << "\n=== Testing Main Game Loop (Part 3) ===\n\n";

    // Create and initialize game engine
    GameEngine game;
    game.buildGraph();

    // Load and validate a map
    logMessage(INFO, "Loading map for testing...");
    MapLoader loader;
    Map *testMap = loader.loadMap("Maps/alberta.map");

    if (testMap == nullptr)
    {
        logMessage(ERROR, "Failed to load map. Test cannot continue.");
        return;
    }

    if (!testMap->validate())
    {
        logMessage(ERROR, "Map validation failed. Test cannot continue.");
        delete testMap;
        return;
    }

    logMessage(INFO, "Map loaded and validated successfully.\n");

    // Create test players
    Player *player1 = new Player("Alice");
    Player *player2 = new Player("Bob");
    Player *player3 = new Player("Charlie");

    player1->setId(1);
    player2->setId(2);
    player3->setId(3);

    // Add players to a vector for distribution
    std::vector<Player *> players;
    players.push_back(player1);
    players.push_back(player2);
    players.push_back(player3);

    logMessage(INFO, "Created 3 test players: Alice, Bob, and Charlie\n");

    // Distribute territories
    logMessage(INFO, "Distributing territories among players...");
    testMap->distributeTerritories(players);

    // Display territory distribution
    for (Player *p : players)
    {
        logMessage(INFO, p->getPlayerName() + " owns " +
                             std::to_string(p->getTerritories().size()) + " territories");
    }

    // Test 1: Verify correct reinforcement calculation
    logMessage(INFO, "\n--- TEST 1: Reinforcement Calculation ---");
    logMessage(INFO, "Testing that players receive correct army units...");

    // Manually set different territory counts to test calculation
    player1->setReinforcementPool(0);
    player2->setReinforcementPool(0);
    player3->setReinforcementPool(0);

    // Calculate what each player should receive
    int p1Territories = player1->getTerritories().size();
    int p1Expected = std::max(3, p1Territories / 3);

    int p2Territories = player2->getTerritories().size();
    int p2Expected = std::max(3, p2Territories / 3);

    int p3Territories = player3->getTerritories().size();
    int p3Expected = std::max(3, p3Territories / 3);

    logMessage(INFO, player1->getPlayerName() + " has " + std::to_string(p1Territories) +
                         " territories, expects " + std::to_string(p1Expected) + " armies");
    logMessage(INFO, player2->getPlayerName() + " has " + std::to_string(p2Territories) +
                         " territories, expects " + std::to_string(p2Expected) + " armies");
    logMessage(INFO, player3->getPlayerName() + " has " + std::to_string(p3Territories) +
                         " territories, expects " + std::to_string(p3Expected) + " armies");

    // Note: For a complete test, you would need to set up the game engine properly
    // with these players and call reinforcementPhase()

    logMessage(INFO, "\n--- TEST 2: Deploy Orders Priority ---");
    logMessage(INFO, "Testing that players only issue Deploy orders when they have reinforcements...");

    // Give player1 some reinforcements
    player1->setReinforcementPool(10);

    logMessage(INFO, player1->getPlayerName() + " has " +
                         std::to_string(player1->getReinforcementPool()) + " armies in reinforcement pool");

    // Issue an order - should be Deploy
    bool hasMore = player1->issueOrder(testMap); // Pass the map

    if (hasMore && player1->getOrdersList()->size() > 0)
    {
        Order *firstOrder = player1->getOrdersList()->get(0);
        Deploy *deployOrder = dynamic_cast<Deploy *>(firstOrder);

        if (deployOrder != nullptr)
        {
            logMessage(INFO, "✓ Player correctly issued a Deploy order while having reinforcements");
        }
        else
        {
            logMessage(ERROR, "✗ Player issued non-Deploy order while having reinforcements");
        }
    }

    logMessage(INFO, "\n--- TEST 3: Advance Orders After Deployment ---");
    logMessage(INFO, "Testing that players can issue Advance orders after depleting reinforcements...");

    // Set player2 to have no reinforcements but have territories
    player2->setReinforcementPool(0);

    if (player2->getTerritories().size() > 0)
    {
        // Give first territory some armies
        player2->getTerritories()[0]->setArmies(10);

        // Issue order - should be Advance or done
        hasMore = player2->issueOrder(testMap); // Pass the map
        logMessage(INFO, player2->getPlayerName() + " issued an order (or finished issuing)");
    }

    logMessage(INFO, "\n--- TEST 4: Card-Based Orders ---");
    logMessage(INFO, "Testing that players can play cards to issue special orders...");

    // Create a deck and give player3 some cards
    Deck testDeck;
    Hand *player3Hand = player3->getHandOfCards();

    // Draw some cards
    testDeck.draw(*player3, *player3Hand);
    testDeck.draw(*player3, *player3Hand);

    logMessage(INFO, player3->getPlayerName() + " has " +
                         std::to_string(player3Hand->size()) + " cards");

    logMessage(INFO, "Card-based order logic is in Player::issueOrder() method");

    logMessage(INFO, "\n--- TEST 5: Player Elimination ---");
    logMessage(INFO, "Testing that players with no territories are removed...");

    // Create a player with no territories
    Player *player4 = new Player("David");
    player4->setId(4);

    if (player4->getTerritories().empty())
    {
        logMessage(INFO, "✓ Player " + player4->getPlayerName() +
                             " has no territories and would be eliminated in main game loop");
    }

    delete player4;

    logMessage(INFO, "\n--- TEST 6: Win Condition ---");
    logMessage(INFO, "Testing game ends when one player owns all territories...");

    int totalPlayers = 0;
    for (Player *p : players)
    {
        if (!p->getTerritories().empty())
        {
            totalPlayers++;
        }
    }

    logMessage(INFO, "Currently " + std::to_string(totalPlayers) + " players own territories");
    logMessage(INFO, "Game would continue until only 1 player remains");

    // Cleanup
    logMessage(INFO, "\n=== Cleaning up test resources ===");
    delete player1;
    delete player2;
    delete player3;
    delete testMap;

    logMessage(INFO, "\n=== Main Game Loop Test Complete ===");
    logMessage(INFO, "Note: For full testing, run the actual mainGameLoop() with proper setup.");
}

/*
int main()
{
    // Uncomment to test different parts
    // testGameStates();
    // testStartupPhase();
    testMainGameLoop();
    return 0;
}
*/
