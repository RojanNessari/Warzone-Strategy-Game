#ifndef Cards_H
#define Cards_H
#include <string>
#include <vector>
using namespace std;

class Player;
class OrderList;
class Order;

enum  class CardType{
    Bomb, 
    Reinforcement, 
    Blockade,
    Airlift,
    Diplomacy
};
const char* CardTypeToString(CardType type);

class Card{
    private:
    CardType type;
    public:
    explicit Card(CardType t);
    Card(const Card &other );
    Card &operator=(const Card &other);
    friend ostream& operator<<(ostream& os,const Card &card);
    void play(Player& player,OrderList& ordersList);
};

class Hand{

    private:
   vector<Card*> card;

    public :
    explicit Hand(vector<Card*> c);
    Hand(const Hand&other);
    Hand &operator=(const Hand &other);

    void addCard(Card* c);
    Card* removeAt(size_t index);
    void playCard(int index);

    friend ostream operator<<(ostream & out,const Hand& h);
 

};
class Deck{
    private :
    vector< Card*> card;
    public:
    explicit Deck(vector<Card*> c);
    Deck(const Deck& other);
    Deck&operator=(const Deck &other);
   
    Card* draw();
    friend ostream&operator<<(const ostream os,const Deck &d);
};
const bool DEBUG=true;
void DebugPrint (const string &massage);

#endif