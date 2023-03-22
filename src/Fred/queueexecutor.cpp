#include "Fred/queueexecutor.h"
#include "Alfred/print.h"

QueueExecutor::QueueExecutor(Fred* fred)
{
    this->fred = fred;
    this->isFinished = false;
    this->queueLock = new QueueLock;
    this->queueExecutorThread = new thread(executeQueue, this);
    
}

QueueExecutor::~QueueExecutor()
{
    this->isFinished = true;
    this->queueLock->notify();
    queueExecutorThread->join();
    delete queueExecutorThread;
    // delete this;
}

void QueueExecutor::executeQueue(QueueExecutor *queueExecutor)
{
    while (1)
    {

        if (queueExecutor->isFinished)
        {
            return;
        }

        queueExecutor->queueLock->wait();
        while (!queueExecutor->stackEmpty())
        {

            Queue* request;
            {
                lock_guard<mutex> lockGuard(queueExecutor->stackMutex);
                request = queueExecutor->stack.front();
                queueExecutor->stack.pop_front();
            }
            //do processing
            Queue::clearQueue(request);
        }

        // queue->checkLlaStopSession(); //if no request in any queue, stop LLA session
    }
}

void QueueExecutor::newRequest(Queue* request)
{
    {
        lock_guard<mutex> lockGuard(stackMutex);

        stack.push_back(request);
    }
    queueLock->notify();


}

size_t QueueExecutor::getStackSize()
{
    lock_guard<mutex> lockGuard(stackMutex);
    return stack.size();
}

bool QueueExecutor::stackEmpty()
{
    lock_guard<mutex> lockGuard(stackMutex);
    return stack.empty();
}