#include "Models/Player.h"
#include "Models/Orders.h"
#include "Models/Cards.h"
#include <iostream>
using namespace std;

void testPlayers()
{
    cout << "\n🧪 Testing Player Class Features 🧪\n";

    // Create players
    Player p1;
    Player p2;

    cout << "\n👤 Initial Players:\n";
    cout << p1 << endl;
    cout << p2 << endl;

    // Issue orders
    cout << "\n📝 Issuing orders for p1...\n";
    p1.issueOrder();
    p1.issueOrder();
    cout << p1 << endl;

    // Add cards to hand
    cout << "\n🃏 Adding cards to p2's hand...\n";
    Card *c1 = new Card(CardType::Airlift);
    Card *c2 = new Card(CardType::Bomb);
    p2.getHandOfCards()->addCard(c1);
    p2.getHandOfCards()->addCard(c2);
    cout << p2 << endl;

    // Test copy constructor
    cout << "\n📋 Copying p1 to p3...\n";
    Player p3 = p1;
    cout << p3 << endl;

    // Test assignment operator
    cout << "\n🔄 Assigning p2 to p1...\n";
    p1 = p2;
    cout << p1 << endl;

    // Test toDefend and toAttack (should show pointer count)
    cout << "\n🛡️ p1 toDefend: " << p1.toDefend().size() << " territories\n";
    cout << "⚔️ p1 toAttack: " << p1.toAttack().size() << " territories\n";

    cout << "\n✅ All Player features tested!\n";
}

int main()
{
    testPlayers();
    return 0;
}
