#include <iostream>
#include <string>
#include "../Models/CommandProcessing.h"
#include "../Models/GameEngine.h"
#include "../utils/logger.h"
using namespace std;

const string CONSOLE_MODE = "-console";
const string FILE_MODE = "-file";

string mode;
string file_name;

void setMode(const string &inputMode)
{
    mode = inputMode;
}
string getMode()
{
    return mode;
}

void setFileName(const string &inputFileName)
{
    file_name = inputFileName;
}

string getFileName()
{
    return file_name;
}

void testCommandProcessor()
{
    // Step 1: Initialize Game Engine
    GameEngine engine;
    engine.buildGraph();
    logMessage(INFO, "Game Engine initialized and graph built.");

    // Step 2: Get current Mode
    string currentMode = getMode();

    if (currentMode == CONSOLE_MODE)
    {
        CommandProcessor consoleProcessor;
        while (true)
        {
            Command *cmd = consoleProcessor.getCommand();

            if (cmd)
            {
                if (consoleProcessor.validate(cmd, &engine))
                {
                    cmd->saveEffect("Command " + cmd->getCommand() + " executed successfully.");
                }
                else
                {
                    if (cmd->getCommand() == "terminate")
                    {
                        logMessage(INFO, "Terminating Test per User Request.");
                        break;
                    }
                    cmd->saveEffect("Invalid command for current game " + cmd->getCommand());
                }
                logMessage(INFO, cmd->getCommand() + " - " + cmd->getEffect());
            }
        }
    }
    else if (currentMode == FILE_MODE)
    {
        string testFileName = getFileName();
        FileCommandProcessorAdapter fileProcessor(testFileName);
        while (true)
        {
            Command *cmd = fileProcessor.getCommand(); // read from file
            if (cmd == nullptr)
            {
                break; // end of file
            }
            if (cmd)
            {
                if (fileProcessor.validate(cmd, &engine))
                {
                    cmd->saveEffect("Command executed successfully.");
                }
                else
                {
                    cmd->saveEffect("Invalid command for the current game state.");
                }
                logMessage(INFO, cmd->getCommand() + " - " + cmd->getEffect());
            }
        }
    }
    else
    {
        CommandProcessor consoleProcessor;
        logMessage(INFO, "[Invalid Command Test]");
        Command invalidCommand("InvalidCommand");
        if (!consoleProcessor.validate(&invalidCommand, &engine))
        {
            invalidCommand.saveEffect("Invalid command for the current game state.");
        }
        logMessage(INFO, invalidCommand.getCommand() + " - " + invalidCommand.getEffect());
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        logMessage(ERROR, "Usage: " + string(argv[0]) + " -console OR -file <filename>");
        return 1;
    }

    string mode = argv[1];
    setMode(mode); // set mode globally

    if (mode == CONSOLE_MODE)
    {
        testCommandProcessor();
    }
    else if (mode == FILE_MODE && argc >= 3)
    {
        string filename = argv[2];
        logMessage(INFO, "Running in file mode with: " + filename);
        logMessage(INFO, "[Reading commands from file: " + filename + "]");
        setFileName(filename);
        testCommandProcessor();
    }
    else
    {
        logMessage(ERROR, "Invalid arguments!");
        logMessage(ERROR, "Usage: " + string(argv[0]) + " -console OR -file <filename>");
        return 1;
    }

    return 0;
}