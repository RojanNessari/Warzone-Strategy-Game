#include <iostream>
#include "../Models/Cards.h"
#include "../Models/Player.h"
#include "../Models/Orders.h"

void testCards()
{
    std::cout << " Testing Cards module \n";

    Deck deck;
    std::cout << deck << '\n';

    Player player1;
    Hand hand1;
    OrdersList ordersList;

    std::cout << " Drawing 5 cards from deck to player's hand...\n";
    for (int i = 0; i < 5; ++i)
    {
        Card *drawn = deck.draw(player1, hand1);
        if (drawn)
            std::cout << " Drew: " << *drawn << " \n";
    }

    std::cout << " Player's hand after drawing 5 cards: " << hand1 << '\n';
    std::cout << " Deck after drawing 5 cards: " << deck << '\n';

    std::cout << " Playing the 3rd card in player's hand...\n";
    hand1.playCard(2, player1, ordersList, deck);

    std::cout << " Player's hand after playing 3rd card: " << hand1 << '\n';
    std::cout << " Deck after playing 3rd card: " << deck << '\n';

    std::cout << " Orders created by played cards:\n";
    ordersList.print();

    std::cout << " Testing complete.\n";
}
