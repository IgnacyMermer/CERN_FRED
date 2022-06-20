#ifndef QUEUE_H
#define QUEUE_H

#include <thread>
#include <list>
#include <utility>
#include <mutex>
#include <atomic>
#include "Fred/fredtopics.h"
#include "Parser/processmessage.h"
#include "Parser/utility.h"
#include "Alfred/queuelock.h"

using namespace std;

class Fred;
class LlaLock;

class Queue
{
public:
    Queue(Fred* fred);
    ~Queue();

    void newRequest(pair<ProcessMessage *, ChainTopic *> request);

    size_t getStackSize();
    bool stackEmpty();
    void setLlaLock(LlaLock* llaLock);
    LlaLock* getLlaLock();
    bool processing();
    void wakeUp();

private:
    Fred* fred;
    QueueLock* queueLock;
    thread* queueThread;
    atomic<bool> isFinished, isProcessing;
    list<pair<ProcessMessage *, ChainTopic*> > stack;
    mutex stackMutex;
    LlaLock* llaLock;

    static void clearQueue(Queue* queue);
    bool checkLlaStartSession();
    void checkLlaStopSession();
};

#endif // QUEUE_H
