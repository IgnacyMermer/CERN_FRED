#include "Fred/queue.h"
#include "Fred/alfrpcinfo.h"
#include "Fred/alfinfo.h"
#include "Alfred/print.h"
#include "Fred/llalock.h"

Queue::Queue(Fred* fred, int bank)
{
    this->fred = fred;
    this->isFinished = false;
    this->isProcessing = false;
    this->llaLock = NULL;
    this->queueLock = new QueueLock;
    this->bank = bank;

    if (this->fred->queueExecutors.find(bank) == this->fred->queueExecutors.end()) 
    {
        this->fred->queueExecutors[bank] = new QueueExecutor(this->fred);

    } 

}

Queue::~Queue()
{
    this->isFinished = true;
    this->queueLock->notify();
    // if(this->fred->queueExecutors[bank] != nullptr)
    // {
    //     delete this->fred->queueExecutors[bank];
    //     this->fred->queueExecutors[bank]=nullptr;
    // }
    // queueThread->join();
    
    // delete queueThread;
    delete queueLock;
}

void Queue::clearQueue(Queue *queue)
{
    if (queue->isFinished)
    {
        return;
    }

    if(queue->stackEmpty())
    {
        return;
    }
    // while (!queue->stackEmpty())
    // {
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
            if (request.second->mapi != NULL && request.second->mapi->noRpcRequest)
            {
                Print::PrintVerbose(request.second->name, "Skipping RPC request");
                request.second->mapi->noRpcRequest = false; //reset noRpcRequest 
                queue->isProcessing = false;
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

                            if (request.second->mapi && request.second->mapi->customMessageProcess())
                            {
                                request.second->mapi->processOutputMessage("failure\nCouldn't start session");
                            }
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
        catch (exception& e)
        {
            string errorMessage = e.what();
            Print::PrintError(request.second->name, errorMessage);

            request.second->error->Update(errorMessage);
            Print::PrintError(request.second->name, "Updating error service!");

            queue->isProcessing = false;
        }

        
    // }

    queue->checkLlaStopSession(); //if no request in any queue, stop LLA session
    
}

void Queue::newRequest(pair<ProcessMessage*, ChainTopic*> request)
{
    {
        lock_guard<mutex> lockGuard(stackMutex);

        stack.push_back(request);
    }
    this->fred->queueExecutors[bank]->newRequest(this);
    queueLock->notify();
}

void Queue::wakeUp()
{
    queueLock->notify();
}

size_t Queue::getStackSize()
{
    lock_guard<mutex> lockGuard(stackMutex);
    return stack.size();
}

bool Queue::stackEmpty()
{
    lock_guard<mutex> lockGuard(stackMutex);
    return stack.empty();
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
