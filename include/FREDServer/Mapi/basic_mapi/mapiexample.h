#ifndef MAPIEXAMPLE_H
#define MAPIEXAMPLE_H

#include "Fred/Mapi/mapi.h"

class MapiExample: public Mapi
{
public:
    MapiExample();

    string processInputMessage(string input);
    string processOutputMessage(string output);
};

#endif // MAPIEXAMPLE_H
