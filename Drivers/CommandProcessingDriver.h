#ifndef COMMAND_PROCESSING_DRIVER_H
#define COMMAND_PROCESSING_DRIVER_H
#include <string>

const std::string CONSOLE_MODE = "-console";
const std::string FILE_MODE = "-file";
void testCommandProcessor();
void setMode(const std::string &inputMode);
void setFileName(const std::string &inputFileName);
std::string getFileName();

#endif