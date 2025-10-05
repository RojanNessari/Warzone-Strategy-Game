#include "Models/GameEngine.h"
#include <iostream>
#include <string>

void testGameStates()
{
    GameEngine ge; // create the engine
    ge.buildGraph();

    // Print the menu using Raw String literal R
    std::cout << R"(=== Part 5: Game Engine ===
Valid commands:
  1) loadmap          2) validatemap     3) addplayer
  4) assigncountries  5) play            6) issueorder
  7) endissueorders   8) execorder       9) endexecorders
 10) win             11) end

Type 'quit' to exit.
)";

    std::string cmd; // user input buffer
    while (true)
    {
        std::cout << "\nCurrent: " << ge << "\n> "; // show current state and prompt
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

        ge.applyCommand(cmd); // engine validates all other transitions
    }
}
