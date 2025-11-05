#include <iostream>
#include <fstream>
#include <sstream>
#include "CommandProcessor.h"
#include "GameEngine.h"
#include "Player.h"

using namespace std;

// ============================================================================
// Command Class Implementation
// ============================================================================

// Constructor
Command::Command(const string &commandText) : command(commandText), effect("") {}

// Copy constructor
Command::Command(const Command &otherCommand)
    : command(otherCommand.command), effect(otherCommand.effect) {}

// Assignment operator
Command &Command::operator=(const Command &otherCommand)
{
    // Check for self-assignment
    if (this != &otherCommand)
    {
        // Copy both members
        command = otherCommand.command;
        effect = otherCommand.effect;
    }
    return *this;
}

// Destructor
Command::~Command() {}

// Get the command text
string Command::getCommand() const
{
    return command;
}

// Get the effect text
string Command::getEffect() const
{
    return effect;
}

// This method stores the result/effect after a command is executed
// Example: After executing "loadmap conquest.map", the effect might be:
//          "Map 'conquest.map' loaded successfully" or "Error: Map file not found"
void Command::saveEffect(const string &effectText)
{

    effect = effectText; // Store the effect string
    // Display message
    cout << "Effect saved: " << effectText << endl;
}

// Stream insertion operator for Command
ostream &operator<<(ostream &os, const Command &cmd)
{
    os << "Command: " << cmd.command;
    if (!cmd.effect.empty())
    {
        os << " | Effect: " << cmd.effect;
    }
    return os;
}

// ============================================================================
// FileLineReader Class Implementation
// ============================================================================

// Constructor - opens the file
FileLineReader::FileLineReader(const string &fileName)
    : fileName(fileName) {}

// Copy constructor
FileLineReader::FileLineReader(const FileLineReader &other)
    : fileName(other.fileName) {}

// Assignment operator
FileLineReader &FileLineReader::operator=(const FileLineReader &other)
{
    if (this != &other)
    {
        fileName = other.fileName;
    }
    return *this;
}

// Destructor
FileLineReader::~FileLineReader() {}

string FileLineReader::readLineFromFile()
{
    ifstream file(fileName);
    string line;

    if (file.is_open())
    {
        if (getline(file, line))
        {
            file.close();
            return line;
        }
        file.close();
    }

    return ""; // Return empty string if file can't be read or EOF
}

// Stream insertion operator for FileLineReader
ostream &operator<<(ostream &os, const FileLineReader &reader)
{
    os << "FileLineReader reading from: " << reader.fileName;
    return os;
}

// ============================================================================
// CommandProcessor Class Implementation
// ============================================================================

// Constructor
CommandProcessor::CommandProcessor() : commands() {}

// Copy constructor
CommandProcessor::CommandProcessor(const CommandProcessor &otherProcessor)
{
    // Deep copy of all commands
    for (Command *cmd : otherProcessor.commands)
    {
        commands.push_back(new Command(*cmd));
    }
}

// Assignment operator
CommandProcessor &CommandProcessor::operator=(const CommandProcessor &otherProcessor)
{
    if (this != &otherProcessor)
    {
        // Delete existing commands
        for (Command *cmd : commands)
        {
            delete cmd;
        }
        commands.clear();

        // Deep copy new commands
        for (Command *cmd : otherProcessor.commands)
        {
            commands.push_back(new Command(*cmd));
        }
    }
    return *this;
}

// Destructor
CommandProcessor::~CommandProcessor()
{
    // Delete all dynamically allocated commands
    for (Command *cmd : commands)
    {
        delete cmd;
    }
    commands.clear();
}

// This method reads a command string from the console (cin)
// It should NOT take any parameters - it reads directly from user input
string CommandProcessor::readCommand()
{
    string commandLine;
    cout << "Enter Command: ";
    getline(cin, commandLine); // Read the entire line include spaces
    return commandLine;
}

// This method is the main public interface - it:
// 1. Reads a command (using readCommand())
// 2. Creates a Command object
// 3. Saves it (using saveCommand())
// 4. Returns it to the caller
Command *CommandProcessor::getCommand()
{
    // Step 1: read command string from console
    string cmdText = readCommand();
    // Fall back check
    if (cmdText.empty())
    {
        return nullptr;
    }
    // Step 2: Create a new Command Object
    Command *cmd = new Command(cmdText);

    // Step 3: Save the Command in the collection
    saveCommand(cmd);

    // Step 4: Return the command
    return cmd;
}

// This method stores a command in the internal collection
void CommandProcessor::saveCommand(Command *cmd)
{
    if (cmd != nullptr)
    {
        commands.push_back(cmd); // add to vetor
        cout << "Command saved: " << cmd->getCommand() << endl;
    }
}

