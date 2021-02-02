#include "Fred/queue.h"
#include "Fred/alfrpcinfo.h"
#include "Fred/alfinfo.h"
#include "Alfred/print.h"
#include "Fred/llalock.h"

Queue::Queue(Fred* fred)
{
    this->fred = fred;
    this->isFinished = false;
    this->isProcessing = false;
    this->llaLock = NULL;
    this->queueThread = new thread(clearQueue, this);
}

Queue::~Queue()
{
    this->isFinished = true;
    conditionVar.notify_one();
    queueThread->join();
    delete queueThread;
}

void Queue::clearQueue(Queue *queue)
{
    mutex lock;
    unique_lock<mutex> uniqueLock(lock);

    while (1)
    {
        if (queue->isFinished)
        {
            return;
        }

        queue->conditionVar.wait(uniqueLock);

        while (!queue->stack.empty())
        {
            queue->isProcessing = true;

            pair<ProcessMessage*, ChainTopic*> request;
            {
                lock_guard<mutex> lockGuard(queue->stackMutex);
                request = queue->stack.front();
                queue->stack.pop_front();
            }
            //do processing
            AlfRpcInfo* alfLink = request.first->getUseCru() ? request.second->alfLink.first : request.second->alfLink.second;
            alfLink->setTransaction(request);  //CAN bus backup

            Print::PrintVerbose(request.second->name, "Parsing message");

            vector<string> fullMessage;

            try
            {
                if (request.second->mapi == NULL)
                {
                    fullMessage = request.first->generateFullMessage(*request.second->instruction);
                }
                else
                {
                    fullMessage = vector<string>({ request.first->generateMapiMessage() });
                }
            }
            catch (exception& e)
            {
                string errorMessage = e.what();
                Print::PrintError(request.second->name, errorMessage);

                request.second->error->Update(errorMessage);
                Print::PrintError(request.second->name, "Updating error service!");

                queue->isProcessing = false;
                continue;
            }

            if (request.second->mapi != NULL && request.second->mapi->noRpcRequest)
            {
                Print::PrintVerbose(request.second->name, "Skipping RPC request");
                request.second->mapi->noRpcRequest = false; //reset noRpcRequest 
                queue->isProcessing = false;
                continue;
            }
            else
            {
                bool errorOccured = false;

                for (size_t i = 0; i < fullMessage.size() && !errorOccured; i++)
                {
                    char* buffer = strdup(fullMessage[i].c_str());

                    do
                    {
                        if (!queue->checkLlaStartSession())
                        {
                            Print::PrintError(request.second->name, "Error starting LLA session!");
                            request.second->error->Update("Error starting LLA session!");
                            errorOccured = true;
                            continue;
                        }

                        Print::PrintVerbose(request.second->name, "Sending RPC request:\n" + string(buffer));
                        alfLink->Send(buffer);

                        errorOccured = request.first->getPollPattern()->at(i) == "ERROR";
                    }
                    while (!request.first->getPollPattern()->at(i).empty() && !errorOccured);

                    free(buffer);
                }
            }

            queue->isProcessing = false;
        }

        queue->checkLlaStopSession(); //if no request in any queue, stop LLA session
    }
}

void Queue::newRequest(pair<ProcessMessage*, ChainTopic*> request)
{
    {
        lock_guard<mutex> lockGuard(stackMutex);
        stack.push_back(request);
    }

    if (!isProcessing)
    {
        conditionVar.notify_one();
    }
}

size_t Queue::getStackSize()
{
    lock_guard<mutex> lockGuard(stackMutex);
    return stack.size();
}

void Queue::setLlaLock(LlaLock* llaLock)
{
    this->llaLock = llaLock;
}

LlaLock* Queue::getLlaLock()
{
    return this->llaLock;
}

bool Queue::checkLlaStartSession()
{
    if (!this->llaLock) //ignore LLA if not set
    {
        return true;
    }

    return this->llaLock->startLlaSession();
}

void Queue::checkLlaStopSession()
{
    if (this->llaLock)
    {
        this->llaLock->stopLlaSession();
    }
}

bool Queue::processing()
{
    return this->isProcessing;
}
