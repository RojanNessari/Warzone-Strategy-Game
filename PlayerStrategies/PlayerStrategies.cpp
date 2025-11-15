#include "PlayerStrategies.h"
#include "../Models/Player.h"
#include "../Models/Map.h"
#include "../Models/Orders.h"
#include "../Models/Cards.h"
#include "../utils/logger.h"
#include <algorithm>
#include <limits>

using namespace std;

enum class StrategyName : char
{
    HUMAN,
    AGGRESSIVE,
    BENEVOLENT,
    NEUTRAL,
    CHEATER
};

const char *toStringStrategy(StrategyName n)
{
    switch (n)
    {
    case StrategyName::HUMAN:
        return "Human";
    case StrategyName::AGGRESSIVE:
        return "Aggressive";
    case StrategyName::BENEVOLENT:
        return "Benevolent";
    case StrategyName::NEUTRAL:
        return "Neutral";
    case StrategyName::CHEATER:
        return "Cheater";
    default:
        return "Human";
    }
}

// ==================== HUMAN PLAYER STRATEGY ====================

HumanPlayerStrategy::HumanPlayerStrategy() {}
HumanPlayerStrategy::~HumanPlayerStrategy() {}

string HumanPlayerStrategy::getStrategyName() const
{
    return toStringStrategy(StrategyName::HUMAN);
}

bool HumanPlayerStrategy::issueOrder(Player *player, Map *map)
{
    logMessage("INFO", player->getPlayerName() + "'s Turn (" + getStrategyName() + " Strategy");
    logMessage("INFO", "Reinforcement pool: " + player->getReinforcementPool());

    cout << "\nWhat would you like to do?" << endl;
    cout << "1. Deploy armies" << endl;
    cout << "2. Advance armies" << endl;
    cout << "3. Play a card" << endl;
    cout << "4. Done issuing orders" << endl;
    cout << "Enter choice: ";
    try
    {
        int choice;
        if (!(cin >> choice))
        {
            logMessage("WARNING", "You entered incorrect input. Proceeding with option 4");
            throw runtime_error("Invalid Input. Proceeding with option number 4");
        }
        if (choice < 1 || choice > 4)
        {
            logMessage("WARNING", "You entered incorrect number. Proceeding with option 4");
            return false;
        }
        if (choice == 4)
        {
            logMessage("INFO", player->getPlayerName() + "Done issuing orders");
            return false; // Done issuing orders
        }

        // Processing input phase:

        if (choice == 1)
        {
            vector<Territory *> territories = player->getTerritories();
            for (size_t i = 0; i < territories.size(); i++)
            {
                cout << i + 1 << ". " << territories[i]->getName()
                     << " (armies: " << territories[i]->getArmies() << ")" << endl;
            }
            logMessage("INFO", "Select territory to deploy to (1 - " + territories.size() + "): ");
            int territoryChoice;
            cin >> territoryChoice;
            if (territoryChoice < 1 || territoryChoice > territories.size())
            {
                logMessage("WARNING", "Invalid Choice!");
                return true;
            }

            logMessage("INFO", "How many armies to deploy? (available: " + player->getReinforcementPool() + "): ");
            int armies;
            cin >> armies;
            if (armies > player->getReinforcementPool() || armies < 1)
            {
                logMessage("WARNING", "Invalid number of armies!");
                return true;
            }

            Territory *target = territories(territoryChoice - 1);
            player->getOrdersList()->add(new Deploy(player, target, armies));
            player->takeFromReinforcement(armies);
            logMessage("INFO", "Deploy order created!");
        }
        else if (choice == 2)
        {
            // Advance Order
            vector<Territory *> territories = player->getTerritories();
            logMessage("Your Territories: ");
            for (size_t i = 0; i < territories.size(); i++)
            {
                cout << i + 1 << ". " << territories[i]->getName()
                     << " (armies: " << territories[i]->getArmies() << ")" << endl;
            }
            logMessage("INFO", "Select source territory: ");
            int sourceChoice;
            cin >> sourceChoice;

            if (sourceChoice < 1 || sourceChoice > territories.size())
            {
                logMessage("WARNING", "Invalid choice");
                return true;
            }
            Territory *source = territories(sourceChoice - 1);
            vector<Territory *> neighbors = map->getNeighborsOf(source);

            logMessage("INFO", "Neighboring territories");
            for (size_t i = 0; i < neighbors.size(); i++)
            {
                cout << i + 1 << ". " << neighbors[i]->getName()
                     << " (armies: " << neighbors[i]->getArmies() << ")" << endl;
            }

            logMessage("INFO", "Select target territory: ");
            int targetChoice;
            cin >> targetChoice;

            if (targetChoice < 1 || targetChoice > neighbors.size())
            {
                logMessage("WARNING", "Invalid Choice!");
                return true;
            }

            logMessage("INFO", );
        }
    }
    catch (const exception &e)
    {
        logMessage("DEBUG", "Human player did something unexpected: " + e.what());
        cin.clear();
        return false;
    }
}