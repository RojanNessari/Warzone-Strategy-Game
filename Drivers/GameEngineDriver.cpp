#include "GameEngineDriver.h"
#include "../Models/GameEngine.h"
#include <iostream>
using namespace std;

void testStartupPhase() {
    cout << "=== Testing Game Startup Phase (Part 2) ===\n";
    GameEngine game;
    game.buildGraph();
    game.startupPhase();
}

int main() {
    testStartupPhase();
    return 0;
}
