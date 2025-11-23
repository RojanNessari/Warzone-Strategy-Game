#include <iostream>
#include <fstream>
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
        Notify(this, INFO, "msg: " + newMsg); // notify observer
    }
};

// Enhanced testLoggingObserver to explicitly demonstrate all requirements
void testLoggingObserver()
{
    // Use the global singleton logger instead of creating a new one
    LogObserver *logger = LogObserver::getInstance();

    // Test CommandProcessor
    std::cout << "Testing CommandProcessor..." << std::endl;
    CommandProcessor *processor = new CommandProcessor();
    processor->Attach(logger);                // Attach observer
    Command *cmd = processor->getCommand();   // Simulate getting a command
    cmd->saveEffect("Effect of the command"); // Save the effect of the command

    // Verify subclassing for CommandProcessor
    if (dynamic_cast<Subject *>(processor) && dynamic_cast<ILoggable *>(processor))
    {
        std::cout << "CommandProcessor is a subclass of Subject and ILoggable." << std::endl;
    }

    // Test OrdersList and Orders
    std::cout << "Testing OrdersList and Orders..." << std::endl;
    OrdersList *ordersList = new OrdersList();
    ordersList->Attach(logger);        // Attach observer
    Order *deployOrder = new Deploy(); // Example order
    ordersList->add(deployOrder);      // Add order to the list
    deployOrder->execute();            // Execute the order

    // Verify subclassing for Orders and OrdersList
    if (dynamic_cast<Subject *>(ordersList) && dynamic_cast<ILoggable *>(ordersList))
    {
        std::cout << "OrdersList is a subclass of Subject and ILoggable." << std::endl;
    }
    if (dynamic_cast<Subject *>(deployOrder) && dynamic_cast<ILoggable *>(deployOrder))
    {
        std::cout << "Order is a subclass of Subject and ILoggable." << std::endl;
    }

    // Test GameEngine
    std::cout << "Testing GameEngine..." << std::endl;
    GameEngine *engine = new GameEngine();
    engine->Attach(logger);          // Attach observer
    engine->buildGraph();            // Build the state graph
    engine->applyCommand("loadmap"); // Simulate state transitions
    engine->applyCommand("validatemap");
    engine->applyCommand("addplayer");
    engine->applyCommand("gamestart");

    // Verify subclassing for GameEngine
    if (dynamic_cast<Subject *>(engine) && dynamic_cast<ILoggable *>(engine))
    {
        std::cout << "GameEngine is a subclass of Subject and ILoggable." << std::endl;
    }

    // Verify log file contents
    std::ifstream logFile("Logs/GameLogs.txt");
    if (logFile.is_open())
    {
        std::cout << "Log file contents:" << std::endl;
        std::string line;
        while (std::getline(logFile, line))
        {
            std::cout << line << std::endl;
        }
        logFile.close();
    }
    else
    {
        std::cerr << "Error: Could not open log file." << std::endl;
    }

    // Cleanup
    delete processor;
    delete ordersList;
    delete engine;
    // Don't delete logger - it's managed by the singleton
    // LogObserver::destroyInstance() will be called at program end

    std::cout << "Check Logs/GameLogs.txt for results." << std::endl;
}

/*
int main()
{
    testLoggingObserver();
    return 0;
}
    */
