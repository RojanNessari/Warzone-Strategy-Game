#include <iostream>
#include <vector>
#include <string>
#include "Map/Map.h"
#include "Player/Player.h"
#include "PlayerStrategies/PlayerStrategies.h"
#include "Cards/Cards.h"

using namespace std;

void printResult(bool condition, const string& msg) {
    cout << (condition ? "[PASS] " : "[FAIL] ") << msg << endl;
}

Territory* createTerritory(const string& name, int id, int continentId) {
    return new Territory(name, id, continentId, 0, 0);
}

void testPlayerStrategies() {
    cout << "\n        STARTING PLAYER TESTING         " << endl;

    // Create dummy map
    Map* map = new Map();

    // Create territories
    Territory* t1 = createTerritory("Alpha", 1, 1);
    Territory* t2 = createTerritory("Beta", 2, 1);
    Territory* t3 = createTerritory("Gamma", 3, 1);

    // adjacency links
    t1->addAdjacentTerritory(2);
    t2->addAdjacentTerritory(1);
    t2->addAdjacentTerritory(3);
    t3->addAdjacentTerritory(2);

    // Add to map
    map->addTerritory(t1);
    map->addTerritory(t2);
    map->addTerritory(t3);

    // Create a deck 
    Deck* deck = new Deck();

    //Create players
    Player* aggressive = new Player("Aggressive");
    Player* benevolent = new Player("Benevolent");
    Player* neutral = new Player("Neutral");
    Player* cheater = new Player("Cheater");

    // Give each player territories for testing
    aggressive->addTerritory(t1);
    benevolent->addTerritory(t2);
    neutral->addTerritory(t3);

    t1->setOwner(aggressive);
    t2->setOwner(benevolent);
    t3->setOwner(neutral);

    //STRATEGY TESTS

    cout << "\nTesting Aggressive Strategy\n";
    aggressive->setStrategy(new AggressivePlayerStrategy());
    printResult(!aggressive->toDefend().empty(), "Aggressive should defend strongest territory.");
    printResult(!aggressive->toAttack(map).empty(), "Aggressive should identify adjacent enemies.");
    printResult(aggressive->issueOrder(map, deck), "Aggressive should issue an order successfully.");

    cout << "\nTesting Benevolent Strategy\n";
    benevolent->setStrategy(new BenevolentPlayerStrategy());
    printResult(!benevolent->toDefend().empty(), "Benevolent should defend weakest territory.");
    printResult(!benevolent->toAttack(map).empty() == false, "Benevolent should rarely attack.");
    printResult(benevolent->issueOrder(map, deck), "Benevolent should issue an order successfully.");

    cout << "\nTesting Neutral Strategy\n";
    neutral->setStrategy(new NeutralPlayerStrategy());
    printResult(neutral->toAttack(map).empty(), "Neutral should not attack normally.");
    printResult(!neutral->toDefend().empty(), "Neutral should list its territories to defend.");
    printResult(neutral->issueOrder(map, deck), "Neutral should issue no-aggression orders.");

    cout << "\nTesting Cheater Strategy\n";
    cheater->setStrategy(new CheaterPlayerStrategy());

    // Give cheater one territory and an adjacent enemy 
    Territory* cheatT = createTerritory("Delta", 4, 1);
    cheatT->addAdjacentTerritory(1);
    map->addTerritory(cheatT);
    cheater->addTerritory(cheatT);
    cheatT->setOwner(cheater);

    printResult(!cheater->toAttack(map).empty(), "Cheater should always find territories to instantly conquer.");
    printResult(cheater->issueOrder(map, deck), "Cheater should perform instant conquer.");

    cout << "      PLAYER STRATEGIES TEST DONE       " << endl;

    // Cleanup
    delete map;
    delete deck;
    delete aggressive;
    delete benevolent;
    delete neutral;
    delete cheater;
}
