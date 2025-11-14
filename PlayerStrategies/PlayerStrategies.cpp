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
        }
    }
    catch (const exception &e)
    {
        logMessage("DEBUG", "Human player did something unexpected: " + e.what());
        cin.clear();
        return false;
    }
}