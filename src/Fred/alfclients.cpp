#include "Fred/alfclients.h"
#include "Fred/alfrpcinfo.h"
#include "Fred/crualfrpcinfo.h"
#include "Fred/llaalfrpcinfo.h"
#include "Fred/fred.h"
#include "Fred/llalock.h"

AlfClients::AlfClients(Fred *fred)
{
    this->fred = fred;
}

AlfClients::~AlfClients()
{
    for (auto alf = clients.begin(); alf != clients.end(); alf++)
    {
        for (auto serial = alf->second.begin(); serial != alf->second.end(); serial++)
        {
            for (auto link = serial->second.begin(); link != serial->second.end(); link++)
            {
                delete link->second.queue;
            }
        }
    }

    for (auto llaAlf = llaClients.begin(); llaAlf != llaClients.end(); llaAlf++)
    {
        for (auto serial = llaAlf->second.begin(); serial != llaAlf->second.end(); serial++)
        {
            delete serial->second.llaLock;
        }
    }
}

void AlfClients::registerAlf(Location::AlfEntry &entry)
{
    if (clients.count(entry.id) == 0)
    {
        clients[entry.id] = map<int32_t, map<int32_t, Nodes> >();
    }

    for (auto serial = entry.serials.begin(); serial != entry.serials.end(); serial++)
    {
        if (clients[entry.id].count(serial->first) == 0)
        {
            clients[entry.id][serial->first] = map<int32_t, Nodes>();
        }

        for (size_t link = 0; link < serial->second.links.size(); link++)
        {
            if (clients[entry.id][serial->first].count(serial->second.links[link]) == 0)
            {
                //cout << entry.id << " " << serial->first << " " << serial->second.links[link] << "\n";
                clients[entry.id][serial->first][serial->second.links[link]] = createAlfInfo(entry.id, serial->first, serial->second.links[link]);
            }
        }
    }
}

AlfClients::Nodes AlfClients::createAlfInfo(string id, int32_t serial, int32_t link)
{
    Nodes nodes = { .sca = NULL, .swt = NULL, .ic = NULL, .queue = NULL };

    nodes.swt = new AlfRpcInfo(id + "/SERIAL_" + to_string(serial) + "/LINK_" + to_string(link) + "/SWT_SEQUENCE", " ", this->fred);
    this->fred->RegisterRpcInfo(nodes.swt);

    if (id.find("ALF") == 0)
    {
        nodes.sca = new AlfRpcInfo(id + "/SERIAL_" + to_string(serial) + "/LINK_" + to_string(link) + "/SCA_SEQUENCE", " ", this->fred);
        this->fred->RegisterRpcInfo(nodes.sca);
        nodes.ic = new AlfRpcInfo(id + "/SERIAL_" + to_string(serial) + "/LINK_" + to_string(link) + "/IC_SEQUENCE", " ", this->fred);
        this->fred->RegisterRpcInfo(nodes.ic);
    }

    nodes.queue = new Queue(id, serial, link, this->fred);

    return nodes;
}

void AlfClients::registerCruAlf(Location::AlfEntry& entry)
{
    if (cruClients.count(entry.id) == 0)
    {
        cruClients[entry.id] = map<int32_t, CruNodes>();
    }

    for (auto serial = entry.serials.begin(); serial != entry.serials.end(); serial++)
    {
        if (cruClients[entry.id].count(serial->first) == 0)
        {
            CruNodes cruNodes;
            cruNodes.registerWrite = new CruAlfRpcInfo(entry.id + "/SERIAL_" + to_string(serial->first) + "/REGISTER_WRITE", this->fred, ALFRED_TYPES::CRU_TYPES::WRITE);
            cruNodes.registerRead = new CruAlfRpcInfo(entry.id + "/SERIAL_" + to_string(serial->first) + "/REGISTER_READ", this->fred, ALFRED_TYPES::CRU_TYPES::READ);
            cruNodes.patternPlayer = new CruAlfRpcInfo(entry.id + "/SERIAL_" + to_string(serial->first) + "/PATTERN_PLAYER", this->fred, ALFRED_TYPES::CRU_TYPES::PATTERN_PLAYER);

            this->fred->RegisterRpcInfo(cruNodes.registerWrite);
            this->fred->RegisterRpcInfo(cruNodes.registerRead);
            this->fred->RegisterRpcInfo(cruNodes.patternPlayer);

            cruClients[entry.id][serial->first] = cruNodes;
        }
    }
}

