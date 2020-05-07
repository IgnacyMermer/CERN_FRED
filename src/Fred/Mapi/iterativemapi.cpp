#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "Fred/fredtopics.h"
#include "Fred/fred.h"
#include "Alfred/print.h"
#include "Parser/processmessage.h"

/*
 * Send a new request to the same MAPI topic 
 */
void Iterativemapi::newRequest(string request)
{
    bool useCru = dynamic_cast<MappedCommand*>(this->thisMapi->command)->getUseCru();
    ProcessMessage* processMessage = new ProcessMessage(this, request, useCru);
    Queue* queue = useCru ? this->thisMapi->alfQueue.first : this->thisMapi->alfQueue.second;
    if (!queue)
    {
        string error = "Required ALF/CANALF not available!";
        Print::PrintError(name, error);
        thisMapi->error->Update(error);
        delete processMessage;
        return;
    }

    queue->newRequest(make_pair(processMessage, thisMapi));
}
