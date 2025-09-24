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
    DebugPrint("Card created by copy constructor of type: " + string(CardTypeToString(type)));
}
Card & Card ::operator=(const Card&other){
    if(this!=&other){
       type=other.type;
   
    }
    return *this;
     DebugPrint("Card created by assignment operator of type: " + string(CardTypeToString(type)));

}
    ostream& operator<<(std::ostream& os, const Card& card) {
        os << "Card(Type: " << CardTypeToString(card.type)<< ")";
        return os;
    }
    void play(Player& player,OrderList& ordersList){
        // Implementation of play method
        DebugPrint("Playing card of type: " + string(CardTypeToString(type)));
        // Add logic to create and add the corresponding order to ordersList
    }
  

  
  