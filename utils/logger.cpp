#include "logger.h"
#include <iostream>
#include <string>

void logMessage(LogLevel level, const std::string &message)
{
    std::string color;
    std::string prefix;

    switch (level)
    {
    case DEBUG:
        color = CYAN;
        prefix = "[DEBUG]";
        break;

    case INFO:
        color = GREEN;
        prefix = "[INFO]";
        break;

    case ERROR:
        color = RED;
        prefix = "[ERROR]";
        break;

    case WARNING:
        color = YELLOW;
        prefix = "[WARNING]";
        break;

    case ANTICHEAT:
        color = MAGENTA;
        prefix = "[ANTICHEAT]";
        break;

    case AI:
        color = BLUE;
        prefix = "[AI]";
        break;

    case INVENTORY:
        color = CYAN;
        prefix = "[INVENTORY]";
        break;

    case COMBAT:
        color = RED;
        prefix = "[COMBAT]";
        break;

    case EVENT:
        color = MAGENTA;
        prefix = "[EVENT]";
        break;

    case PROGRESSION:
        color = GREEN;
        prefix = "[PROGRESSION]";
        break;

    case REPLAY:
        color = CYAN;
        prefix = "[REPLAY]";
        break;

    case INPUT:
        color = BLUE;
        prefix = "[INPUT]";
        break;

    default:
        color = RESET;
        prefix = "[UNKNOWN]";
        break;
    }

    // Print to stdout except for ERROR
    if (level == ERROR)
        std::cerr << color << prefix << RESET << " " << message << std::endl;
    else
        std::cout << color << prefix << RESET << " " << message << std::endl;
}
