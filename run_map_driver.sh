echo "🔨 Compiling MapDriver.cpp..."
g++ -std=c++17 -o MapDriver MapDriver.cpp Models/Map.cpp

if [[ $? -eq 0 ]]; then
    echo "🚀 Running MapDriver..."
    ./MapDriver
    
    echo "🧹 Cleaning up executable..."
    rm -f MapDriver
    echo "✅ Cleanup complete!"
else
    echo "❌ Compilation failed!"
fi