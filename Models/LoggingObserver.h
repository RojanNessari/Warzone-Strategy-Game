#pragma once
#include <string>
#include <list>
#include <fstream>

// Interface for loggable objects
class ILoggable {
public:
    virtual std::string stringToLog() = 0;
    virtual ~ILoggable() = default;
};

// Base observer class
class Observer {
public:
    virtual void Update(ILoggable* loggable) = 0;
    virtual ~Observer() = default;
};

// Base subject class
class Subject {
private:
    std::list<Observer*>* observers;
public:
    Subject();
    virtual ~Subject();
    virtual void Attach(Observer* o);
    virtual void Detach(Observer* o);
    virtual void Notify(ILoggable* loggable);
};


class LogObserver : public Observer {
public:
    LogObserver();
    ~LogObserver();
    void Update(ILoggable* loggable) override;
};
#pragma once
