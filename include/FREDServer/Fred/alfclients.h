#ifndef ALFCLIENTS_H
#define ALFCLIENTS_H

#include <string>
#include <vector>
#include <map>

#include "Fred/Config/location.h"
#include "Fred/Config/instructions.h"
#include "Fred/queue.h"
#include "Alfred/rpcinfo.h"

using namespace std;

class AlfRpcInfo;
class CruAlfRpcInfo;

class AlfClients
{
public:
    struct Nodes
    {
        AlfRpcInfo *sca, *swt, *ic;
        Queue *queue;
    };

    struct CruNodes
    {
        CruAlfRpcInfo *registerWrite, *registerRead, *patternPlayer;
    };

private:
    map<string, map<int32_t, map<int32_t, Nodes> > > clients;
    map<string, map<int32_t, CruNodes> > cruClients;
    Fred* fred;

public:
    AlfClients(Fred* fred);
    ~AlfClients();

    void registerAlf(Location::AlfEntry& entry);
    Nodes createAlfInfo(string id, int32_t serial, int32_t link);
    void registerCruAlf(Location::AlfEntry& entry);

    AlfRpcInfo* getAlfNode(string alf, int32_t serial, int32_t link, Instructions::Type type);
    Queue* getAlfQueue(string alf, int32_t serial, int32_t link);

    CruAlfRpcInfo* getCruAlfNode(string alf, int32_t serial, ALFRED_TYPES::CRU_TYPES type);
};

#endif // ALFCLIENTS_H
