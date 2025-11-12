#include <iostream>
#include "../Models/Orders.h"
#include "../Models/Player.h"
#include "../Models/Map.h"
#include "../Models/GameEngine.h"
#include "../Models/Cards.h"
using namespace std;

const string MOCK_PLAYER_NAME_1 = "n00b69";
const string MOCK_PLAYER_NAME_2 = "FLORA78";

void testOrdersLists()
{
    OrdersList list;

    list.add(new Deploy());
    list.add(new Advance());
    list.add(new Bomb());
    list.add(new Blockade());
    list.add(new Airlift());
    list.add(new Negotiate());

    cout << " Initial Orders:" << endl;
    list.print();

    cout << "\n Moving order 2 (Bomb) to the top..." << endl;
    list.move(2, 0);
    list.print();

    cout << "\n Removing order 3..." << endl;
    list.remove(3);
    list.print();

    cout << "\n Executing all orders..." << endl;
    for (size_t i = 0; i < list.size(); i++)
    {
        Order *o = list.get((int)i);
        if (o && o->validate())
        {
            o->execute();
        }
    }
    list.print();
}

void testOrderExecution()
{
    cout << "Testing Order Execution (Part 4)" << endl;

    // Setup test environment
    GameEngine *engine = new GameEngine();
    Player *player1 = new Player(MOCK_PLAYER_NAME_1);
    Player *player2 = new Player(MOCK_PLAYER_NAME_2);
    Player *neutral = engine->getNeutralPlayer();

    player1->setId(1);
    player2->setId(2);
    player1->setReinforcementPool(50);
    player2->setReinforcementPool(50);

    // Create test territories
    Territory *t1 = new Territory("Territory1", 1, 1, 0, 0);
    Territory *t2 = new Territory("Territory2", 2, 1, 0, 0);
    Territory *t3 = new Territory("Territory3", 3, 1, 0, 0);
    Territory *t4 = new Territory("Territory4", 4, 1, 0, 0);

    t1->setOwner(player1);
    t2->setOwner(player2);
    t3->setOwner(player1);
    t4->setOwner(player2);
    t1->setArmies(10);
    t2->setArmies(5);
    t3->setArmies(8);
    t4->setArmies(6);

    // Make territories adjacent
    t1->addAdjacentTerritory(2);
    t1->addAdjacentTerritory(3);
    t1->addAdjacentTerritory(4);

    t2->addAdjacentTerritory(1);
    t2->addAdjacentTerritory(4);

    t3->addAdjacentTerritory(1);

    t4->addAdjacentTerritory(1);
    t4->addAdjacentTerritory(2);

    // Add territories to players
    player1->addTerritory(t1);
    player1->addTerritory(t3);
    player2->addTerritory(t2);
    player2->addTerritory(t4);

    cout << "\n1. Testing Deploy Order:" << endl;
    Deploy deploy(player1, t1, 5);
    deploy.execute();
    cout << "Effect: " << deploy.getEffect() << endl;
    cout << "Reinforcement pool after deploy: " << player1->getReinforcementPool() << endl;

    cout << "\n2. Testing Invalid Deploy Order:" << endl;
    Deploy invalidDeploy(player1, t2, 5); // t2 not owned by player1
    invalidDeploy.execute();
    cout << "Effect: " << invalidDeploy.getEffect() << endl;

    cout << "\n3. Testing Advance Order (Friendly Move):" << endl;
    Advance advanceFriendly(player1, t1, t3, 3);
    advanceFriendly.execute();
    cout << "Effect: " << advanceFriendly.getEffect() << endl;
    cout << "Source armies: " << t1->getArmies() << ", Target armies: " << t3->getArmies() << endl;

    cout << "\n4. Testing Advance Order (Attack):" << endl;
    cout << "Before attack - Owner of t2: Player" << t2->getOwner()->getId() << endl;
    cout << "Before attack - Armies in t1: " << t1->getArmies() << ", t2: " << t2->getArmies() << endl;

    Advance advanceAttack(player1, t1, t2, 8);
    advanceAttack.execute();
    cout << "Effect: " << advanceAttack.getEffect() << endl;
    cout << "After attack - Owner of t2: Player" << t2->getOwner()->getId() << endl;
    cout << "Conquered this turn: " << player1->hasConqueredThisTurn() << endl;

    cout << "\n5. Testing Bomb Order:" << endl;
    cout << "Before bomb - Armies in t4: " << t4->getArmies() << endl;
    Bomb bomb(player1, t4, nullptr); // Map not needed for this simple test
    bomb.execute();
    cout << "Effect: " << bomb.getEffect() << endl;
    cout << "After bomb - Armies in t4: " << t4->getArmies() << endl;

    cout << "\n6. Testing Invalid Bomb Order:" << endl;
    Bomb invalidBomb(player1, t1, nullptr); // Can't bomb own territory
    invalidBomb.execute();
    cout << "Effect: " << invalidBomb.getEffect() << endl;

    cout << "\n7. Testing Blockade Order:" << endl;
    cout << "Before blockade - Owner of t3: Player" << t3->getOwner()->getId() << endl;
    cout << "Before blockade - Armies in t3: " << t3->getArmies() << endl;

    Blockade blockade(player1, t3, engine);
    blockade.execute();
    cout << "Effect: " << blockade.getEffect() << endl;
    cout << "After blockade - Owner of t3: Player" << t3->getOwner()->getId() << endl;
    cout << "After blockade - Armies in t3: " << t3->getArmies() << endl;

    cout << "\n8. Testing Airlift Order:" << endl;
    cout << "Before airlift - Armies in t1: " << t1->getArmies() << ", t3: " << t3->getArmies() << endl;

    // Reset t3 ownership to player1 for airlift test
    t3->setOwner(player1);
    Airlift airlift(player1, t1, t3, 4);
    airlift.execute();
    cout << "Effect: " << airlift.getEffect() << endl;
    cout << "After airlift - Armies in t1: " << t1->getArmies() << ", t3: " << t3->getArmies() << endl;

    cout << "\n9. Testing Negotiate Order:" << endl;
    Negotiate negotiate(player1, player2, engine);
    negotiate.execute();
    cout << "Effect: " << negotiate.getEffect() << endl;

    // Test that truce prevents attacks
    cout << "\n10. Testing Truce Prevents Attacks:" << endl;
    Advance attackDuringTruce(player1, t1, t4, 3);
    // The attack should be prevented by the truce (this would be handled in validate())
    cout << "Truce between player1 and player2: " << engine->isTruced(player1, player2) << endl;

    cout << "\n11. Testing Card Reward System:" << endl;
    player1->resetConqueredFlag();
    cout << "Before conquest - conqueredThisTurn: " << player1->hasConqueredThisTurn() << endl;

    // Simulate a conquest
    player1->markConqueredThisTurn();
    cout << "After conquest - conqueredThisTurn: " << player1->hasConqueredThisTurn() << endl;

    cout << "\n12. Testing OrdersList Functionality:" << endl;
    OrdersList ordersList;
    ordersList.add(new Deploy(player1, t1, 3));
    ordersList.add(new Advance(player1, t1, t2, 2));
    ordersList.add(new Bomb(player1, t4, nullptr));

    cout << "Orders in list: " << ordersList.size() << endl;
    ordersList.print();

    cout << "\nMoving order 0 to position 2:" << endl;
    ordersList.move(0, 2);
    ordersList.print();

    cout << "\nRemoving order at position 1:" << endl;
    ordersList.remove(1);
    ordersList.print();

    // Cleanup
    delete engine;
    delete player1;
    delete player2;
    delete t1;
    delete t2;
    delete t3;
    delete t4;

    cout << "\nAll Order Execution Tests Complete" << endl;
}

/*
int main()
{
    testOrderExecution();
    return 0;
}
*/
