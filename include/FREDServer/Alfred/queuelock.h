#ifndef LOCK_H
#define LOCK_H

#include <mutex>
#include <condition_variable>

using namespace std;

class QueueLock
{
private:
    mutex lock;
    condition_variable condition;
    uint32_t available, capacity;

public:
    QueueLock(uint32_t capacity = 0xFFFFFFFF);
    ~QueueLock();

    void wait();
    bool wait_for(int milis);
    void notify();
    void clear();
};

#endif
