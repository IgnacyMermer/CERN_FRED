/*
 * Uncomment commented lines and recompile to use MAPI example
 */

#include <stdio.h>
#include <fstream>
#include "Fred/fred.h"
#include "Alfred/print.h"
//#include "Mapi/mapiexample.h"

int main(int argc, char** argv)
{
	bool parseOnly = Fred::commandLineArguments(argc, argv);
	Fred fred(parseOnly, Fred::readConfigFile(), "./sections",300);

    //MapiExample mapiExample;
	
	try
	{
        //fred.registerMapiObject("FRED1/MAPI_EXAMPLE/LOOP0/TEST", &mapiExample);
	}
	catch (exception& e)
	{
		exit(EXIT_FAILURE);
	}

	fred.Start();
	return 0;
}
