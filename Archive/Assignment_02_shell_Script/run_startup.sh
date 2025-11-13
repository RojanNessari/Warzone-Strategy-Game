echo "Compiling GameEngineDriver.cpp..."
g++ -std=c++17 -o GameEngineDriver Drivers/GameEngineDriver.cpp Models/*.cpp utils/*.cpp
    if [[ $? -eq 0 ]]; then
        echo "Running GameEngineDriver..."
        ./GameEngineDriver
        rm -f GameEngineDriver
        echo "GameEngineDriver execution complete!"
    else
        echo "GameEngineDriver compilation failed!"
    fi