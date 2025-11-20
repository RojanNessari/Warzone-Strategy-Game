/*-----------------------------------------------------------------------------
  File: MainDriver.cpp
  Course: COMP 345
  Assignment: A1 (Parts 1–5)
  Authors: Adja Bineta Boye (ID: 40281923), Ali Sher (ID: 40255236), Achraf Cheniti (ID: 40244865)
           Joseph El Bitar (ID: 40270590), Rojan Nessari (ID: 40255637)
  Date: 2025-11-11
-----------------------------------------------------------------------------*/

#include <iostream>
#include "Drivers/GameEngineDriver.h"
#include "Drivers/CardsDriver.h"
#include "Drivers/OrdersDriver.h"
#include "Drivers/MapDriver.h"
#include "Drivers/PlayerDriver.h"
#include "Drivers/CommandProcessingDriver.h"
#include "Drivers/LoggingObserverDriver.h"
#include "utils/LoggingObserver.h"

#include <string>

using namespace std;

void Assignment_01_Menu()
{ /*
    Purpose:
    Single entry point for the marker. Prints a menu and dispatches to:
        1) testLoadMaps()      // Map + MapLoader + validation
        2) testPlayers()       // Player: toDefend/toAttack/issueOrder
        3) testOrdersLists()   // Orders/OrdersList: validate/execute/move/remove
        4) testCards()         // Deck/Hand/Card: draw/play/return
        5) testGameStates()    // Game engine state machine + commands

    Usage:
    Run, then enter 1–5 or 'quit'.
  */
    string cmd;
    while (true)
    {
        // Display menu as written
        cout << R"(
╔══════════════════════════════════════════════════════╗
║                COMP-345 Assignment-01                ║
╠══════════════════════════════════════════════════════╣
║                                                      ║
║  1) Run testLoadMaps()    - Part 1: Map Module       ║
║  2) Run testPlayers()     - Part 2: Player Module    ║
║  3) Run testOrdersLists() - Part 3: Orders Module    ║
║  4) Run testCards()       - Part 4: Cards Module     ║
║  5) Run testGameStates()  - Part 5: Game Engine      ║
║                                                      ║
║  Type "quit" to exit                                 ║
║                                                      ║
╚══════════════════════════════════════════════════════╝
    )";

        cout << "\n Enter your choice (1-5) or 'quit': ";
        if (!getline(cin, cmd)) // Get the users input
            break;

        if (cmd == "quit")
        {
            cout << " Goodbye! Thanks for using Comp-345-Assignment-01!" << endl;
            break;
        }

        if (cmd == "1")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Map Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testLoadMaps(); // Part 1 tests
            cout << "\n Map Module Test Completed!\n";
        }
        else if (cmd == "2")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Player Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testPlayers(); // Part 2 tests
            cout << "\n Player Module Test Completed!\n";
        }
        else if (cmd == "3")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << " Running Orders Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testOrdersLists(); // Part 3 tests
            cout << "\n Orders Module Test Completed!\n";
        }
        else if (cmd == "4")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << " Running Cards Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testCards(); // Part 4 tests
            cout << "\n Cards Module Test Completed!\n";
        }
        else if (cmd == "5")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Game Engine Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testGameStates(); // Part 5 tests
            cout << "\n Game Engine Module Test Completed!\n";
        }
        else
        {
            // Any other input is rejected; loop continues
            cout << " Invalid input! Please enter a number from 1-5 or 'quit'.\n";
        }
    }
}

void Assignment_02_Menu()
{
    string cmd;

    while (true)
    {
        // Display menu as written
        cout << R"(
╔══════════════════════════════════════════════════════════════════════════════════╗
║                           COMP-345 Assignment-02                                 ║
╠══════════════════════════════════════════════════════════════════════════════════╣
║                                                                                  ║
║  1) Run testCommandProcessor()   - Part 1: Command processor and command adapter ║
║  2) Run testStartupPhase()       - Part 2: Game startup phase                    ║
║  3) Run testMainGameLoop()       - Part 3: Game play: main game loop             ║
║  4) Run testOrderExecution()     - Part 4: Order execution implementation        ║
║  5) Run testLoggingObserver()    - Part 5: Game log observer: commands and orders║
║                                                                                  ║
║  Type "quit" to exit                                                             ║
║                                                                                  ║
╚══════════════════════════════════════════════════════════════════════════════════╝
    )";

        cout << "\nEnter your choice (1-5) or 'quit': " << endl;
        if (!getline(cin, cmd)) // Get the users input
            break;

        if (cmd == "1")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Command processor and command adapter...\n";
            cout << " ═══════════════════════════════════════════════════\n";

            // Part 1 requires choosing between console or file mode
            cout << "\n Choose input mode:\n";
            cout << "   1) Console mode (-console)\n";
            cout << "   2) File mode (-file <filename>)\n";
            cout << " Enter choice (1 or 2): ";

            string modeChoice;
            getline(cin, modeChoice);

            if (modeChoice == "1")
            {
                setMode("-console");
                cout << " Running in CONSOLE mode...\n";
                testCommandProcessor();
            }
            else if (modeChoice == "2")
            {
                cout << " Enter filename (e.g., cmdProcessingTestFiles/test_commands.txt): ";
                string filename;
                getline(cin, filename);
                setMode("-file");
                setFileName(filename);
                cout << " Running in FILE mode with: " << filename << "\n";
                testCommandProcessor();
            }
            else
            {
                cout << " Invalid choice! Skipping test.\n";
            }

            cout << "\n Command Processor/Adapter Completed!\n";
        }
        else if (cmd == "2")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Game startup phase Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testStartupPhase(); // Part 2 tests
            cout << "\n StartupPhase Test Completed!\n";
        }
        else if (cmd == "3")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << " Running Game play: main game loop Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testMainGameLoop(); // Part 3 tests
            cout << "\n Main game loop Test Completed!\n";
        }
        else if (cmd == "4")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << " Running Order execution implementation Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testOrderExecution(); // Part 4 tests
            cout << "\n Order execution Test Completed!\n";
        }
        else if (cmd == "5")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Game log observer: commands and orders Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testLoggingObserver(); // Part 5 tests
            cout << "\n Game log observer Test Completed!\n";
        }
        else if (cmd == "quit")
        {
            cout << " Goodbye! Thanks for using Comp-345-Assignment-02!" << endl;
            break;
        }
        else
        {
            // Any other input is rejected; loop continues
            cout << " Invalid input! Please enter a number from 1-5 or 'quit'.\n";
        }
    }
}

int main()
{
    string cmd;
    cout << "Run Assignment-01 ? [Y/n]" << endl;
    getline(cin, cmd);
     static LogObserver globalLogger;
    Subject::Attach(&globalLogger);
    if (cmd == "y" || cmd == "Y")
    {
        Assignment_01_Menu();
        Assignment_02_Menu();
    }

    if (cmd == "n" || cmd == "N")
    {
        Assignment_02_Menu();
    }

    return 0;
}