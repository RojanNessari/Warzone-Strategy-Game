#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// ANSI color codes
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";

enum LogLevel
{
    DEBUG,
    INFO,
    ERROR,
    WARNING
};

void logMessage(LogLevel level, const std::string &message);

#endif