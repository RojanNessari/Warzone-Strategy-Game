#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum LogLevel
{
    DEBUG,
    INFO,
    ERROR
};

void logMessage(LogLevel level, const std::string &message);

#endif