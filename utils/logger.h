#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// ANSI color codes
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";

enum LogLevel
{
    DEBUG,
    INFO,
    ERROR,
    WARNING,
    ANTICHEAT,
    AI,
    INVENTORY,
    COMBAT,
    EVENT,
    PROGRESSION,
    REPLAY,
    INPUT,
    HUMAN

};

void logMessage(LogLevel level, const std::string &message);

#endif