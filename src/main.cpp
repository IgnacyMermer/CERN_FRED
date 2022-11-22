/*
 * Uncomment commented lines and recompile to use MAPI example
 */

#include <stdio.h>
#include <fstream>
#include "Fred/fred.h"
#include "Alfred/print.h"
#ifdef USE_MAPI
#include "mapifactory.h"
#endif
//#include "Mapi/mapiexample.h"

int main(int argc, char** argv)
{
	bool parseOnly = Fred::commandLineArguments(argc, argv);
	Fred fred(parseOnly, Fred::readConfigFile(), "./sections");

    //MapiExample mapiExample;
	
	// try
	// {
    //     //fred.registerMapiObject("FRED1/MAPI_EXAMPLE/LOOP0/TEST", &mapiExample);
	// }
	// catch (exception& e)
	// {
	// 	exit(EXIT_FAILURE);
	// }

#ifdef USE_MAPI

	try
	{
		MapiFactory mapiFactory(&fred);
	}
	catch(exception& e)
	{
		exit(EXIT_FAILURE);
	}

#endif
	fred.Start();
	return 0;
}
