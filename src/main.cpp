#include <stdio.h>
#include <fstream>
#include "Fred/fred.h"
#include "Alfred/print.h"
#ifdef USE_MAPI
#include "mapifactory.h"
#endif

int main(int argc, char** argv)
{
	bool parseOnly = Fred::commandLineArguments(argc, argv);
	Fred fred(parseOnly, Fred::readConfigFile(), "./sections");

#ifdef USE_MAPI

	try
	{
		MapiFactory* mapiFactory = new MapiFactory(&fred);
	}
	catch(exception& e)
	{
		exit(EXIT_FAILURE);
	}

#endif
	fred.Start();
#ifdef USE_MAPI
	delete mapiFactory;
#endif
	return 0;
}
