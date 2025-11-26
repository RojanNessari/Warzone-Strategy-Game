#!/bin/bash

echo "MAIN DRIVER"
echo "=============="
echo "Compiling MainDriver.cpp..."

if g++ -std=c++17 -o MainDriver MainDriver.cpp Drivers/*.cpp Models/*.cpp utils/*.cpp PlayerStrategies/*.cpp; then
    echo "Compilation succeeded. Running MainDriver..."
    ./MainDriver -M "Maps/alberta.map" -P Aggressive, Benevolent, Neutral -G 4 -D 10 || echo "MainDriver exited with non-zero status"
    rm -f MainDriver
    echo "Main driver execution complete!"
else
    echo "Main driver compilation failed!"
    exit 1
fi