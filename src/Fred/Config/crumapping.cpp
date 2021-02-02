#include "Fred/Config/crumapping.h"
#include "Parser/utility.h"

CruMapping::CruMapping(vector<string> data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        if (data[i].find("=") != string::npos)
        {
            string left = data[i].substr(0, data[i].find("="));
            string right = data[i].substr(data[i].find("=") + 1);
            processCruUnit(left, right);
        }
    }
}

void CruMapping::processCruUnit(string& left, string& right)
{
    CruUnit unit;

    unit.cruUnitName = left;

    vector<string> path = Utility::splitString(right, "/");
    if (path.size() == 2 || path.size() == 3)
    {
        CruUnit::Alf alf;
        alf.alfId = path[0];
        alf.serialId = stoi(path[1].substr(path[1].find("_") + 1)); //SERIAL_x
        alf.endpointId = path.size() == 3 ? stoi(path[2].substr(path[2].find("_") + 1)) : -1; //ENDPOINT_x
        alf.version = alf.endpointId == -1 ? AlfEntry::Version::v0 : AlfEntry::Version::v1;

        if (alf.alfId.find("ALF") == 0)
        {
            processLocation(alf.alfId, alf.serialId, alf.endpointId);
            unit.alf = alf;
        }
    }

    cruUnits.push_back(unit);
}

void CruMapping::processLocation(string alfId, int32_t serialId, int32_t endpointId)
{
    if (!alfs.count(alfId)) //new ALF
    {
        AlfEntry::SerialEntry::EndpointEntry endpointEntry;
        endpointEntry.id = endpointId;

        AlfEntry::SerialEntry serialEntry;
        serialEntry.id = serialId;
        serialEntry.endpoints[endpointId] = endpointEntry;

        AlfEntry NewAlfEntry;
        NewAlfEntry.id = alfId;
        NewAlfEntry.serials[serialId] = serialEntry;
        NewAlfEntry.version = endpointId == -1 ? AlfEntry::Version::v0 : AlfEntry::Version::v1;

        alfs[alfId] = NewAlfEntry;
    }
    else //already existing ALF
    {
        if ((endpointId == -1 ? AlfEntry::Version::v0 : AlfEntry::Version::v1) != alfs[alfId].version)
        {
            throw new runtime_error("ALF_" + alfId + " - version mismatch");
        }

        if (!alfs[alfId].serials.count(serialId)) //new serial
        {
            AlfEntry::SerialEntry::EndpointEntry endpointEntry;
            endpointEntry.id = endpointId;

            AlfEntry::SerialEntry NewSerialEntry;
            NewSerialEntry.id = serialId;
            NewSerialEntry.endpoints[endpointId] = endpointEntry;

            alfs[alfId].serials[serialId] = NewSerialEntry;
        }
        else //already existing serial
        {
            if (!alfs[alfId].serials[serialId].endpoints.count(endpointId))
            {
                AlfEntry::SerialEntry::EndpointEntry NewEndpointEntry;
                NewEndpointEntry.id = endpointId;
                alfs[alfId].serials[serialId].endpoints[endpointId] = NewEndpointEntry;
            }
        }
    }
}

vector<CruMapping::CruUnit>& CruMapping::getCruUnits()
{
    return cruUnits;
}

map<string, Location::AlfEntry>& CruMapping::alfList()
{
    return alfs;
}
