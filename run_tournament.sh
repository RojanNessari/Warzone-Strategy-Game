#!/bin/bash

echo "TOURNAMENT Driver"
echo "=============="
echo "Compiling Drivers/TournamentDriver.cpp..."

if g++ -std=c++17 -o TournamentDriver Drivers/TournamentDriver.cpp  Models/*.cpp utils/*.cpp PlayerStrategies/*.cpp; then
    echo "Compilation succeeded. Running MainDriver..."
    ./TournamentDriver -M "Maps/alberta.map" -P Aggressive, Benevolent, Neutral -G 4 -D 10 || echo "TournamentDriver exited with non-zero status"
    rm -f TournamentDriver
    echo "TournamentDriver execution complete!"
else
    echo "TournamentDriver compilation failed!"
    exit 1
fi

