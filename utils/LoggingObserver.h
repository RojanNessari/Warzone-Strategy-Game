#pragma once
#include <string>
#include <vector>
#include <mutex>

// Interface for loggable objects
class ILoggable
{
public:
    virtual std::string stringToLog() const = 0;
    virtual ~ILoggable() = default;
};

// Observer interface
class Observer
{
public:
    virtual void Update(const ILoggable *loggable, const std::string &messageType) = 0;
    virtual ~Observer() = default;
};

// Subject base: single global observer list so any Subject::Notify() reaches attached observers.
class Subject
{
public:
    static void Attach(Observer *o);
    static void Detach(Observer *o);

protected:
    // Instances call this to notify all attached observers
    void Notify(const ILoggable *loggable, const std::string &messageType) const;

private:
    static std::vector<Observer *> &getObservers();
    static std::mutex &getMutex();
};

// Concrete logging observer: prints to console (via logMessage) and appends to Logs/gamelog.txt
class LogObserver : public Observer
{
public:
    LogObserver();
    ~LogObserver() override;
    void Update(const ILoggable *loggable, const std::string &messageType) override;
};