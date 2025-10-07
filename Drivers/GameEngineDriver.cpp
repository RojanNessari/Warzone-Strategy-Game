#include "../Models/GameEngine.h"
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

         // universal exit from Part 5
        if (cmd == "quit" || cmd == "end") {
            std::cout << "Terminating.\n";
            break;
        }

        ge.applyCommand(cmd); // engine validates all other transitions

        
    }
}


/*

Demo scripts for Part 5 (Game Engine) — paste anywhere as a reference comment.
Usage:

Run the program.

At the main menu, type: 5

Then enter the commands below at the ">" prompt in order.

A) HAPPY PATHS
A1 — Straight to a win
loadmap
validatemap
addplayer
assigncountries
issueorder
endissueorders
execorder
win
end

A2 — One full loop, then win
loadmap
validatemap
addplayer
assigncountries
issueorder
issueorder
endissueorders
execorder
execorder
endexecorders
issueorder
endissueorders
execorder
win
end

A3 — From win back to play loop
loadmap
validatemap
addplayer
assigncountries
issueorder
endissueorders
execorder
win
play
issueorder
endissueorders
execorder
win
end

B) SELF-LOOPS / IDEMPOTENT EDGES
B1 — Re-load map while in map_loaded
loadmap
loadmap
loadmap
validatemap
addplayer
assigncountries
issueorder
endissueorders
execorder
win
end

B2 — Add multiple players in players_added
loadmap
validatemap
addplayer
addplayer
addplayer
assigncountries
issueorder
endissueorders
execorder
win
end

B3 — Issue multiple orders in issue_orders
loadmap
validatemap
addplayer
assigncountries
issueorder
issueorder
issueorder
endissueorders
execorder
win
end

B4 — Execute multiple orders in execute_orders
loadmap
validatemap
addplayer
assigncountries
issueorder
endissueorders
execorder
execorder
execorder
win
end

C) INVALID TRANSITIONS (expect "Invalid command from state...")
C1 — 'play' too early
loadmap
validatemap
addplayer
assigncountries
play // invalid here
issueorder
endissueorders
execorder
win
end

C2 — Execute before issuing
loadmap
validatemap
addplayer
assigncountries
execorder // invalid here
issueorder
endissueorders
execorder
win
end

C3 — End issue phase before it starts
loadmap
validatemap
addplayer
assigncountries
endissueorders // invalid here
issueorder
endissueorders
execorder
win
end

D) QUICK EXITS (with current harness)
// Exit immediately:
end
// or:
quit

// Exit mid-loop:
loadmap
validatemap
addplayer
assigncountries
issueorder
end
*/