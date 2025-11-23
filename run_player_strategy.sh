echo "PlayerStrategyDriver"
echo "=============="
echo "Compiling PlayerStrategyDriver.cpp..."

if g++ -std=c++17 -fsanitize=address -g -o PlayerStrategyDriver Drivers/PlayerStrategyDriver.cpp Models/*.cpp utils/*.cpp PlayerStrategies/*.cpp; then
    echo "Compilation succeeded. Running MainDriver..."
    ./PlayerStrategyDriver || echo "PlayerStrategyDriver exited with non-zero status"
    rm -f PlayerStrategyDriver
    echo "PlayerStrategyDriver execution complete!"
else
    echo "PlayerStrategyDriver compilation failed!"
    exit 1
fi