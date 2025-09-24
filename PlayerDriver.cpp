#include "Player.h"
#include <iostream>
using namespace std;

void testPlayers() {
    Player p1;

    cout << "Defend list: ";
    for (auto& t : p1.toDefend()) cout << t << " ";
    cout << endl;

    cout << "Attack list: ";
    for (auto& t : p1.toAttack()) cout << t << " ";
    cout << endl;

    p1.issueOrder();
    cout << p1 << endl; // Uses the << operator
}

int main() {
    testPlayers();
    return 0;
}
