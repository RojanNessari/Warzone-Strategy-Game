echo "🔨 Compiling OrdersDriver.cpp..."
g++ -std=c++17 -o OrdersDriver Drivers/OrdersDriver.cpp Models/Orders.cpp
if [[ $? -eq 0 ]]; then
    echo "🚀 Running OrdersDriver..."
    ./OrdersDriver
    
    echo "🧹 Cleaning up executable..."
    rm -f OrdersDriver
    echo "✅ Cleanup complete!"
else
    echo "❌ Compilation failed!"
fi
