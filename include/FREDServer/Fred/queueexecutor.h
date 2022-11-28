#ifndef QUEUEEXECUTOR_H
#define QUEUEEXECUTOR_H

#include <thread>
#include <list>
#include <utility>
#include <mutex>
#include <atomic>
#include "Fred/fredtopics.h"
#include "Fred/queue.h"

using namespace std;

class Fred;

class QueueExecutor
{
public:
    QueueExecutor(Fred* fred);
    ~QueueExecutor();

    void newRequest(Queue* request);

    size_t getStackSize();
    bool stackEmpty();

private:
    Fred* fred;
    QueueLock* queueLock;
    thread* queueExecutorThread;
    atomic<bool> isFinished, isProcessing;
    list<Queue*> stack;
    mutex stackMutex;

    static void executeQueue(QueueExecutor* queueExecutor);
    bool checkLlaStartSession();
    void checkLlaStopSession();
};

#endif // QUEUE_H
