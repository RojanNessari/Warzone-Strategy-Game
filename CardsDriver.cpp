#include <iostream>
#include "Models/Cards.h"
using namespace std;


int main(){
    DebugPrint("Starting CardsDriver.cpp");
Card c1(CardType::Airlift);
Card c2=c1; // Copy constructor
Card c3(CardType::Bomb);
c3=c1; // Assignment operator
cout<<c1<<endl;
cout<<c2<<endl;
cout<<c3<<endl;
 return 0;

}
