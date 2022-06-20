#include "Fred/Mapi/indefinitemapi.h"
#include "Fred/mappedcommand.h"
#include "Fred/fredtopics.h"
#include "Parser/processmessage.h"
#include "Alfred/print.h"
#include "Fred/queue.h"
#include "Fred/alfrpcinfo.h"
#include <exception>

map<thread::id, IndefiniteMapi*> IndefiniteMapi::mappedThreads;

IndefiniteMapi::IndefiniteMapi()
{
    this->isFinished = false;
    this->executionThread = NULL;
    this->requestRecv = new QueueLock;
    this->sequenceRecv = new QueueLock(1);
    this->responseReceived = false;
}

IndefiniteMapi::~IndefiniteMapi()
{
    this->isFinished = true;
    this->requestRecv->notify();
    this->sequenceRecv->notify();

    if (this->executionThread)
    {
        this->executionThread->join();
        delete this->executionThread;
    }
}

void IndefiniteMapi::registerMapi(Fred* fred, string name)
{
    Mapi::registerMapi(fred, name);
    if (!this->executionThread)
    {
        try
        {
            this->executionThread = new thread(&IndefiniteMapi::doExecution, this);
        }
        catch (const exception& e)
        {
            Print::PrintError(e.what());
            exit(-1);
        }
    }
}

string IndefiniteMapi::processInputMessage(string input)
{
    return input;
}

string IndefiniteMapi::processOutputMessage(string output)
{
    this->response = output;
    this->responseReceived = true;
    this->sequenceRecv->notify();

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

    this->requestRecv->wait();

    size_t numOfRequests;
    {
        lock_guard<mutex> lock(this->requestAccess);
        numOfRequests = this->requests.size();
    }

    if (!numOfRequests)
    {
        if (this->isFinished)
        {
            running = false;
            return "";
        }
    }

    running = true;

    lock_guard<mutex> lock(this->requestAccess);
    string request = this->requests.front();
    this->requests.pop_front();
    return request;
}

bool IndefiniteMapi::isRequestAvailable(bool& running)
{
    if (this->isFinished)
    {
        running = false;
        return false;
    }

    running = true;

    lock_guard<mutex> lock(this->requestAccess);
    return this->requests.size();
}

string IndefiniteMapi::getRequest()
{
    lock_guard<mutex> lock(this->requestAccess);

    this->requestRecv->wait();

    if (!this->requests.size())
    {
        return "";
    }

    string request = this->requests.front();
    this->requests.pop_front();

    return request;
}

string IndefiniteMapi::executeAlfSequence(string sequence)
{
    if (this->thisMapi->fakeLink)
    {
        return sequence;
    }

    if (this_thread::get_id() != this->executionThreadId)
    {
        Print::PrintError("Cannot call executeAlfSequence from different thread");
        return "";
    }

    this->response = "";
    this->responseReceived = false;
    this->sequenceRecv->clear();

    bool useCru = dynamic_cast<MappedCommand*>(this->thisMapi->command)->getUseCru();
    AlfRpcInfo* alfRpcInfo = useCru ? this->thisMapi->alfLink.first : this->thisMapi->alfLink.second;
    if (alfRpcInfo == NULL)
    {
        string error = "Required ALF/CANALF not available!";
        Print::PrintError(name, error);
        return "failure\n" + error;
    }

    ProcessMessage* processMessage = new ProcessMessage(this, sequence, useCru, alfRpcInfo->getVersion());
    Queue* queue = useCru ? this->thisMapi->alfQueue.first : this->thisMapi->alfQueue.second;

    queue->newRequest(make_pair(processMessage, thisMapi));

    this->sequenceRecv->wait();
    if (!this->responseReceived)
    {
        return "";
    }

    string response = this->response;
    this->response = "";
    this->responseReceived = false;
    return response;
}

bool IndefiniteMapi::customMessageProcess()
{
    return true;
}

void IndefiniteMapi::requestReceived(string request)
{
    lock_guard<mutex> lock(this->requestAccess);
    this->requests.push_back(request);
    this->requestRecv->notify();
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
