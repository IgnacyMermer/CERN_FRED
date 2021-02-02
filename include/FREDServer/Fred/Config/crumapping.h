#ifndef CRUMAPPING_H
#define CRUMAPPING_H

#include "Fred/Config/location.h"
#include <string>
#include <vector>

using namespace std;

class CruMapping: public Location
{
public:

    struct CruUnit
    {
        struct Alf
        {
            string alfId;
            int32_t serialId, endpointId;
            AlfEntry::Version version;
        };

        string cruUnitName;
        Alf alf;
    };

    CruMapping(vector<string> data = vector<string>());
    vector<CruUnit>& getCruUnits();

    map<string, AlfEntry>& alfList();

private:
    vector<CruUnit> cruUnits;

    void processCruUnit(string& left, string& right);
    void processLocation(string alfId, int32_t serialId, int32_t endpointId);
};

#endif // CRUMAPPING_H
