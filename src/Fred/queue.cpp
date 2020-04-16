#include "Fred/queue.h"
#include "Fred/alfrpcinfo.h"
#include "Fred/alfinfo.h"
#include "Alfred/print.h"

Queue::Queue(string alfId, int32_t serial, int32_t link, Fred* fred)
{
    this->id = alfId + "/SERIAL_" + to_string(serial) + "/LINK_" + to_string(link);
    this->fred = fred;
    this->isFinished = false;
    this->isProcessing = false;
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
            bool noRpcRequest = false; //if true, the RPC request is not sent

            try
            {
                if (request.second->mapi == NULL)
                {
                    fullMessage = request.first->generateFullMessage(*request.second->instruction);
                }
                else
                {
                    fullMessage = vector<string>({ request.first->generateMapiMessage() });
                    noRpcRequest = request.second->mapi->noRpcRequest;
                }
            }
            catch (exception& e)
            {
                string errorMessage = e.what();
                Print::PrintError(request.second->name, errorMessage);

                request.second->error->Update(errorMessage.c_str());
                Print::PrintError(request.second->name, "Updating error service!");

                queue->isProcessing = false;
                continue;
            }

            if (noRpcRequest)
            {
                Print::PrintVerbose(request.second->name, "Skipping RPC request");
                noRpcRequest = false; //reset noRpcRequest
            }
            else
            {
                bool errorOccured = false;

                for (size_t i = 0; i < fullMessage.size() && !errorOccured; i++)
                {
                    char* buffer = strdup(fullMessage[i].c_str());

                    do
                    {
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
