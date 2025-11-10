#include "logger.h"
#include <iostream>
#include <string>

// ANSI color codes
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";

void logMessage(LogLevel level, const std::string &logMessage)
{
    switch (level)
    {
    case DEBUG:
        std::cout << YELLOW << "[DEBUG] " << RESET << logMessage << std::endl;
        break;
    case INFO:
        std::cout << GREEN << "[INFO] " << RESET << logMessage << std::endl;
        break;
    case ERROR:
        std::cerr << RED << "[ERROR] " << RESET << logMessage << std::endl;
        break;
    }
}