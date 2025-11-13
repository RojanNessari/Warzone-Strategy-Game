#include "logger.h"
#include <iostream>
#include <string>

void logMessage(LogLevel level, const std::string &logMessage)
{
    switch (level)
    {
    case DEBUG:
        std::cout << BLUE << "[DEBUG] " << RESET << logMessage << std::endl;
        break;
    case INFO:
        std::cout << GREEN << "[INFO] " << RESET << logMessage << std::endl;
        break;
    case ERROR:
        std::cerr << RED << "[ERROR] " << RESET << logMessage << std::endl;
        break;
    case WARNING:
        std::cout << YELLOW << "[WARNING]" << RESET << logMessage << std::endl;
        break;
    }
}