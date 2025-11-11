#include "LoggingObserver.h"
#include <iostream>
#include <string>

const std::string LOGGER_PATH_FILE = "Logs/GameLogs.txt";

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

void Subject::Notify(ILoggable *loggable)
{
    for (Observer *o : *observers)
        o->Update(loggable);
}

// LogObserver methods
LogObserver::LogObserver() {}
LogObserver::~LogObserver() {}

void LogObserver::Update(ILoggable *loggable)
{
    std::ofstream logFile(LOGGER_PATH_FILE, std::ios::app);
    if (logFile.is_open())
    {
        logFile << loggable->stringToLog() << std::endl;
        logFile.close();
    }
    else
    {
        std::cerr << "Error: could not open: " << LOGGER_PATH_FILE << std::endl;
    }
}
