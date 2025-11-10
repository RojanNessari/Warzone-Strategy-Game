#include "GameEngineDriver.h"
#include "../Models/GameEngine.h"
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

int main()
{
    testStartupPhase();
    // testGameStates();
    return 0;
}
