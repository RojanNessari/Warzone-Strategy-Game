#include <iostream>
#include "../utils/LoggingObserver.h"
#include "../Models/CommandProcessing.h"
#include "../Models/Orders.h"
#include "../Models/GameEngine.h"

using namespace std;

// test class
class TestSubject : public Subject, public ILoggable
{
private:
    string message;

public:
    TestSubject(string msg) : message(msg) {}
    void changeMessage(string newMsg)
    {
        message = newMsg;
        Notify(this); // notify observer
    }
    string stringToLog() override
    {
        return "TestSubject message: " + message;
    }
};

// Driver function
void testLoggingObserver()
{
    // Create a LogObserver
    LogObserver *logger = new LogObserver();

    // Test CommandProcessor
    std::cout << "Testing CommandProcessor..." << std::endl;
    CommandProcessor *processor = new CommandProcessor();
    processor->Attach(logger);                // Attach observer
    Command *cmd = processor->getCommand();   // Simulate getting a command
    cmd->saveEffect("Effect of the command"); // Save the effect of the command

    // Test OrdersList and Orders
    std::cout << "Testing OrdersList and Orders..." << std::endl;
    OrdersList *ordersList = new OrdersList();
    ordersList->Attach(logger);        // Attach observer
    Order *deployOrder = new Deploy(); // Example order
    ordersList->add(deployOrder);      // Add order to the list
    deployOrder->execute();            // Execute the order

    // Test GameEngine
    std::cout << "Testing GameEngine..." << std::endl;
    GameEngine *engine = new GameEngine();
    engine->Attach(logger);          // Attach observer
    engine->buildGraph();            // Build the state graph
    engine->applyCommand("loadmap"); // Simulate state transitions
    engine->applyCommand("validatemap");
    engine->applyCommand("addplayer");
    engine->applyCommand("gamestart");

    // Cleanup
    delete processor;
    delete ordersList;
    delete engine;
    delete logger;

    std::cout << "Check gamelog.txt for results." << std::endl;
}

int main()
{
    testLoggingObserver();
    return 0;
}
