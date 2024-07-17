#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "mapigroup/testgroup.h"
#include "Parser/utility.h"

Testgroup::Testgroup(Fred* fred)
{
    this->fred = fred;
}

string Testgroup::processInputMessage(string input)
{
	vector< pair <string, string> > requests;

	if (input == "go")
	{
		requests.push_back(make_pair(this->fred->Name()+"/ITERATIVEBOARDS/ITERATIVEBOARD0/INTERNAL/AVERAGE", "10"));
		requests.push_back(make_pair(this->fred->Name()+"/INDEF_MAPI/LOOP0/TEST", "test"));
		
		newMapiGroupRequest(requests);
	}
	
	requests.empty();
	
	// noRpcRequest = true;

    return "reset";
}

string Testgroup::processOutputMessage(string output)
{
	return output;
}