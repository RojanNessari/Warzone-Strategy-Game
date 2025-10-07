# COMP-345 Project UML Diagrams

This directory contains comprehensive UML diagrams for the COMP-345 Assignment 1 project, created using Mermaid syntax.

## Diagram Files

### Class Diagrams

1. **`complete_system_overview.md`** - Complete system showing all classes and their relationships
2. **`map_and_player_class_diagram.md`** - Map, Territory, Continent, Player, and MapLoader classes
3. **`orders_class_diagram.md`** - Order hierarchy and OrdersList class
4. **`cards_class_diagram.md`** - Card system including Card, Hand, Deck, and CardType
5. **`game_engine_class_diagram.md`** - GameEngine and State classes

### Sequence Diagrams

6. **`card_and_order_sequence.md`** - Shows the sequence of playing cards and executing orders
7. **`game_engine_state_sequence.md`** - Shows the state transitions in the game engine
8. **`map_loading_sequence.md`** - Shows the map loading process

### State Diagrams

9. **`game_engine_state_diagram.md`** - State machine diagram for the game engine

## How to View the Diagrams

### Option 1: GitHub (Recommended)

If this repository is on GitHub, the Mermaid diagrams will render automatically when you view the `.md` files.

### Option 2: VS Code with Mermaid Extension

1. Install the "Mermaid Preview" extension in VS Code
2. Open any `.md` file containing Mermaid diagrams
3. Use Ctrl+Shift+P (Cmd+Shift+P on Mac) and search for "Mermaid: Preview"

### Option 3: Mermaid Live Editor

1. Go to https://mermaid.live/
2. Copy and paste the Mermaid code (without the ```mermaid wrapper) into the editor
3. The diagram will render in real-time

### Option 4: Other Markdown Viewers

Many modern markdown viewers support Mermaid diagrams, including:

- Notion
- GitLab
- Bitbucket
- Typora
- Mark Text

## Diagram Descriptions

### Class Diagrams

- Show the structure of classes including attributes, methods, and relationships
- Use standard UML notation adapted for Mermaid syntax
- Show inheritance (extends), composition (contains), and association relationships

### Sequence Diagrams

- Show the interaction between objects over time
- Illustrate the flow of method calls and object creation
- Demonstrate how the system components work together

### State Diagrams

- Show the possible states of the GameEngine and transitions between them
- Include guard conditions and actions associated with transitions

## Key Relationships Illustrated

1. **Player-Territory**: One player can own multiple territories
2. **Map-Territory**: One map contains multiple territories
3. **Map-Continent**: One map contains multiple continents
4. **Continent-Territory**: One continent contains multiple territories
5. **Player-Hand**: Each player has one hand of cards
6. **Player-OrdersList**: Each player has one list of orders
7. **Order Hierarchy**: All order types inherit from the base Order class
8. **GameEngine-State**: GameEngine manages multiple states and has one current state

## Assignment Parts Covered

- **Part 1**: Map classes (Map, Territory, Continent, MapLoader)
- **Part 2**: Player class and its relationships
- **Part 3**: Orders hierarchy and OrdersList
- **Part 4**: Cards system (Card, Hand, Deck)
- **Part 5**: GameEngine and State management

These diagrams provide a comprehensive view of the system architecture and can be used for:

- Understanding the codebase structure
- Documentation purposes
- Design discussions
- Code review reference
- Assignment submission documentation
