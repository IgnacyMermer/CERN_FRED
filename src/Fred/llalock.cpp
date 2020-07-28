#include "Fred/llalock.h"
#include <unistd.h>

LlaLock::LlaLock(const string& alf, int32_t serial, uint32_t repeat, uint32_t delay, vector<Queue*> queues, Fred* fred)
{
    this->alf = alf;
    this->serial = serial;
    this->repeat = repeat;
    this->delay = delay;
    this->queues = queues;
    this->fred = fred;
    this->hasLlaSession = false;

    this->startLla = dynamic_cast<LlaAlfRpcInfo*>(this->fred->GetRpcInfo(this->alf + "/SERIAL_" + to_string(this->serial) + "/LLA_SESSION_START"));
    this->stopLla = dynamic_cast<LlaAlfRpcInfo*>(this->fred->GetRpcInfo(this->alf + "/SERIAL_" + to_string(this->serial) + "/LLA_SESSION_STOP"));
}

bool LlaLock::startLlaSession()
{
    lock_guard<mutex> lock(this->sessionAccesss);

    if (this->hasLlaSession)
    {
        return true;
    }

    int attempt = 0;

    do
    {
        this->hasLlaSession = this->startLla->requestLlaSession();
        if (this->hasLlaSession)
        {
            break;
        }

        if (this->delay)
        {
            usleep(this->delay * 1000); //ms to us
        }
    }
    while (++attempt < this->repeat);

    return this->hasLlaSession;
}

bool LlaLock::stopLlaSession()
{
    lock_guard<mutex> lock(this->sessionAccesss);

    if (!this->hasLlaSession)
    {
        return true;
    }

    this->hasLlaSession = !this->stopLla->requestLlaSession();
    return !this->hasLlaSession;
}

bool LlaLock::checkQueuesEmpty()
{
    for (size_t i = 0; i < this->queues.size(); i++)
    {
        if (this->queues[i]->getStackSize())
        {
            return false;
        }
    }

    return true;
}
