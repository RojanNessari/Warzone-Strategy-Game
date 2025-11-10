#!/bin/bash

echo "LOG OBSERVER"
echo "=============="
echo "Compiling Driver/LoggingObserverDriver.cpp..."

if g++ -std=c++17 -o LoggingObserver Drivers/LoggingObserverDriver.cpp  Models/*.cpp utils/*.cpp; then
    echo "Compilation succeeded. Running MainDriver..."
    ./LoggingObserver || echo "LoggingObserver exited with non-zero status"
    rm -f LoggingObserver
    echo "Main driver execution complete!"
else
    echo "Main driver compilation failed!"
    exit 1
fi