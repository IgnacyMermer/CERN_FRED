#include "Alfred/queuelock.h"

QueueLock::QueueLock(uint32_t capacity)
{
    this->available = 0;
    this->capacity = capacity;
}

QueueLock::~QueueLock()
{
    notify();
}

void QueueLock::wait()
{
    unique_lock<mutex> guard(this->lock);
    while (!this->available)
    {
        this->condition.wait(guard);
    }
    --this->available;
}

bool QueueLock::wait_for(int milis)
{
    unique_lock<mutex> guard(this->lock);
    while (!this->available)
    {
        if (this->condition.wait_for(guard, milis*1ms) == cv_status::timeout)
        {
            break;
        }
    }

    if (this->available)
    {
        --this->available;
        return true;
    }

    return false;
}

void QueueLock::notify()
{
    lock_guard<mutex> guard(this->lock);
    if (this->available < this->capacity)
    {
        ++this->available;
    }
    this->condition.notify_one();
}

void QueueLock::clear()
{
    lock_guard<mutex> guard(this->lock);
    this->available = 0;
}
