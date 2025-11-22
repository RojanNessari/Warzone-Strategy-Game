#include <iostream>
#include <vector>
#include "../Models/GameEngine.h"

void testTournament() {
    std::cout << "=== Testing Tournament Mode with Player Strategies ===" << std::endl;
    
    GameEngine engine;
    engine.buildGraph();
    
    // Test with available map files
    std::vector<std::string> testMaps = {
        "Maps/alberta.map"  
    };
    
    // Player strategies that should exhibit different behaviors
    std::vector<std::string> testStrategies = {
        "Aggressive", "Benevolent", "Cheater"
    };
    
    int testGames = 2;
    int testTurns = 20;
    
    std::cout << "Testing tournament with implemented player strategies:" << std::endl;
    std::cout << "- Aggressive: Focuses on attacking and conquering" << std::endl;
    std::cout << "- Benevolent: Focuses on defending weak territories" << std::endl;
    std::cout << "- Cheater: Automatically conquers adjacent territories" << std::endl << std::endl;
    
    std::cout << "Tournament Parameters:" << std::endl;
    std::cout << "- Maps: ";
    for (const auto& map : testMaps) std::cout << map << " ";
    std::cout << std::endl << "- Strategies: ";
    for (const auto& strategy : testStrategies) std::cout << strategy << " ";
    std::cout << std::endl << "- Games: " << testGames;
    std::cout << std::endl << "- Max Turns: " << testTurns << std::endl;
    
    try {
        engine.runTournament(testMaps, testStrategies, testGames, testTurns);
        std::cout << "Tournament completed successfully with strategy behaviors!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Tournament error: " << e.what() << std::endl;
    }
    
    std::cout << "=== Tournament Test Complete ===" << std::endl;
}

// Optional: Test different strategy combinations
void testTournamentStrategies() {
    std::cout << "\n=== Testing Different Strategy Combinations ===" << std::endl;
    
    GameEngine engine;
    engine.buildGraph();
    
    // Test 1: Aggressive vs Benevolent
    std::cout << "Test 1: Aggressive vs Benevolent" << std::endl;
    engine.runTournament(
        {"Maps/alberta.map"}, 
        {"Aggressive", "Benevolent"}, 
        1,  // 1 game
        15  // 15 turns
    );
    
    // Test 2: All strategy types
    std::cout << "\nTest 2: All Strategy Types" << std::endl;
    engine.runTournament(
        {"Maps/alberta.map"},
        {"Aggressive", "Benevolent", "Neutral", "Cheater"},
        1,  // 1 game
        15  // 15 turns
    );
    
    // Test 3: Cheater strategy dominance test
    std::cout << "\nTest 3: Cheater Strategy Test" << std::endl;
    engine.runTournament(
        {"Maps/alberta.map"},
        {"Cheater", "Aggressive", "Benevolent"},
        1,  // 1 game
        10  // 10 turns
    );
}

int main() {
    testTournament();
    
    // Uncomment to test additional strategy combinations
    // testTournamentStrategies();
    
    return 0;
}