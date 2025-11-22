#pragma once
#include <string>
#include <list>
#include <fstream>
#include "logger.h"

// Interface for loggable objects
class ILoggable
{
public:
    virtual ~ILoggable() = default;
};

// Base observer class
class Observer
{
public:
    virtual void Update(ILoggable *loggable, LogLevel level, std::string messageType) = 0;

    virtual ~Observer() = default;
};

// Base subject class
class Subject
{
private:
    std::list<Observer *> *observers;

public:
    Subject();
    virtual ~Subject();
    virtual void Attach(Observer *o);
    virtual void Detach(Observer *o);
    virtual void Notify(ILoggable *loggable, LogLevel level, std::string messageType);
    virtual void Notify(const ILoggable *loggable, LogLevel level, std::string messageType) const;
};

class LogObserver : public Observer
{
public:
    LogObserver();
    ~LogObserver();
    void Update(ILoggable *loggable, LogLevel level, std::string messageType) override;

    // Singleton instance
    static LogObserver *getInstance();
    static void destroyInstance();

private:
    static LogObserver *instance;
};
#pragma once
