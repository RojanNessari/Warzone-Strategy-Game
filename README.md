# Warzone Game Engine (COMP 345 Team Project)

C++ implementation of a turn-based strategy game engine inspired by *Warzone*, developed as a **team project** for  
**COMP 345 – Advanced Program Design with C++**  
Concordia University, Montreal, Canada

---

## 📌 Project Overview

This project is a modular and extensible **strategy game engine** built in C++.  
It was developed incrementally across multiple assignments, each focusing on core **object-oriented design**, **memory management**, and **software design patterns**.

The game engine supports map loading and validation, player decision-making, order execution, AI strategies, tournament simulation, and observer-based logging.

---

## ✨ Key Features

### 🗺️ Map System
- Graph-based map representation (territories as nodes, adjacency as edges)
- Validation rules:
  - Map is a connected graph
  - Continents are connected subgraphs
  - Each territory belongs to exactly one continent
- Supports loading external Conquest `.map` files

### 👤 Player, Orders, and Cards
- Players own:
  - Territories
  - A list of orders
  - A hand of cards
- Orders implemented using inheritance:
  - Deploy
  - Advance
  - Bomb
  - Blockade
  - Airlift
  - Negotiate
- Orders are validated at execution time
- Cards generate special orders and return to the deck after being played

### ⚙️ Game Engine & Command Processing
- State-driven game flow:
  - Startup
  - Reinforcement
  - Issue Orders
  - Execute Orders
  - Win
- Command-based interaction using a `CommandProcessor`
- Commands can be read from:
  - Console
  - File (via Adapter design pattern)

### ♟️ Strategy Pattern (AI Players)
Player behaviors implemented using the **Strategy design pattern**:
- Human
- Aggressive
- Benevolent
- Neutral (becomes aggressive if attacked)
- Cheater

Strategies can be assigned and changed dynamically during gameplay.

### 🏆 Tournament Mode
- Fully automated tournament execution
- Parameters:
  - Multiple maps
  - Multiple AI strategies
  - Multiple games per map
  - Maximum number of turns
- Outputs a summarized tournament result table

### 📝 Observer Pattern Logging
- Observer-based logging system
- Logs written to `gamelog.txt` include:
  - Commands and their effects
  - Orders added and executed
  - Game state transitions

---

## 🧩 Design Patterns Used

- Strategy — player behaviors
- Observer — game logging
- Adapter — file-based command input
- State Machine — game engine phases

---

---

## ▶️ Build & Run

### Requirements
- C++17 compatible compiler (e.g. `g++`)
- macOS / Linux / Windows (WSL)

### Compile (example)
```bash
g++ -std=c++17 -o main MainDriver.cpp \
  Models/Map.cpp Models/Player.cpp Models/Orders.cpp Models/Cards.cpp \
  Models/GameEngine.cpp Models/CommandProcessing.cpp Models/LoggingObserver.cpp

