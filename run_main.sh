#!/bin/bash

run_main() {
    echo "🚀 MAIN DRIVER"
    echo "=============="
    echo "🔨 Compiling MainDriver.cpp..."
    g++ -std=c++17 -o MainDriver MainDriver.cpp Drivers/*.cpp Models/*.cpp
    if [[ $? -eq 0 ]]; then
        echo "🚀 Running MainDriver..."
        ./MainDriver
        rm -f MainDriver
        echo "✅ Main driver execution complete!"
    else
        echo "❌ Main driver compilation failed!"
    fi
}

# Execute the main driver
run_main