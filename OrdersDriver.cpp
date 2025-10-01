#include "Orders.h"
#include <iostream>

int main() {
    OrdersList list;

    // Add one order for testing
    list.add(new Deploy());

    std::cout << "Initial Orders:" << std::endl;
    list.print();

    return 0;
}
