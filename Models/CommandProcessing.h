#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H

#include <vector>
#include <string>
#include <iostream>
#include "../utils/LoggingObserver.h"

// Forward declarations
class Command;
class GameEngine;
class FileLineReader;

class CommandProcessor : public Subject, public ILoggable
{
public:
    // Constructor
    CommandProcessor();

    // Copy constructor
    CommandProcessor(const CommandProcessor &otherProcessor);

    // Assignment operator
    CommandProcessor &operator=(const CommandProcessor &otherProcessor);

    // Destructor
    ~CommandProcessor();

    // Public method to get a command - returns a Command object
    Command *getCommand();

    // Validate if the command is valid in the current game state
    bool validate(Command *cmd, GameEngine *engine);

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const CommandProcessor &cp);
    // logging function
    std::string stringToLog();

protected:
    // Protected method to read command from console (no parameters - reads from cin)
    virtual std::string readCommand();

    // Collection of commands
    std::vector<Command *> commands;

    // Save a command to the collection
    void saveCommand(Command *cmd);
};

// Adapter Pattern: Adapts CommandProcessor to read from a file instead of console
// This class inherits from CommandProcessor and overrides readCommand() to use FileLineReader
class FileCommandProcessorAdapter : public CommandProcessor
{
public:
    // Constructor - takes filename
    FileCommandProcessorAdapter(const std::string &fileName);

    // Copy constructor
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &otherFileCmdPro);

    // Assignment operator
    FileCommandProcessorAdapter &operator=(const FileCommandProcessorAdapter &otherFileCmdPro);

    // Destructor
    ~FileCommandProcessorAdapter();

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const FileCommandProcessorAdapter &adapter);

private:
    // The adaptee - reads lines from file
    FileLineReader *fileReader;

    // Override readCommand() to read from file instead of console
    std::string readCommand() override;
};

// Helper class to read lines from a file (Adaptee in Adapter pattern)
class FileLineReader
{
public:
    // Constructor - opens file
    FileLineReader(const std::string &fileName);

    // Copy constructor
    FileLineReader(const FileLineReader &other);

    // Assignment operator
    FileLineReader &operator=(const FileLineReader &other);

    // Destructor - closes file
    ~FileLineReader();

    // Read next line from file
    std::string readLineFromFile();

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const FileLineReader &reader);

private:
    std::string fileName;
    std::ifstream *fileStream; // Keep file open between reads
    int currentLine;           // Track current line number
};

// Command class - represents a command with its text and execution effect
class Command : public Subject, public ILoggable
{
public:
    // Constructor - creates command with given text
    Command(const std::string &commandText);

    // Copy constructor
    Command(const Command &otherCommand);

    // Assignment operator
    Command &operator=(const Command &otherCommand);

    // Destructor
    ~Command();

    // Save the effect of executing this command
    void saveEffect(const std::string &effectText);

    // Get the command text
    std::string getCommand() const;

    // Get the effect text
    std::string getEffect() const;

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Command &cmd);

    std::string stringToLog();

    std::vector<std::string> tournamentMaps;
    std::vector<std::string> tournamentStrategies;
    int tournamentGames = 0;
    int tournamentMaxTurns = 0;

private:
    std::string command; // The command text
    std::string effect;  // The effect after execution
};

#endif