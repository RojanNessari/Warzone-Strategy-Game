#include <iostream>
#include "Drivers/GameEngineDriver.h"
#include "Drivers/CardsDriver.h"
#include "Drivers/OrdersDriver.h"
#include "Drivers/MapDriver.h"
#include "Drivers/PlayerDriver.h"

using namespace std;

void menu()
{
    cout << R"(
╔══════════════════════════════════════════════════════╗
║               🚀 COMP-345 Assignment-01 🚀           ║
╠══════════════════════════════════════════════════════╣
║                                                      ║
║  1) 🗺️  Run testLoadMaps()    - Part 1: Map Module    ║
║  2) 👤  Run testPlayers()     - Part 2: Player Module║
║  3) 📜  Run testOrdersLists() - Part 3: Orders Module║
║  4) 🃏  Run testCards()       - Part 4: Cards Module ║
║  5) 🎮  Run testGameStates()  - Part 5: Game Engine  ║
║                                                      ║
║  📝 Type "quit" to exit                              ║
║                                                      ║
╚══════════════════════════════════════════════════════╝
    )";

    string cmd;

    while (true)
    {
        cout << "\n🎯 Enter your choice (1-5) or 'quit': ";
        if (!getline(cin, cmd))
            break;

        if (cmd == "quit")
        {
            cout << "👋 Goodbye! Thanks for using COMP-345 Project!\n";
            break;
        }

        if (cmd == "1")
        {
            cout << "\n🗺️ ═══════════════════════════════════════════════════\n";
            cout << "   Running Map Module Test...\n";
            cout << "🗺️ ═══════════════════════════════════════════════════\n";
            testLoadMaps();
            cout << "\n✅ Map Module Test Completed!\n";
        }
        else if (cmd == "2")
        {
            cout << "\n👤 ═══════════════════════════════════════════════════\n";
            cout << "   Running Player Module Test...\n";
            cout << "👤 ═══════════════════════════════════════════════════\n";
            testPlayers();
            cout << "\n✅ Player Module Test Completed!\n";
        }
        else if (cmd == "3")
        {
            cout << "\n📜 ═══════════════════════════════════════════════════\n";
            cout << "   Running Orders Module Test...\n";
            cout << "📜 ═══════════════════════════════════════════════════\n";
            testOrdersLists();
            cout << "\n✅ Orders Module Test Completed!\n";
        }
        else if (cmd == "4")
        {
            cout << "\n🃏 ═══════════════════════════════════════════════════\n";
            cout << "   Running Cards Module Test...\n";
            cout << "🃏 ═══════════════════════════════════════════════════\n";
            testCards();
            cout << "\n✅ Cards Module Test Completed!\n";
        }
        else if (cmd == "5")
        {
            cout << "\n🎮 ═══════════════════════════════════════════════════\n";
            cout << "   Running Game Engine Module Test...\n";
            cout << "🎮 ═══════════════════════════════════════════════════\n";
            testGameStates();
            cout << "\n✅ Game Engine Module Test Completed!\n";
        }
        else
        {
            cout << "❌ Invalid input! Please enter a number from 1-5 or 'quit'.\n";
        }
    }
}

int main()
{
    menu();
    return 0;
}