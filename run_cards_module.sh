echo "🔨 Compiling CardsDriver.cpp..."
g++ -std=c++17 -o CardsDriver CardsDriver.cpp Models/Cards.cpp

if [[ $? -eq 0 ]]; then
    echo "🚀 Running CardsDriver..."
    ./CardsDriver

    echo "🧹 Cleaning up executable..."
    rm -f CardsDriver
    echo "✅ Cleanup complete!"
else
    echo "❌ Compilation failed!"
fi
