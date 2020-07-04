#include "Fred/Config/llamapping.h"
#include "Parser/utility.h"

LlaMapping::LlaMapping(vector<string> data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        if (data[i].size())
        {
            processLlaUnit(data[i]);
        }
    }
}

void LlaMapping::processLlaUnit(string& line)
{
    vector<string> path = Utility::splitString(line, "/");
    if (path.size() == 2)
    {
        string alfId = path[0];
        int32_t serialId = stoi(path[1].substr(path[1].find("_") + 1)); //SERIAL_x

        if (alfId.find("ALF") == 0)
        {
            processLocation(alfId, serialId);
        }
    }
}

void LlaMapping::processLocation(string alfId, int32_t serialId)
{
    if (!alfs.count(alfId)) //new ALF
    {
        AlfEntry::SerialEntry serialEntry;
        serialEntry.id = serialId;

        AlfEntry NewAlfEntry;
        NewAlfEntry.id = alfId;
        NewAlfEntry.serials[serialId] = serialEntry;

        alfs[alfId] = NewAlfEntry;
    }
    else //already existing ALF
    {
        if (!alfs[alfId].serials.count(serialId)) //new serial
        {
            AlfEntry::SerialEntry NewSerialEntry;
            NewSerialEntry.id = serialId;

            alfs[alfId].serials[serialId] = NewSerialEntry;
        }
    }
}

map<string, Location::AlfEntry>& LlaMapping::alfList()
{
    return alfs;
}