// This method checks if a command is valid in the current game state
// It needs to implement the state transition table from your requirements:
//
// Command              | Valid in State              | Transition to
// ---------------------|-----------------------------|-----------------
// loadmap <mapfile>    | start, maploaded           | maploaded
// validatemap          | maploaded                  | mapvalidated
// addplayer <name>     | mapvalidated, playersadded | playersadded
// gamestart            | playersadded               | assignreinforcement
// replay               | win                        | start
// quit                 | win                        | exit program

bool CommandProcessor::validate(Command *cmd, GameEngine *engine)
{ // Fall back check
    if (cmd == nullptr || engine == nullptr)
    {
        return false;
    }

    // Get Current State name from engine
    string currentState = engine->current()->getName();

    // Get Cmd text and extract cmd name
    string cmdTxt = cmd->getCommand();
    istringstream iss(cmdTxt);
    string commandName;
    iss >> commandName; // Extract first word

    // Validate based on the stat transition table
    if (commandName == "loadmap")
    {
        if (currentState == "start" || currentState == "maploaded")
        {
            cmd->saveEffect("Valid command: loadmap in state " + currentState);
            return true;
        }
    }
    else if (commandName == "validatemap")
    {
        if (currentState == "maploaded")
        {
            cmd->saveEffect("Valid command: validatemap in state " + currentState);
            return true;
        }
    }
    else if (commandName == "addplayer")
    {
        if (currentState == "mapvalidated" || currentState == "playersadded")
        {
            cmd->saveEffect("Valid command: addplayer in state " + currentState);
            return true;
        }
    }
    else if (commandName == "gamestart")
    {
        if (currentState == "playersadded")
        {
            cmd->saveEffect("Valid command: gamestart in state " + currentState);
            return true;
        }
    }
    else if (commandName == "replay")
    {
        if (currentState == "win")
        {
            cmd->saveEffect("Valid command: replay in state " + currentState);
            return true;
        }
    }
    else if (commandName == "quit")
    {
        if (currentState == "win")
        {
            cmd->saveEffect("Valid command: quit in state " + currentState);
            return true;
        }
    }
    else
    {
        cmd->saveEffect("Error: Unknown command '" + commandName + "'");
        return true;
    }
    // If we get here, command was invalid for current state
    cmd->saveEffect("Error: Command '" + commandName + "' is not valid in state '" + currentState + "'");
    return false;
}

// Stream insertion operator for CommandProcessor
ostream &operator<<(ostream &os, const CommandProcessor &cp)
{
    os << "CommandProcessor with " << cp.commands.size() << " command(s):" << endl;
    for (size_t i = 0; i < cp.commands.size(); ++i)
    {
        os << "  [" << i << "] " << *(cp.commands[i]) << endl;
    }
    return os;
}

// ============================================================================
// FileCommandProcessorAdapter Class Implementation
// ============================================================================

// Constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string &fileName)
    : CommandProcessor(), fileReader(new FileLineReader(fileName))
{
    cout << "FileCommandProcessorAdapter created for file: " << fileName << endl;
}

// Copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &otherFileCmdPro)
    : CommandProcessor(otherFileCmdPro)
{
    // Deep copy the file reader
    if (otherFileCmdPro.fileReader != nullptr)
    {
        fileReader = new FileLineReader(*(otherFileCmdPro.fileReader));
    }
    else
    {
        fileReader = nullptr;
    }
}

// Assignment operator
FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &otherFileCmdPro)
{
    if (this != &otherFileCmdPro)
    {
        // Call base class assignment operator
        CommandProcessor::operator=(otherFileCmdPro);

        // Delete existing file reader
        delete fileReader;

        // Deep copy new file reader
        if (otherFileCmdPro.fileReader != nullptr)
        {
            fileReader = new FileLineReader(*(otherFileCmdPro.fileReader));
        }
        else
        {
            fileReader = nullptr;
        }
    }
    return *this;
}

// Destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete fileReader;
}

string FileCommandProcessorAdapter::readCommand()
{
    if (fileReader != nullptr)
    {
        // Use the FileLineReader to get next line from file
        string line = fileReader->readLineFromFile();

        if (!line.empty())
        {
            cout << "Read from file: " << line << endl;
            return line;
        }
        else
        {
            cout << "End of file or empty line" << endl;
            return "";
        }
    }
    return ""; // No file reader available
}
// Stream insertion operator for FileCommandProcessorAdapter
ostream &operator<<(ostream &os, const FileCommandProcessorAdapter &adapter)
{
    os << "FileCommandProcessorAdapter:" << endl;
    if (adapter.fileReader != nullptr)
    {
        os << "  " << *(adapter.fileReader) << endl;
    }
    os << "  Commands: " << adapter.commands.size();
    return os;
}