#include <iostream>
#include <string>
#include "../Models/CommandProcessor.h"
#include "../Models/GameEngine.h"
using namespace std;
string LINE(30, '=');

void testCommandProcessor()
{ /*
  testCommandProcessor()
  _description_:
      1. Commands can be read using CommandProcessor class from the console
      2. Commands can be read from text file using CommandProcessorAdapter
      3. commands that are invalid in the current game state are rejected, and valid commands result in  the correct effect and state change
  */
    cout << LINE << endl;
    cout << "Testing CommandProcessor Module" << endl;
    cout << LINE << endl;

    // Step 1: Initialize Game Engine
    GameEngine engine;

    // Step 2: Test Console Input

    cout << "\n[Console Input Test]" << endl;
    CommandProcessor consoleProcessor;
    Command *cmd_1 = consoleProcessor.getCommand();
    if (cmd_1)
    {
        if (consoleProcessor.validate(cmd_1, &engine))
        {
            cmd_1->saveEffect("Command " + cmd_1->getCommand() + " executed successfully.");
        }
        else
        {
            cmd_1->saveEffect("Invalid command for current game " + cmd_1->getCommand());
        }
        cout << *cmd_1 << endl;
    }

    // step 3: Test File Input
    cout << "\n[File Input Test]" << endl;
    string testFileName = "test_commands.txt";
    FileCommandProcessorAdapter fileProcessor(testFileName);
    Command *cmd_2 = fileProcessor.getCommand(); // read from file
    if (cmd_2)
    {
        if (fileProcessor.validate(cmd_2, &engine))
        {
            cmd2->saveEffect("Command executed successfully.");
        }
        else
        {
            cmd2->saveEffect("Invalid command for the current game state.");
        }
        cout << *cmd_2 << endl;
    }
    // Step 4: Test Invalid Commands
    cout << "\n[Invalid Command Test]" << endl;
    Command invalidCommand('InvalidCommand');
    if (!consoleProcessor.validate(&invalidCommand, &gameEngine))
    {
        invalidCommand.saveEffect("Invalid command for the current game state.");
    }
    cout << invalidCommand << endl;

    cout << LINE << endl;
    cout << "CommandProcessor Module Test Completed" << endl;
    cout << LINE << endl;
}

int main()
{
    testCommandProcessor();
    return 0;
}