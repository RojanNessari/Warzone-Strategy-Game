#include "Cards.h"
#include <iostream>
using namespace std;

void DebugPrint (const string &massage){
    if(DEBUG){
        cout<<"[DEBUG] "<<massage<<endl;
    }
} 
const char* CardTypeToString(CardType type){
    switch(type){
        case CardType::Bomb:return "Bomb";
        case CardType::Reinforcement:return "Reinforcement";
        case CardType::Blockade:return "Blockade";      
        case CardType::Airlift:return "Airlift";
        case CardType::Diplomacy:return "Diplomacy";
        default:return "Unknown";
    }
}
Card::Card(CardType t){
    type=t;
    DebugPrint("Card created  in Card costractor of type: " + string(CardTypeToString(t)));

}
Card::Card(const Card& other) {
    type = other.type;   
    DebugPrint(" copy constructor is using to copy " );
}
Card & Card ::operator=(const Card&other){
    if(this!=&other){
       type=other.type;
    DebugPrint(" assignment operator is using to assign ");    
    }
    
    return *this;
    

}
ostream& operator<<(std::ostream& os, const Card& card) {
        os << "Card(Type: " << CardTypeToString(card.type)<< ")";
        return os;
    }
void play(Player& player,OrderList& ordersList){
        // Implementation of play method
        DebugPrint("Playing card of type: " );
        // Add logic to create and add the corresponding order to ordersList
    }
  

Hand::Hand(vector<Card*> c){
    card=c;
    DebugPrint("Hand created in Hand costractor with "+ to_string(c.size())+" cards");
}
Hand::Hand(const Hand&other){
    card=other.card;
    DebugPrint(" copy constructor is using to copy " + to_string(other.card.size())+" cards");
} 
  