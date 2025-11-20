
echo "MAIN DRIVER"
echo "=============="
echo "Compiling MainDriver.cpp..."

# Use ASAN and debug symbols to catch memory errors
if g++ -std=c++17 -g -fsanitize=address -o MainDriver MainDriver.cpp Drivers/*.cpp Models/*.cpp utils/*.cpp  PlayerStrategies/*.cpp; then
    echo "Compilation succeeded. Running MainDriver..."
    ./MainDriver || echo "MainDriver exited with non-zero status"
    rm -f MainDriver
    echo "Main driver execution complete!"
else
    echo "Main driver compilation failed!"
    exit 1
fi