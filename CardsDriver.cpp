#include <iostream>
#include "Models/Cards.h"
#include "Models/Player.h"
#include "Models/Orders.h"

using namespace std;

 void testCards(){
cout << "Testing Cards module\n";

Deck deck;
 cout << deck << '\n';
Player player1;
Hand hand1;
OrderList ordersList;

cout << "Drawing 5 cards from deck to player's hand...\n";
for (int i=0;i<5;i++){
    Card *drawn =deck.draw(player1,hand1);
if (drawn) cout << "Drew: " << *drawn << '\n';

 }
cout<<"Player's hand after drawing 5 cards:"<<hand1<<endl;
cout<<"Deck after drawing 5 cards:"<<deck<<endl;
 cout << "Playing the 3rd card in player's hand...\n";
hand1.playCard(2,player1,ordersList,deck); // Play the 3rd card (index 2)
cout<<"Player's hand after playing 3rd card:"<<hand1<<endl;
cout<<"Deck after playing 3rd card:"<<deck<<endl;
cout << "Testing complete.\n";
}

int main(){
testCards();
return 0;
}
