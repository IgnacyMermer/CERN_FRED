#include "Fred/fred.h"
#include "Fred/Mapi/mapi.h"
#include "Alfred/print.h"

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

class MapiFactory
{
private:
    Fred* fred;                                             // Fred Object

    vector<Mapi*> mapiObjects;                              // Vector of Stave Function/Monitoring Mapi objects

    void generateObjects();
public:
    MapiFactory(Fred* fred);
    virtual ~MapiFactory();
};