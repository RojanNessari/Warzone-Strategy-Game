#include "LoggingObserver.h"
#include "logger.h" // provides logMessage() and LogLevel enum
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <iostream>

using namespace std;

static const string LOGGER_PATH_FILE = "Logs/gamelog.txt";

// Static storage helpers
vector<Observer *> &Subject::getObservers()
{
    static vector<Observer *> observers;
    return observers;
}
mutex &Subject::getMutex()
{
    static mutex m;
    return m;
}

void Subject::Attach(Observer *o)
{
    if (!o) return;
    lock_guard<mutex> lg(getMutex());
    getObservers().push_back(o);
}

void Subject::Detach(Observer *o)
{
    lock_guard<mutex> lg(getMutex());
    auto &obs = getObservers();
    obs.erase(remove(obs.begin(), obs.end(), o), obs.end());
}

void Subject::Notify(const ILoggable *loggable, const std::string &messageType) const
{
    if (!loggable) return;
    lock_guard<mutex> lg(getMutex());
    for (auto *o : getObservers())
        if (o)
            o->Update(loggable, messageType);
}

// Helper: ensure Logs directory exists
static void ensureDirectoryExists(const string &filepath)
{
    try {
        filesystem::path p(filepath);
        auto parent = p.parent_path();
        if (!parent.empty() && !filesystem::exists(parent))
            filesystem::create_directories(parent);
    } catch (...) {
        // best effort, do not throw
    }
}

static string currentTimestamp()
{
    auto now = time(nullptr);
    tm tm_buf;
    localtime_r(&now, &tm_buf);
    ostringstream ss;
    ss << put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

static LogLevel toLogLevel(const std::string &messageType)
{
    if (messageType == "DEBUG") return DEBUG;
    if (messageType == "INFO") return INFO;
    if (messageType == "ERROR") return ERROR;
    if (messageType == "WARNING") return WARNING;
    if (messageType == "ANTICHEAT") return ANTICHEAT;
    if (messageType == "AI") return AI;
    if (messageType == "INVENTORY") return INVENTORY;
    if (messageType == "COMBAT") return COMBAT;
    if (messageType == "PROGRESSION") return PROGRESSION;
    if (messageType == "INPUT") return INPUT;
    return INFO;
}

LogObserver::LogObserver()
{
    ensureDirectoryExists(LOGGER_PATH_FILE);
}

LogObserver::~LogObserver() {}

void LogObserver::Update(const ILoggable *loggable, const std::string &messageType)
{
    if (!loggable) return;

    string text = loggable->stringToLog();
    string ts = currentTimestamp();
    string entry = "[" + ts + "] [" + messageType + "] " + text;

    // 1) Console via colorized logMessage() (from logger.h/.cpp)
    LogLevel lvl = toLogLevel(messageType);
    logMessage(lvl, text); // prints to console

    // 2) Append to file
    ofstream ofs(LOGGER_PATH_FILE, ios::app);
    if (ofs.is_open())
    {
        ofs << entry << "\n";
        ofs.close();
    }
    else
    {
        // fallback console error
        logMessage(ERROR, "Failed to open log file: " + LOGGER_PATH_FILE);
    }
}