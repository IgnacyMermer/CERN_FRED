#include "Fred/Mapi/indefinitemapi.h"
#include "Fred/mappedcommand.h"
#include "Fred/fredtopics.h"
#include "Parser/processmessage.h"
#include "Alfred/print.h"
#include "Fred/queue.h"
#include <exception>

map<thread::id, IndefiniteMapi*> IndefiniteMapi::mappedThreads;

IndefiniteMapi::IndefiniteMapi()
{
    this->isFinished = false;
    this->executionThread = NULL;
}

IndefiniteMapi::~IndefiniteMapi()
{
    this->isFinished = true;
    this->requestRecv.notify_one();

    if (this->executionThread)
    {
        this->executionThread->join();
        delete this->executionThread;
    }
}

void IndefiniteMapi::registerMapi(Fred* fred, string name)
{
    Mapi::registerMapi(fred, name);
    this->executionThread = new thread(&IndefiniteMapi::doExecution, this);
}

string IndefiniteMapi::processInputMessage(string input)
{
    return input;
}

string IndefiniteMapi::processOutputMessage(string output)
{
    this->response = output;
    this->sequenceRecv.notify_one();

    return "";
}

void IndefiniteMapi::doExecution()
{
    this->executionThreadId = this_thread::get_id();
    IndefiniteMapi::mappedThreads[this->executionThreadId] = this;

    while (!this->isFinished)
    {
        try
        {
            this->processExecution();
        }
        catch (const exception& e)
        {
            this->publishError(string("Error in process execution: ") + e.what());
        }
    }
}

string IndefiniteMapi::waitForRequest(bool &running)
{
    if (this_thread::get_id() != this->executionThreadId)
    {
        Print::PrintError("Cannot call waitForRequest from different thread");
        return "";
    }

    mutex requestMutex;
    unique_lock<mutex> requestLock(requestMutex);

    this->requestRecv.wait(requestLock);

    if (this->isFinished)
    {
        running = false;
        return "";
    }

    running = true;
    string request = this->request;
    this->request = "";
    return request;
}

string IndefiniteMapi::executeAlfSequence(string sequence)
{
    if (this_thread::get_id() != this->executionThreadId)
    {
        Print::PrintError("Cannot call executeAlfSequence from different thread");
        return "";
    }

    mutex sequenceMutex;
    unique_lock<mutex> sequenceLock(sequenceMutex);

    bool useCru = dynamic_cast<MappedCommand*>(this->thisMapi->command)->getUseCru();
    ProcessMessage* processMessage = new ProcessMessage(this, sequence, useCru);
    Queue* queue = useCru ? this->thisMapi->alfQueue.first : this->thisMapi->alfQueue.second;
    if (!queue)
    {
        string error = "Required ALF/CANALF not available!";
        Print::PrintError(name, error);
        delete processMessage;
        return "failure\n" + error;
    }

    queue->newRequest(make_pair(processMessage, thisMapi));

    this->sequenceRecv.wait(sequenceLock);

    string response = this->response;
    this->response = "";
    return response;
}

bool IndefiniteMapi::customMessageProcess()
{
    return true;
}

void IndefiniteMapi::requestReceived(string request)
{
    this->request = request;
    this->requestRecv.notify_one();
}

IndefiniteMapi* IndefiniteMapi::getCurrentMapi()
{
    thread::id currentId = this_thread::get_id();
    if (IndefiniteMapi::mappedThreads.count(currentId))
    {
        return IndefiniteMapi::mappedThreads[currentId];
    }

    return NULL;
}
