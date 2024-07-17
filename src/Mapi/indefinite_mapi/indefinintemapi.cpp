#include "Parser/utility.h"
#include "Alfred/print.h"
#include "indefinite_mapi/indefinitemapi.h"

// Constructor
TestIndefMapi::TestIndefMapi() : IndefiniteMapi::IndefiniteMapi()
{
}

// Destructor
TestIndefMapi::~TestIndefMapi()
{
}

void TestIndefMapi::processExecution()
{
    bool running;
    string response;

    string request = this->waitForRequest(running); // Wait for incoming request from WinCC
    if (!running)
    {
        return;
    }

    if (request == "test")
    {
        this->publishAnswer("Testing");
    }
    else if (request == "error")
    {
        this->publishError("Error!");
    }
    else
    {

        response = this->executeAlfSequence("read\n0x00000170,0x80000000"); // execute desired sequence to alf, waits for response from ALF
        this->publishAnswer(response);

        response = this->executeAlfSequence("read\n0x00000180,0x80000000"); // It is possible to execute multiple sequences to ALF with one command from WinCC
        this->publishAnswer(response);
    }
}