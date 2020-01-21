#include "Fred/queue.h"
#include "Fred/alfrpcinfo.h"
#include "Fred/alfinfo.h"
#include "Alfred/print.h"

Queue::Queue(string alfId, Fred* fred)
{
    this->id = alfId;
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

        queue->conditionVar.wait(uniqueLock);;

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
            request.second->alfLink->setTransaction(request);

            PrintVerbose(request.second->name, "Parsing message");

            string fullMessage;
            
            try
            {
                if (request.second->mapi == NULL)
                {
                    fullMessage = request.first->generateFullMessage(*request.second->instruction);
                }
                else
                {
                    fullMessage = request.first->generateMapiMessage();
                }
            }
            catch (exception& e)
            {
                string errorMessage = e.what();
                PrintError(request.second->name, errorMessage);

                request.second->error->Update(errorMessage.c_str());
                PrintError(request.second->name, "Updating error service!");

                queue->isProcessing = false;
                continue;
            }

            char* buffer;

            //if (!request.second->instruction->subscribe || request.second->mapi != NULL) //if MAPI or not subscribe topic
            //{
                if (request.second->mapi != NULL && request.second->mapi->noRpcRequest) //if MAPI and noRpcRequest = true, than the RPC request is not sent
                {
                    PrintVerbose(request.second->name, "Skipping RPC request");
                    request.second->mapi->noRpcRequest = false; //reset noRpcRequest 
                    fullMessage = "not empty string";
                    buffer = strdup(fullMessage.c_str());
                }
                else
                {
                    buffer = strdup(fullMessage.c_str());
                    PrintVerbose(request.second->name, "Sending RPC request:\n" + string(buffer));
                    request.second->alfLink->Send(buffer);
                }
            //}
            //else
            //{
            //    Mapping::Unit* unit = request.second->unit;
            //    if (request.second->interval > 0.0)
            //    {
            //        RpcInfoString* rpcInfo = queue->fred->getAlfClients().getAlfNode(unit->alfId, unit->serialId, unit->linkId, request.second->instruction->type, true);
            //        fullMessage = request.second->name + "\n" + to_string(request.second->interval) + "\n" + fullMessage;
            //        buffer = strdup(fullMessage.c_str());
            //        PrintVerbose(request.second->name, "Sending RPC subscribe request:\n" + string(buffer));
            //        request.second->alfInfo->setTransaction(request);
            //        rpcInfo->Send(buffer);
            //    }
            //    else
            //    {
            //        RpcInfoString* rpcInfo = queue->fred->getAlfClients().getAlfNode(unit->alfId, unit->serialId, unit->linkId, request.second->instruction->type, false);
            //        fullMessage = request.second->name;
            //        buffer = strdup(fullMessage.c_str());
            //        PrintVerbose(request.second->name, "Sending RPC unsubscribe request:\n");
            //        request.second->alfInfo->clearTransaction();
            //        rpcInfo->Send(buffer);
            //    }
            //}

            free(buffer);

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