void AlfClients::registerLlaAlf(Location::AlfEntry& entry)
{
    if (llaClients.count(entry.id) == 0)
    {
        llaClients[entry.id] = map<int32_t, LlaNodes>();
    }

    for (auto serial = entry.serials.begin(); serial != entry.serials.end(); serial++)
    {
        if (llaClients[entry.id].count(serial->first) == 0)
        {
            vector<Queue*> queues = this->getAlfCruQueues(entry.id, serial->first);
            if (queues.size())
            {
                LlaNodes llaNodes;
                llaNodes.llaStart = new LlaAlfRpcInfo(entry.id + "/SERIAL_" + to_string(serial->first) + "/LLA_SESSION_START", this->fred, ALFRED_TYPES::LLA_TYPES::START);
                llaNodes.llaStop = new LlaAlfRpcInfo(entry.id + "/SERIAL_" + to_string(serial->first) + "/LLA_SESSION_STOP", this->fred, ALFRED_TYPES::LLA_TYPES::STOP);

                this->fred->RegisterRpcInfo(llaNodes.llaStart);
                this->fred->RegisterRpcInfo(llaNodes.llaStop);

                llaNodes.llaLock = new LlaLock(entry.id, serial->first, queues, this->fred);

                for (size_t i = 0; i < queues.size(); i++)
                {
                    queues[i]->setLlaLock(llaNodes.llaLock);
                }

                llaClients[entry.id][serial->first] = llaNodes;
            }
        }
    }
}

AlfRpcInfo* AlfClients::getAlfNode(string alf, int32_t serial, int32_t link, Instructions::Type type)
{
    Nodes& nodes = clients[alf][serial][link];

    switch (type)
    {
        case Instructions::Type::SCA:
            return nodes.sca;
        case Instructions::Type::SWT:
            return nodes.swt;
        case Instructions::Type::IC:
            return nodes.ic;
    }

    return NULL;
}

Queue* AlfClients::getAlfQueue(string alf, int32_t serial, int32_t link)
{
    return clients[alf][serial][link].queue;
}

vector<Queue*> AlfClients::getAlfCruQueues(string alf, int32_t serial)
{
    vector<Queue*> queues;

    if (clients.count(alf))
    {
        if (clients[alf].count(serial))
        {
            for (auto link = clients[alf][serial].begin(); link != clients[alf][serial].end(); link++)
            {
                queues.push_back(link->second.queue);
            }
        }
    }

    return queues;
}

CruAlfRpcInfo* AlfClients::getCruAlfNode(string alf, int32_t serial, ALFRED_TYPES::CRU_TYPES type)
{
    CruNodes& nodes = cruClients[alf][serial];

    switch (type)
    {
    case ALFRED_TYPES::CRU_TYPES::WRITE:
        return nodes.registerWrite;
    case ALFRED_TYPES::CRU_TYPES::READ:
        return nodes.registerRead;
    case ALFRED_TYPES::CRU_TYPES::PATTERN_PLAYER:
        return nodes.patternPlayer;
    }

    return NULL;
}

vector<CruAlfRpcInfo*> AlfClients::getAllCruRpcs()
{
    vector<CruAlfRpcInfo*> rpcInfos;

    for (auto alf = this->cruClients.begin(); alf != this->cruClients.end(); alf++)
    {
        for (auto serial = alf->second.begin(); serial != alf->second.end(); serial++)
        {
            rpcInfos.push_back(serial->second.registerWrite);
            rpcInfos.push_back(serial->second.registerRead);
            rpcInfos.push_back(serial->second.patternPlayer);
        }
    }

    return rpcInfos;
}

vector<LlaAlfRpcInfo*> AlfClients::getAllLlaRpcs()
{
    vector<LlaAlfRpcInfo*> rpcInfos;

    for (auto alf = this->llaClients.begin(); alf != this->llaClients.end(); alf++)
    {
        for (auto serial = alf->second.begin(); serial != alf->second.end(); serial++)
        {
            rpcInfos.push_back(serial->second.llaStart);
            rpcInfos.push_back(serial->second.llaStop);
        }
    }

    return rpcInfos;
}
