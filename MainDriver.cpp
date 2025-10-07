/*-----------------------------------------------------------------------------
  File: MainDriver.cpp
  Course: COMP 345
  Assignment: A1 (Parts 1–5)
  Authors: Adja Bineta Boye (ID: 40281923), Ali Sher (ID: 40255236), Achraf Cheniti (ID: 40244865)
           Joseph El Bitar (ID: 40270590)
  Date: 2025-10-07

  Purpose:
    Single entry point for the marker. Prints a menu and dispatches to:
      1) testLoadMaps()      // Map + MapLoader + validation
      2) testPlayers()       // Player: toDefend/toAttack/issueOrder
      3) testOrdersLists()   // Orders/OrdersList: validate/execute/move/remove
      4) testCards()         // Deck/Hand/Card: draw/play/return
      5) testGameStates()    // Game engine state machine + commands

  Usage:
    Run, then enter 1–5 or 'quit'.

  Build (Windows/MinGW from project root):
    g++ -std=c++17 -O2 -Wall -Wextra -I. -IModels -IDrivers -ITests ^
        .\MainDriver.cpp .\Models\*.cpp .\Drivers\*.cpp -o .\App.exe
    chcp 65001 > $null
    $OutputEncoding = [Console]::OutputEncoding = [Text.UTF8Encoding]::new($false)
    .\App.exe

    # PowerShell: ensure UTF-8 so box-drawing shows correctly
      chcp 65001 > $null
      $OutputEncoding = [Console]::OutputEncoding = [Text.UTF8Encoding]::new($false)
      .\App.exe
-----------------------------------------------------------------------------*/

#include <iostream>
#include "Drivers/GameEngineDriver.h"
#include "Drivers/CardsDriver.h"
#include "Drivers/OrdersDriver.h"
#include "Drivers/MapDriver.h"
#include "Drivers/PlayerDriver.h"
#include <string>

using namespace std;

void menu()
{
   

    string cmd;

    while (true)
    {
         //Display menu as written
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
        if (!getline(cin, cmd)) //Get the users input
            break;

        if (cmd == "quit")
        {
            cout << " Goodbye! Thanks for using COMP-345 Project!\n";
            break;
        }

        if (cmd == "1")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Map Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testLoadMaps();//Part 1 tests
            cout << "\n Map Module Test Completed!\n";
        }
        else if (cmd == "2")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Player Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testPlayers();//Part 2 tests
            cout << "\n Player Module Test Completed!\n";
        }
        else if (cmd == "3")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << " Running Orders Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testOrdersLists();//Part 3 tests
            cout << "\n Orders Module Test Completed!\n";
        }
        else if (cmd == "4")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << " Running Cards Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testCards();//Part 4 tests
            cout << "\n Cards Module Test Completed!\n";
        }
        else if (cmd == "5")
        {
            cout << "\n ═══════════════════════════════════════════════════\n";
            cout << "   Running Game Engine Module Test...\n";
            cout << " ═══════════════════════════════════════════════════\n";
            testGameStates();//Part 5 tests
            cout << "\n Game Engine Module Test Completed!\n";
        }
        else
        {
            //Any other input is rejected; loop continues
            cout << " Invalid input! Please enter a number from 1-5 or 'quit'.\n";
        }
    }
}

int main()
{
    menu();
    return 0;
}