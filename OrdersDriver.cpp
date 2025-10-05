#include "Models/Orders.h"
#include <iostream>

void testOrdersLists()
{
    OrdersList list;

    list.add(new Deploy());
    list.add(new Advance());
    list.add(new Bomb());
    list.add(new Blockade());
    list.add(new Airlift());
    list.add(new Negotiate());

    std::cout << "📋 Initial Orders:" << std::endl;
    list.print();

    std::cout << "\n🔼 Moving order 2 (Bomb) to the top..." << std::endl;
    list.move(2, 0);
    list.print();

    std::cout << "\n❌ Removing order 3..." << std::endl;
    list.remove(3);
    list.print();

    std::cout << "\n⚡ Executing all orders..." << std::endl;
    for (size_t i = 0; i < list.size(); i++)
    {
        Order *o = list.get((int)i);
        if (o && o->validate())
        {
            o->execute();
        }
    }
    list.print();
}

int main()
{
    testOrdersLists();
    return 0;
}
