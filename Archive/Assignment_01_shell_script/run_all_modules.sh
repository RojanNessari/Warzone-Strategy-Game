#!/bin/bash#!/bin/bash



echo "🚀 Running All Modules 🚀"echo "🚀 Running All Modules 🚀"

echo "=========================="echo "=========================="



# Cards Module# Cards Module

echo ""echo ""

echo "🃏 CARDS MODULE"echo "🃏 CARDS MODULE"

echo "==============="echo "==============="

echo "🔨 Compiling CardsDriver.cpp..."echo "🔨 Compiling CardsDriver.cpp..."

g++ -std=c++17 -o CardsDriver Drivers/CardsDriver.cpp Models/Cards.cpp Models/Player.cpp Models/Orders.cppg++ -std=c++17 -o CardsDriver Drivers/CardsDriver.cpp Models/Cards.cpp Models/Player.cpp Models/Orders.cpp



if [[ $? -eq 0 ]]; thenif [[ $? -eq 0 ]]; then

    echo "🚀 Running CardsDriver..."    echo "🚀 Running CardsDriver..."

    ./CardsDriver    ./CardsDriver

    rm -f CardsDriver    rm -f CardsDriver

    echo "✅ Cards module complete!"    echo "✅ Cards module complete!"

elseelse

    echo "❌ Cards compilation failed!"    echo "❌ Cards compilation failed!"

    exit 1    exit 1

fifi



# Map Module# Map Module

echo ""echo ""

echo "🗺️ MAP MODULE"echo "🗺️ MAP MODULE"

echo "============="echo "============="

echo "🔨 Compiling MapDriver.cpp..."echo "🔨 Compiling MapDriver.cpp..."

g++ -std=c++17 -o MapDriver Drivers/MapDriver.cpp Models/Map.cppg++ -std=c++17 -o MapDriver Drivers/MapDriver.cpp Models/Map.cpp



if [[ $? -eq 0 ]]; thenif [[ $? -eq 0 ]]; then

    echo "🚀 Running MapDriver..."    echo "🚀 Running MapDriver..."

    ./MapDriver    ./MapDriver

    rm -f MapDriver    rm -f MapDriver

    echo "✅ Map module complete!"    echo "✅ Map module complete!"

elseelse

    echo "❌ Map compilation failed!"    echo "❌ Map compilation failed!"

    exit 1    exit 1

fifi



# Orders Module# Orders Module

echo ""echo ""

echo "📜 ORDERS MODULE"echo "📜 ORDERS MODULE"

echo "================"echo "================"

echo "🔨 Compiling OrdersDriver.cpp..."echo "🔨 Compiling OrdersDriver.cpp..."

g++ -std=c++17 -o OrdersDriver Drivers/OrdersDriver.cpp Models/Orders.cppg++ -std=c++17 -o OrdersDriver Drivers/OrdersDriver.cpp Models/Orders.cpp



if [[ $? -eq 0 ]]; thenif [[ $? -eq 0 ]]; then

    echo "🚀 Running OrdersDriver..."    echo "🚀 Running OrdersDriver..."

    ./OrdersDriver    ./OrdersDriver

    rm -f OrdersDriver    rm -f OrdersDriver

    echo "✅ Orders module complete!"    echo "✅ Orders module complete!"

elseelse

    echo "❌ Orders compilation failed!"    echo "❌ Orders compilation failed!"

    exit 1    exit 1

fifi



# Player Module# Player Module

echo ""echo ""

echo "👤 PLAYER MODULE"echo "👤 PLAYER MODULE"

echo "================"echo "================"

echo "🔨 Compiling PlayerDriver.cpp..."echo "🔨 Compiling PlayerDriver.cpp..."

g++ -std=c++17 -o PlayerDriver Drivers/PlayerDriver.cpp Models/Player.cpp Models/Map.cpp Models/Cards.cpp Models/Orders.cppg++ -std=c++17 -o PlayerDriver Drivers/PlayerDriver.cpp Models/Player.cpp Models/Map.cpp Models/Cards.cpp Models/Orders.cpp



if [[ $? -eq 0 ]]; thenif [[ $? -eq 0 ]]; then

    echo "🚀 Running PlayerDriver..."    echo "🚀 Running PlayerDriver..."

    ./PlayerDriver    ./PlayerDriver

    rm -f PlayerDriver    rm -f PlayerDriver

    echo "✅ Player module complete!"    echo "✅ Player module complete!"

elseelse

    echo "❌ Player compilation failed!"    echo "❌ Player compilation failed!"

    exit 1    exit 1

fifi



# Game Engine Module# Game Engine Module

echo ""echo ""

echo "🎮 GAME ENGINE MODULE"echo "🎮 GAME ENGINE MODULE"

echo "===================="echo "===================="

echo "🔨 Compiling GameEngineDriver.cpp..."echo "🔨 Compiling GameEngineDriver.cpp..."

g++ -std=c++17 -o GameEngineDriver Drivers/GameEngineDriver.cpp Models/GameEngine.cppg++ -std=c++17 -o GameEngineDriver Drivers/GameEngineDriver.cpp Models/GameEngine.cpp



if [[ $? -eq 0 ]]; thenif [[ $? -eq 0 ]]; then

    echo "🚀 Running GameEngineDriver..."    echo "🚀 Running GameEngineDriver..."

    ./GameEngineDriver    ./GameEngineDriver

    rm -f GameEngineDriver    rm -f GameEngineDriver

    echo "✅ Game Engine module complete!"    echo "✅ Game Engine module complete!"

elseelse

    echo "❌ Game Engine compilation failed!"    echo "❌ Game Engine compilation failed!"

    exit 1    exit 1

fifi



echo ""echo ""

echo "🎉 All modules completed successfully! 🎉"echo "🎉 All modules completed successfully! 🎉"