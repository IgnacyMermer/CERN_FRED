#include "Fred/cruregistercommand.h"
#include "Alfred/print.h"
#include "Fred/fred.h"
#include "Parser/utility.h"
#include "Fred/crualfrpcinfo.h"
#include <sstream>
#include <iomanip>
#include <exception>

CruRegisterCommand::CruRegisterCommand(ALFRED_TYPES::CRU_TYPES type, string topicName, CruAlfRpcInfo* rpcInfo, Fred* fred): CommandString::CommandString(fred->Name() + CruRegisterCommand::getTypeReqName(type, topicName, "REQ"), (ALFRED*)fred)
{
    this->type = type;
    this->rpcInfo = rpcInfo;
    this->name = fred->Name() + CruRegisterCommand::getTypeReqName(type, topicName, "REQ");
    this->responseService = new ServiceString(fred->Name() + CruRegisterCommand::getTypeReqName(type, topicName, "ANS"), (ALFRED*)fred);
    fred->RegisterService(this->responseService);

    this->isFinished = false;
    this->isProcessing = false;
    this->clearThread = new thread(clearRequests, this);
}

CruRegisterCommand::~CruRegisterCommand()
{
    this->isFinished = true;
    conditionVar.notify_one();
    clearThread->join();
    delete clearThread;
}

const void* CruRegisterCommand::Execution(void *value)
{
    if (!value)
    {
        Print::PrintError(name, "Invalid request, no value received!");
    }

    string request(static_cast<char*>(value));

    Print::PrintVerbose(name, "Received command:\n" + request);

    vector<string> splitted = Utility::splitString(request, ",");

    // not supported
    /*if (this->type == ALFRED_TYPES::CRU_TYPES::WRITE)
    {
        if (splitted.size() != 2)
        {
            Print::PrintError(name, "Invalid number of arguments received for CRU_REGISTER WRITE");
            return NULL;
        }

        executeWrite(splitted);
    }
    else if (this->type == ALFRED_TYPES::CRU_TYPES::READ)
    {
        if (splitted.size() != 1)
        {
            Print::PrintError(name, "Invalid number of arguments received for CRU_REGISTER READ");
            return NULL;
        }

        executeRead(splitted);
    }
    else */if (this->type == ALFRED_TYPES::CRU_TYPES::PATTERN_PLAYER)
    {
        // validate segment count while ignoring comments
        int segmentsCount = 0;
        for (auto & segment : splitted)
        {
            if (segment.find('#') == string::npos)
            {
                segmentsCount++;
            }
        }
        if (segmentsCount != 15)
        {
            Print::PrintError(name, "Invalid number of arguments received for PATTERN PLAYER");
            return NULL;
        }

        executePatternPlayer(splitted);
    }

    return NULL;
}

// not supported
/*void CruRegisterCommand::executeWrite(vector<string>& message) // 0 - ADDR, 1 - VALUE
{
    stringstream request;

    try
    {
        uint32_t address = Utility::stringToNumber(message[0]);
        uint32_t value = Utility::stringToNumber(message[1]);

        request << "0x" << hex << address << "," << "0x" << hex << value;
    }
    catch (invalid_argument& e)
    {
        Print::PrintError(name, "Invalid request received!");
        return;
    }

    newRequest(make_pair(request.str(), rpcInfo));
}

void CruRegisterCommand::executeRead(vector<string>& message) // 0 - ADDR
{
    stringstream request;

    try
    {
        uint32_t address = Utility::stringToNumber(message[0]);

        request << "0x" << hex << address;
    }
    catch (invalid_argument& e)
    {
        Print::PrintError(name, "Invalid request received!");
        return;
    }

    newRequest(make_pair(request.str(), rpcInfo));
}*/

#define SKIP_COMMENT while (message[position].find('#') != string::npos) { position++; }

