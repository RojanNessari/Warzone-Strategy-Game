echo "🔨 Compiling PlayerDriver.cpp..."
g++ -std=c++17 -o PlayerDriver Drivers/PlayerDriver.cpp Models/Player.cpp Models/Map.cpp Models/Cards.cpp Models/Orders.cpp

if [[ $? -eq 0 ]]; then
    echo "🚀 Running PlayerDriver..."
    ./PlayerDriver

    echo "🧹 Cleaning up executable..."
    rm -f PlayerDriver
    echo "✅ Cleanup complete!"
else
    echo "❌ Compilation failed!"
fi