#!/bin/bash

run_cards() {
    echo "🃏 CARDS MODULE"
    echo "==============="
    echo "🔨 Compiling CardsDriver.cpp..."
    g++ -std=c++17 -o CardsDriver Drivers/CardsDriver.cpp Models/Cards.cpp Models/Player.cpp Models/Orders.cpp
    if [[ $? -eq 0 ]]; then
        echo "🚀 Running CardsDriver..."
        ./CardsDriver
        rm -f CardsDriver
        echo "✅ Cards module complete!"
    else
        echo "❌ Cards compilation failed!"
    fi
}

run_map() {
    echo "🗺️ MAP MODULE"
    echo "============="
    echo "🔨 Compiling MapDriver.cpp..."
    g++ -std=c++17 -o MapDriver Drivers/MapDriver.cpp Models/Map.cpp
    if [[ $? -eq 0 ]]; then
        echo "🚀 Running MapDriver..."
        ./MapDriver
        rm -f MapDriver
        echo "✅ Map module complete!"
    else
        echo "❌ Map compilation failed!"
    fi
}

run_orders() {
    echo "📜 ORDERS MODULE"
    echo "================"
    echo "🔨 Compiling OrdersDriver.cpp..."
    g++ -std=c++17 -o OrdersDriver Drivers/OrdersDriver.cpp Models/Orders.cpp
    if [[ $? -eq 0 ]]; then
        echo "🚀 Running OrdersDriver..."
        ./OrdersDriver
        rm -f OrdersDriver
        echo "✅ Orders module complete!"
    else
        echo "❌ Orders compilation failed!"
    fi
}

run_player() {
    echo "👤 PLAYER MODULE"
    echo "================"
    echo "🔨 Compiling PlayerDriver.cpp..."
    g++ -std=c++17 -o PlayerDriver Drivers/PlayerDriver.cpp Models/Player.cpp Models/Map.cpp Models/Cards.cpp Models/Orders.cpp
    if [[ $? -eq 0 ]]; then
        echo "🚀 Running PlayerDriver..."
        ./PlayerDriver
        rm -f PlayerDriver
        echo "✅ Player module complete!"
    else
        echo "❌ Player compilation failed!"
    fi
}

run_game_engine() {
    echo "🎮 GAME ENGINE MODULE"
    echo "===================="
    echo "🔨 Compiling GameEngineDriver.cpp..."
    g++ -std=c++17 -o GameEngineDriver Drivers/GameEngineDriver.cpp Models/GameEngine.cpp
    if [[ $? -eq 0 ]]; then
        echo "🚀 Running GameEngineDriver..."
        ./GameEngineDriver
        rm -f GameEngineDriver
        echo "✅ Game Engine module complete!"
    else
        echo "❌ Game Engine compilation failed!"
    fi
}

# Check if argument provided
if [[ $# -eq 1 ]]; then
    case $1 in
        "cards"|"1")
            run_cards
            ;;
        "map"|"2")
            run_map
            ;;
        "orders"|"3")
            run_orders
            ;;
        "player"|"4")
            run_player
            ;;
        "engine"|"5")
            run_game_engine
            ;;
        *)
            echo "❌ Invalid option: $1"
            echo "Usage: $0 [cards|map|orders|player|engine] or [1|2|3|4|5]"
            exit 1
            ;;
    esac
else
    # Interactive menu
    echo "🎯 COMP-345 Project Module Runner 🎯"
    echo "===================================="
    echo "1. 🃏 Cards Module"
    echo "2. 🗺️  Map Module"
    echo "3. 📜 Orders Module"
    echo "4. 👤 Player Module"
    echo "5. 🎮 Game Engine Module"
    echo "0. ❌ Exit"
    echo ""
    read -p "Enter your choice [0-5]: " choice

    case $choice in
        1)
            run_cards
            ;;
        2)
            run_map
            ;;
        3)
            run_orders
            ;;
        4)
            run_player
            ;;
        5)
            run_game_engine
            ;;
        0)
            echo "👋 Goodbye!"
            exit 0
            ;;
        *)
            echo "❌ Invalid choice!"
            exit 1
            ;;
    esac
fi