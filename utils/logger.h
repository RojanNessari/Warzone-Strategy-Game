#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum LogLevel
{
    DEBUG,
    INFO,
    ERROR,
    WARNING
};

void logMessage(LogLevel level, const std::string &message);

#endif