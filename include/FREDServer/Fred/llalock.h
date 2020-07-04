#ifndef LLALOCK_H
#define LLALOCK_H

#include <string>
#include <vector>
#include "Fred/queue.h"
#include "Fred/fred.h"
#include "Fred/llaalfrpcinfo.h"
#include <atomic>
#include <mutex>

using namespace std;

class LlaLock
{
public:
    LlaLock(const string& alf, int32_t serial, vector<Queue*> queues, Fred* fred);
    bool startLlaSession();
    bool stopLlaSession();
    bool checkQueuesEmpty();

private:
    string alf;
    int32_t serial;
    vector<Queue*> queues;
    Fred* fred;

    LlaAlfRpcInfo *startLla, *stopLla;
    atomic<bool> hasLlaSession;

    mutex sessionAccesss;
};

#endif
