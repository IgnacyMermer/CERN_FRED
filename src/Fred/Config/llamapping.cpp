#include "Fred/Config/llamapping.h"
#include "Parser/utility.h"

LlaMapping::LlaMapping(vector<string> data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        if (data[i].size())
        {
            vector<string> path = Utility::splitString(data[i], "=");
            if (path.size() == 1)
            {
                processLlaUnit(path[0]);
            }
            else if (path.size() == 2)
            {
                processLlaUnit(path[0], path[1]);
            }
        }
    }
}

void LlaMapping::processLlaUnit(const string &line, const string &params)
{
    vector<string> path = Utility::splitString(line, "/");
    if (path.size() == 2 || path.size() == 3)
    {
        string alfId = path[0];
        int32_t serialId = stoi(path[1].substr(path[1].find("_") + 1)); //SERIAL_x
        int32_t endpointId = path.size() == 3 ? stoi(path[2].substr(path[2].find("_") + 1)) : -1;

        if (alfId.find("ALF") == 0)
        {
            uint32_t repeat = 1;
            uint32_t delay = 0;

            if (params.size())
            {
                vector<double> llaParams = Utility::splitString2Num(params, ",");
                if (llaParams.size() == 2)
                {
                    repeat = llaParams[0] >= 1 ? uint32_t(llaParams[0]) : 1;
                    delay = llaParams[1] >= 0 ? uint32_t(llaParams[1]) : 0;
                }
            }

            processLocation(alfId, serialId, endpointId, repeat, delay);
        }
    }
}

void LlaMapping::processLocation(string alfId, int32_t serialId, int32_t endpointId, uint32_t repeat, uint32_t delay)
{
    if (!checkIfEntryExists(alfId, serialId, endpointId))
    {
        AlfEntry::SerialEntry::EndpointEntry endpointEntry;
        endpointEntry.id = endpointId;

        AlfEntry::SerialEntry serialEntry;
        serialEntry.id = serialId;
        serialEntry.endpoints[endpointId] = endpointEntry;

        AlfEntry alfEntry;
        alfEntry.id = alfId;
        alfEntry.serials[serialId] = serialEntry;
        alfEntry.version = endpointId == -1 ? AlfEntry::Version::v0 : AlfEntry::Version::v1;

        LlaEntry llaEntry;
        llaEntry.alfEntry = alfEntry;
        llaEntry.repeat = repeat;
        llaEntry.delay = delay;

        this->llaEntries.push_back(llaEntry);
    }
}

bool LlaMapping::checkIfEntryExists(const string& alfId, int32_t serialId, int32_t endpointId)
{
    for (size_t i = 0; i < this->llaEntries.size(); i++)
    {
        if (this->llaEntries[i].alfEntry.id == alfId && this->llaEntries[i].alfEntry.serials.count(serialId) && this->llaEntries[i].alfEntry.serials[serialId].endpoints.count(endpointId))
        {
            return true;
        }
    }

    return false;
}

vector<LlaMapping::LlaEntry> &LlaMapping::alfList()
{
    return llaEntries;
}
