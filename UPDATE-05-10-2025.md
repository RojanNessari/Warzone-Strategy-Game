# Project Update Log

## Changes Made

### Player Module

- **Player.h**:

  - Utilized the `Territory` class from `Map`, `OrdersList` from `Orders`, and `Hand` from `Cards`.
  - Added a new method `getHandOfCards()` to retrieve the player's hand of cards.
  - Implemented the **assignment operator** and **copy constructor** for the `Player` class.
  - Although not strictly necessary for the abstract `Order` class, assignment operators and copy constructors were added to all classes as per assignment instructions.

- **PlayerDriver.cpp**:
  - Enhanced testing by adding more test cases to validate the `Player` class functionality.

### Cards Module

- **Cards.h**:

  - Added a `size()` method in the `Hand` class to retrieve the length of the `std::vector<Card *> cards`.

- **CardsDriver.cpp**:
  - Refactored the `main()` function by moving the card testing logic into a free function named `testCards()`.

### Orders Module

- **Orders.h** and **Orders.cpp**:
  - Implemented the **copy constructor** and **assignment operator** for the `OrdersList` class to ensure proper memory management and avoid shallow copy issues.

## Testing

- All modules were tested using the provided shell scripts for each part.
- Compilation completed successfully with no errors.
- All outputs were printed as expected, confirming the correctness of the implementation.

## Next Steps

- Review and test the `Engine` part of the project.
- Ensure all modules integrate seamlessly and meet the assignment requirements.
