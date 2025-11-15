#include "LoggingObserver.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "logger.h"
#include <ctime>   // For timestamp generation
#include <iomanip> // For formatting the timestamp
#include <sstream> // For std::ostringstream

const std::string LOGGER_PATH_FILE = "Logs/gamelog.txt";

// Helper function to ensure directory exists
static void ensureDirectoryExists(const std::string &filepath)
{
    // Extract directory path
    size_t lastSlash = filepath.find_last_of("/\\");
    if (lastSlash != std::string::npos)
    {
        std::string dir = filepath.substr(0, lastSlash);
// Create directory if it doesn't exist (platform-independent approach)
#ifdef _WIN32
        _mkdir(dir.c_str());
#else
        mkdir(dir.c_str(), 0755);
#endif
    }
}

// Subject methods
Subject::Subject()
{
    observers = new std::list<Observer *>;
}

Subject::~Subject()
{
    delete observers;
}

void Subject::Attach(Observer *o)
{
    observers->push_back(o);
}

void Subject::Detach(Observer *o)
{
    observers->remove(o);
}

void Subject::Notify(ILoggable *loggable, std::string messageType)
{
    for (Observer *o : *observers)
        o->Update(loggable, messageType);
}

// LogObserver methods
LogObserver::LogObserver()
{
    // Ensure the Logs directory exists when observer is created
    ensureDirectoryExists(LOGGER_PATH_FILE);
}
LogObserver::~LogObserver() {}

void LogObserver::Update(ILoggable *loggable, std::string messageType)
{
    // Get the current time
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);

    // Format the timestamp
    std::ostringstream timestamp;
    timestamp << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    std::string color;
    if (messageType == "DEBUG")
    {
        color = CYAN;
    }
    else if (messageType == "INFO")
    {
        color = GREEN;
    }
    else if (messageType == "ERROR")
    {
        color = RED;
    }
    else if (messageType == "WARNING")
    {
        color = YELLOW;
    }
    else if (messageType == "ANTICHEAT")
    {
        color = MAGENTA;
    }
    else if (messageType == "AI")
    {
        color = BLUE;
    }
    else if (messageType == "INVENTORY")
    {
        color = CYAN;
    }
    else if (messageType == "COMBAT")
    {
        color = RED;
    }
    else if (messageType == "PROGRESSION")
    {
        color = GREEN;
    }
    else if (messageType == "INPUT")
    {
        color = GREEN;
    }
    else
    {
        color = RESET;
    }
    // Format the log entry
    std::ostringstream logEntry;
    logEntry << "[" << timestamp.str() << "] [" << messageType << "] " << loggable->stringToLog();

    // Write to the log file
    std::ofstream logFile(LOGGER_PATH_FILE, std::ios::app);
    if (logFile.is_open())
    {
        logFile << logEntry.str() << std::endl;
        logFile.close();
    }
    else
    {
        logMessage(ERROR, "Error: could not open: " + LOGGER_PATH_FILE + "\n");
    }
}
