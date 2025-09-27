echo "🔨 Compiling MapDriver.cpp..."
g++ -std=c++17 -o PlayerDriver PlayerDriver.cpp Models/Player.cpp

if [[ $? -eq 0 ]]; then
    echo "🚀 Running MapDriver..."
    ./PlayerDriver
else
    echo "❌ Compilation failed!"
fi