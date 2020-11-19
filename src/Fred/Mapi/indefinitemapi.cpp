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
    this->request = make_pair(string(""), false);
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

    lock_guard<mutex> lock(this->requestAccess);
    pair<string, bool> request = this->request;
    this->request = make_pair(string(""), false);
    return request.second ? request.first : "";
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
    return this->request.second;
}

string IndefiniteMapi::getRequest()
{
    lock_guard<mutex> lock(this->requestAccess);
    pair<string, bool> request = this->request;
    this->request = make_pair(string(""), false);
    return request.second ? request.first : "";
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
    lock_guard<mutex> lock(this->requestAccess);
    this->request = make_pair(request, true);
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
