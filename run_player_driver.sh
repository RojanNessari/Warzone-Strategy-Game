echo "🔨 Compiling MapDriver.cpp..."
g++ -std=c++17 -o PlayerDriver PlayerDriver.cpp Models/Player.cpp

if [[ $? -eq 0 ]]; then
    echo "🚀 Running MapDriver..."
    ./PlayerDriver

    echo "🧹 Cleaning up executable..."
    rm -f PlayerDriver
    echo "✅ Cleanup complete!"
else
    echo "❌ Compilation failed!"
fi