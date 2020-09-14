#ifndef QUEUE_H
#define QUEUE_H

#include <thread>
#include <list>
#include <utility>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include "Fred/fredtopics.h"
#include "Parser/processmessage.h"

using namespace std;

class Fred;
class LlaLock;

class Queue
{
public:
    Queue(string alfId, int32_t serial, int32_t link, Fred* fred);
    ~Queue();

    void newRequest(pair<ProcessMessage *, ChainTopic *> request);

    size_t getStackSize();
    void setLlaLock(LlaLock* llaLock);
    LlaLock* getLlaLock();
    bool processing();

private:
    string id;
    Fred* fred;
    condition_variable conditionVar;
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
