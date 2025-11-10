#!/bin/bash

# Script to test CommandProcessor implementation
# Tests both console mode and file mode with multiple test files

# Compile the CommandProcessingDriver

echo "Compiling CommandProcessingDriver..."
g++ -std=c++17 -o CommandProcessingDriver \
    Drivers/CommandProcessingDriver.cpp \
    Models/CommandProcessing.cpp \
    Models/GameEngine.cpp \
    Models/Map.cpp \
    Models/Player.cpp \
    Models/Orders.cpp \
    Models/Cards.cpp \
    utils/logger.cpp

if [[ $? -ne 0 ]]; then
    echo "❌ Compilation failed!"
    exit 1
fi

echo "✓ Compilation successful!"

echo "======================================"
echo "COMMAND PROCESSOR TEST SUITE" 
echo "======================================" 
echo ""

# Test 1: Console Mode
echo ""
echo "======================================"
echo "[1] Console Mode Test"
echo "======================================"
./CommandProcessingDriver -console
echo ""


# Test 2: File Mode with all test files
while true; do
    read -p "Do you want to proceed with file mode testing? [Y/n]: " yn
    case $yn in 
        [Yy]* ) make install; break;;
        [Nn]*) exit;;
        *) echo "Please enter [Y/n]";;
    esac
done
echo ""
echo "======================================"
echo "[2] Testing File Mode (-file)"
echo "======================================"

# Find all .txt files in cmdProcessingTestFiles directory
test_files=(cmdProcessingTestFiles/*.txt)

if [[ ${#test_files[@]} -eq 0 ]]; then
    echo "⚠ No test files found in cmdProcessingTestFiles/"
else
    for test_file in "${test_files[@]}"; do
        if [[ -f "$test_file" ]]; then
            echo ""
            echo "File contents:"
            cat "$test_file"
            echo ""
            echo "Running test..."
            ./CommandProcessingDriver -file "$test_file"
            echo "----------------------------------------------------------------------------------------------------"
        fi
    done
fi

# Test 3: Invalid argument test
echo ""
echo "======================================"
echo "[3] Testing Invalid Arguments"
echo "======================================"
echo "Testing with no arguments:"
./CommandProcessingDriver
echo ""
echo "Testing with invalid argument:"
./CommandProcessingDriver -invalid
echo ""

# Cleanup
echo "======================================"
echo "[4] Cleanup"
echo "======================================"
rm -f CommandProcessingDriver
echo "✓ Removed executable"

echo ""
echo "======================================"
echo "TEST SUITE COMPLETE!"
echo "======================================"
