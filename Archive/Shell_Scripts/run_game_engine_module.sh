echo "🔨 Compiling GameEngineDriver.cpp..."
g++ -std=c++17 -o GameEngineDriver Drivers/GameEngineDriver.cpp Models/GameEngine.cpp

if [[ $? -eq 0 ]]; then
    echo "🚀 Running GameEngineDriver..."
    ./GameEngineDriver

    echo "🧹 Cleaning up executable..."
    rm -f GameEngineDriver
    echo "✅ Cleanup complete!"
else
    echo "❌ Compilation failed!"
fi
