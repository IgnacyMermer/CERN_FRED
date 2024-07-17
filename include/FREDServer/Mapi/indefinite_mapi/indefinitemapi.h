#include "Fred/Mapi/indefinitemapi.h"

class TestIndefMapi: public IndefiniteMapi
{
private:
	void processExecution();

public:
	TestIndefMapi();
	virtual ~TestIndefMapi();
};

