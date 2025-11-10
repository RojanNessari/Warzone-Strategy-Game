#include <iostream>
#include "LoggingObserver.h"
using namespace std;

// test class 
class TestSubject : public Subject, public ILoggable {
private:
    string message;
public:
    TestSubject(string msg) : message(msg) {}
    void changeMessage(string newMsg) {
        message = newMsg;
        Notify(this); // notify observer
    }
    string stringToLog() override {
        return "TestSubject message: " + message;
    }
};

// Driver function
void testLoggingObserver() {
    cout << "Testing LoggingObserver..." << endl;

    LogObserver* logger = new LogObserver();
    TestSubject* subject = new TestSubject("Initial");

    subject->Attach(logger); // attach observer

    // trigger logs
    subject->changeMessage("LoadMap executed");
    subject->changeMessage("Map validated");
    subject->changeMessage("Game state: Reinforcement Phase");

    cout << "Check gamelog.txt" << endl;

    delete subject;
    delete logger;
}

int main() {
    testLoggingObserver();
    return 0;
}
