#include "LoggingObserver.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <ctime>   // For timestamp generation
#include <iomanip> // For formatting the timestamp
#include <sstream> // For std::ostringstream

const std::string LOGGER_PATH_FILE = "Logs/gamelog.log";

// Static member initialization
LogObserver *LogObserver::instance = nullptr;

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
    // Auto-attach global logger if it exists
    LogObserver *globalLogger = LogObserver::getInstance();
    if (globalLogger)
    {
        observers->push_back(globalLogger);
    }
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

void Subject::Notify(ILoggable *loggable, LogLevel level, std::string messageType)
{
    for (Observer *o : *observers)
        o->Update(loggable, level, messageType);
}
// Const version of Notify
void Subject::Notify(const ILoggable *loggable, LogLevel level, std::string messageType) const
{
    for (Observer *o : *observers)
        o->Update(const_cast<ILoggable *>(loggable), level, messageType);
}

// LogObserver methods
LogObserver::LogObserver()
{
    // Ensure the Logs directory exists when observer is created
    ensureDirectoryExists(LOGGER_PATH_FILE);
}

LogObserver::~LogObserver() {}

// Singleton methods
LogObserver *LogObserver::getInstance()
{
    if (instance == nullptr)
    {
        instance = new LogObserver();
    }
    return instance;
}

void LogObserver::destroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void LogObserver::Update(ILoggable *loggable, LogLevel level, std::string messageType)
{
    // Get the current time
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);

    // Format the timestamp
    std::ostringstream timestamp;
    timestamp << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    std::string color;
    if (level == DEBUG)
    {
        color = CYAN;
    }
    else if (level == INFO)
    {
        color = GREEN;
    }
    else if (level == ERROR)
    {
        color = RED;
    }
    else if (level == WARNING)
    {
        color = YELLOW;
    }
    else if (level == ANTICHEAT)
    {
        color = MAGENTA;
    }
    else if (level == AI)
    {
        color = BLUE;
    }
    else if (level == INVENTORY)
    {
        color = CYAN;
    }
    else if (level == COMBAT)
    {
        color = RED;
    }
    else if (level == PROGRESSION)
    {
        color = GREEN;
    }
    else if (level == INPUT)
    {
        color = GREEN;
    }
    else if (level == EVENT)
    {
        color = MAGENTA;
    }
    else if (level == HUMAN)
    {
        color = BLUE;
    }
    else
    {
        color = RESET;
    }

    // Convert LogLevel to string
    std::string levelStr;
    switch (level)
    {
    case DEBUG:
        levelStr = "DEBUG";
        break;
    case INFO:
        levelStr = "INFO";
        break;
    case ERROR:
        levelStr = "ERROR";
        break;
    case WARNING:
        levelStr = "WARNING";
        break;
    case ANTICHEAT:
        levelStr = "ANTICHEAT";
        break;
    case AI:
        levelStr = "AI";
        break;
    case HUMAN:
        levelStr = "HUMAN";
        break;
    case INVENTORY:
        levelStr = "INVENTORY";
        break;
    case COMBAT:
        levelStr = "COMBAT";
        break;
    case PROGRESSION:
        levelStr = "PROGRESSION";
        break;
    case REPLAY:
        levelStr = "REPLAY";
        break;
    case INPUT:
        levelStr = "INPUT";
        break;
    case EVENT:
        levelStr = "EVENT";
        break;
    default:
        levelStr = "UNKNOWN";
        break;
    }

    // Format the log entry
    std::ostringstream logEntry;
    logEntry << "[" << timestamp.str() << "] [" << levelStr << "] " << messageType;

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

// Helper method to log directly to file without needing ILoggable
void LogObserver::logToFile(LogLevel level, const std::string &message)
{
    Update(nullptr, level, message);
}
