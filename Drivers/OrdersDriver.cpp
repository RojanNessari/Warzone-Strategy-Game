#include <iostream>
#include "../Models/Orders.h"
#include "../Models/Player.h"
#include "../Models/Map.h"
#include "../Models/GameEngine.h"
#include "../Models/Cards.h"

void testOrdersLists()
{
    OrdersList list;

    list.add(new Deploy());
    list.add(new Advance());
    list.add(new Bomb());
    list.add(new Blockade());
    list.add(new Airlift());
    list.add(new Negotiate());

    std::cout << " Initial Orders:" << std::endl;
    list.print();

    std::cout << "\n Moving order 2 (Bomb) to the top..." << std::endl;
    list.move(2, 0);
    list.print();

    std::cout << "\n Removing order 3..." << std::endl;
    list.remove(3);
    list.print();

    std::cout << "\n Executing all orders..." << std::endl;
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

void testOrderExecution() {
    std::cout << "Testing Order Execution (Part 4)" << std::endl;
    
    // Setup test environment
    GameEngine* engine = new GameEngine();
    Player* player1 = new Player();
    Player* player2 = new Player();
    Player* neutral = engine->getNeutralPlayer();
    
    player1->setId(1);
    player2->setId(2);
    player1->setReinforcementPool(50);
    player2->setReinforcementPool(50);
    
    // Create test territories
    Territory* t1 = new Territory("Territory1", 1, 1, 0, 0);
    Territory* t2 = new Territory("Territory2", 2, 1, 0, 0);
    Territory* t3 = new Territory("Territory3", 3, 1, 0, 0);
    Territory* t4 = new Territory("Territory4", 4, 1, 0, 0);
    
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
    
    std::cout << "\n1. Testing Deploy Order:" << std::endl;
    Deploy deploy(player1, t1, 5);
    deploy.execute();
    std::cout << "Effect: " << deploy.getEffect() << std::endl;
    std::cout << "Reinforcement pool after deploy: " << player1->getReinforcementPool() << std::endl;
    
    std::cout << "\n2. Testing Invalid Deploy Order:" << std::endl;
    Deploy invalidDeploy(player1, t2, 5);  // t2 not owned by player1
    invalidDeploy.execute();
    std::cout << "Effect: " << invalidDeploy.getEffect() << std::endl;
    
    std::cout << "\n3. Testing Advance Order (Friendly Move):" << std::endl;
    Advance advanceFriendly(player1, t1, t3, 3);
    advanceFriendly.execute();
    std::cout << "Effect: " << advanceFriendly.getEffect() << std::endl;
    std::cout << "Source armies: " << t1->getArmies() << ", Target armies: " << t3->getArmies() << std::endl;
    
    std::cout << "\n4. Testing Advance Order (Attack):" << std::endl;
    std::cout << "Before attack - Owner of t2: Player" << t2->getOwner()->getId() << std::endl;
    std::cout << "Before attack - Armies in t1: " << t1->getArmies() << ", t2: " << t2->getArmies() << std::endl;
    
    Advance advanceAttack(player1, t1, t2, 8);
    advanceAttack.execute();
    std::cout << "Effect: " << advanceAttack.getEffect() << std::endl;
    std::cout << "After attack - Owner of t2: Player" << t2->getOwner()->getId() << std::endl;
    std::cout << "Conquered this turn: " << player1->hasConqueredThisTurn() << std::endl;
    
    std::cout << "\n5. Testing Bomb Order:" << std::endl;
    std::cout << "Before bomb - Armies in t4: " << t4->getArmies() << std::endl;
    Bomb bomb(player1, t4, nullptr);  // Map not needed for this simple test
    bomb.execute();
    std::cout << "Effect: " << bomb.getEffect() << std::endl;
    std::cout << "After bomb - Armies in t4: " << t4->getArmies() << std::endl;
    
    std::cout << "\n6. Testing Invalid Bomb Order:" << std::endl;
    Bomb invalidBomb(player1, t1, nullptr);  // Can't bomb own territory
    invalidBomb.execute();
    std::cout << "Effect: " << invalidBomb.getEffect() << std::endl;
    
    std::cout << "\n7. Testing Blockade Order:" << std::endl;
    std::cout << "Before blockade - Owner of t3: Player" << t3->getOwner()->getId() << std::endl;
    std::cout << "Before blockade - Armies in t3: " << t3->getArmies() << std::endl;
    
    Blockade blockade(player1, t3, engine);
    blockade.execute();
    std::cout << "Effect: " << blockade.getEffect() << std::endl;
    std::cout << "After blockade - Owner of t3: Player" << t3->getOwner()->getId() << std::endl;
    std::cout << "After blockade - Armies in t3: " << t3->getArmies() << std::endl;
    
    std::cout << "\n8. Testing Airlift Order:" << std::endl;
    std::cout << "Before airlift - Armies in t1: " << t1->getArmies() << ", t3: " << t3->getArmies() << std::endl;
    
    // Reset t3 ownership to player1 for airlift test
    t3->setOwner(player1);
    Airlift airlift(player1, t1, t3, 4);
    airlift.execute();
    std::cout << "Effect: " << airlift.getEffect() << std::endl;
    std::cout << "After airlift - Armies in t1: " << t1->getArmies() << ", t3: " << t3->getArmies() << std::endl;
    
    std::cout << "\n9. Testing Negotiate Order:" << std::endl;
    Negotiate negotiate(player1, player2, engine);
    negotiate.execute();
    std::cout << "Effect: " << negotiate.getEffect() << std::endl;
    
    // Test that truce prevents attacks
    std::cout << "\n10. Testing Truce Prevents Attacks:" << std::endl;
    Advance attackDuringTruce(player1, t1, t4, 3);
    // The attack should be prevented by the truce (this would be handled in validate())
    std::cout << "Truce between player1 and player2: " << engine->isTruced(player1, player2) << std::endl;
    
    std::cout << "\n11. Testing Card Reward System:" << std::endl;
    player1->resetConqueredFlag();
    std::cout << "Before conquest - conqueredThisTurn: " << player1->hasConqueredThisTurn() << std::endl;
    
    // Simulate a conquest
    player1->markConqueredThisTurn();
    std::cout << "After conquest - conqueredThisTurn: " << player1->hasConqueredThisTurn() << std::endl;
    
    std::cout << "\n12. Testing OrdersList Functionality:" << std::endl;
    OrdersList ordersList;
    ordersList.add(new Deploy(player1, t1, 3));
    ordersList.add(new Advance(player1, t1, t2, 2));
    ordersList.add(new Bomb(player1, t4, nullptr));
    
    std::cout << "Orders in list: " << ordersList.size() << std::endl;
    ordersList.print();
    
    std::cout << "\nMoving order 0 to position 2:" << std::endl;
    ordersList.move(0, 2);
    ordersList.print();
    
    std::cout << "\nRemoving order at position 1:" << std::endl;
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
    
    std::cout << "\nAll Order Execution Tests Complete" << std::endl;
}

int main() {
    testOrderExecution();
    return 0;
}
