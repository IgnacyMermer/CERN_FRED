#include "mapifactory.h"

#include "basic_mapi/mapiexample.h"
#include "indefinite_mapi/indefinitemapi.h"
#include "iterative_mapi/averagetemp.h"
#include "mapigroup/testgroup.h"

MapiFactory::MapiFactory(Fred *fred)
{
    this->fred = fred;

    try
    {
        this->generateObjects();
    }
    catch (const exception &e)
    {
        Print::PrintError(e.what());
        exit(EXIT_FAILURE);
    }
}

MapiFactory::~MapiFactory()
{
    /*for (size_t i = 0; i < this->mapiObjects.size(); i++)
    {
        //delete this->mapiObjects[i];
    }*/
}

/**
 *
 * Registering mapi objects into FRED
 *
 **/
void MapiFactory::generateObjects()
{

    // Basic mapi object
    bool fakeLink = false;
    MapiExample *mapiExample = new MapiExample();
    this->fred->registerMapiObject(fred->Name() + "/MAPI_EXAMPLE/LOOP0/TEST", mapiExample, true);
    this->mapiObjects.push_back(mapiExample);

    // Iterative mapi object
    Averagetemp *averageTemp = new Averagetemp();
    this->fred->registerMapiObject(fred->Name() + "/ITERATIVEBOARDS/ITERATIVEBOARD0/INTERNAL/AVERAGE",averageTemp, true);
    this->mapiObjects.push_back(averageTemp);

    // Mapi Group object
    Testgroup *testgroup = new Testgroup(this->fred);
    this->fred->registerMapiObject(fred->Name() + "/TESTGROUPS/TESTGROUP0/TEST",testgroup, true);
    this->mapiObjects.push_back(testgroup);

    // Indefinite mapi object
    TestIndefMapi *testIndefMapi = new TestIndefMapi();
    this->fred->registerMapiObject(fred->Name() + "/INDEF_MAPI/LOOP0/TEST", testIndefMapi, true);
    this->mapiObjects.push_back(mapiExample);
    
}