void CruRegisterCommand::executePatternPlayer(vector<string>& message) // pat0, pat1, pat2, pat3, pat1, pat1, pat2, pat3, pat1, pat2, pat3, pat2, execute_pat1, execute_pat1, execute_pat2
{
    stringstream request;

    try
    {
        int position = 0;

        // patterns
        SKIP_COMMENT
        uint128_t pat0 = Utility::stringToLargeNumber(message[position++]);
        SKIP_COMMENT
        uint128_t pat1 = Utility::stringToLargeNumber(message[position++]);
        SKIP_COMMENT
        uint128_t pat2 = Utility::stringToLargeNumber(message[position++]);
        SKIP_COMMENT
        uint128_t pat3 = Utility::stringToLargeNumber(message[position++]);

        // length, delay, length, length
        SKIP_COMMENT
        uint32_t pat1Length = Utility::stringToNumber(message[position++]);
        SKIP_COMMENT
        uint32_t pat1Delay = Utility::stringToNumber(message[position++]);
        SKIP_COMMENT
        uint32_t pat2Length = Utility::stringToNumber(message[position++]);
        SKIP_COMMENT
        uint32_t pat3Length = Utility::stringToNumber(message[position++]);

        // triggers
        SKIP_COMMENT
        uint32_t pat1Trigger = Utility::stringToNumber(message[position++]);
        SKIP_COMMENT
        uint32_t pat2Trigger = Utility::stringToNumber(message[position++]);
        SKIP_COMMENT
        uint32_t pat3Trigger = Utility::stringToNumber(message[position++]);

        // pat2 TF ORBIT BC
        SKIP_COMMENT
        uint32_t pat2TfOrbitBc = Utility::stringToNumber(message[position++]);

        // execution
        SKIP_COMMENT
        bool pat1ExecuteAtStart = message[position++] == "true";
        SKIP_COMMENT
        bool pat1ExecuteNow = message[position++] == "true";
        SKIP_COMMENT
        bool pat2ExecuteNow = message[position++] == "true";

        request
                << Utility::largeNumberToHexString(pat0) << "\n"
                << Utility::largeNumberToHexString(pat1) << "\n"
                << Utility::largeNumberToHexString(pat2) << "\n"
                << Utility::largeNumberToHexString(pat3) << "\n"
                << to_string(pat1Length) << "\n"
                << to_string(pat1Delay) << "\n"
                << to_string(pat2Length) << "\n"
                << to_string(pat3Length) << "\n"
                << to_string(pat1Trigger) << "\n"
                << to_string(pat2Trigger) << "\n"
                << to_string(pat3Trigger) << "\n"
                << to_string(pat2TfOrbitBc) << "\n"
                << (pat1ExecuteAtStart ? "true" : "false") << "\n"
                << (pat1ExecuteNow ? "true" : "false") << "\n"
                << (pat2ExecuteNow ? "true" : "false");
    }
    catch (invalid_argument& e)
    {
        Print::PrintError(name, "Invalid request received!");
        return;
    }

    newRequest(make_pair(request.str(), rpcInfo));
}

void CruRegisterCommand::clearRequests(CruRegisterCommand *self)
{
    mutex lock;
    unique_lock<mutex> uniqueLock(lock);

    while (1)
    {
        if (self->isFinished)
        {
            return;
        }

        self->conditionVar.wait(uniqueLock);

        while (!self->stack.empty())
        {
            self->isProcessing = true;

            pair<string, RpcInfoString*> request;
            {
                lock_guard<mutex> lockGuard(self->stackMutex);
                request = self->stack.front();
                self->stack.pop_front();
            }
            //do processing

            Print::PrintVerbose(self->name, "Sending request to " + request.second->Name() + "\n" + request.first);
            char* buffer = strdup(request.first.c_str());
            dynamic_cast<CruAlfRpcInfo*>(request.second)->setResponseService(self->responseService);
            request.second->Send(buffer);
            free(buffer);

            self->isProcessing = false;
        }
    }
}

void CruRegisterCommand::newRequest(pair<string, RpcInfoString*> request)
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

string CruRegisterCommand::getTypeReqName(ALFRED_TYPES::CRU_TYPES type, string topicName, const string& suffix)
{
    switch (type)
    {
    //case ALFRED_TYPES::CRU_TYPES::READ:
    //    return "/" + topicName + "/REGISTER_READ_" + suffix;
    //case ALFRED_TYPES::CRU_TYPES::WRITE:
    //    return "/" + topicName + "/REGISTER_WRITE_" + suffix;
    case ALFRED_TYPES::CRU_TYPES::PATTERN_PLAYER:
        return "/" + topicName + "/PATTERN_PLAYER_" + suffix;
    }
}
