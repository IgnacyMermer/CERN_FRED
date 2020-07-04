#ifndef LLAMAPPING_H
#define LLAMAPPING_H

#include "Fred/Config/location.h"
#include <string>
#include <vector>

using namespace std;

class LlaMapping: public Location
{
public:
    LlaMapping(vector<string> data = vector<string>());

    map<string, AlfEntry>& alfList();

private:
    void processLlaUnit(string& line);
    void processLocation(string alfId, int32_t serialId);
};

#endif
