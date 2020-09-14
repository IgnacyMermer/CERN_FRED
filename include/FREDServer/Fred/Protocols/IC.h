#ifndef IC_H
#define IC_H

#include <vector>
#include <string>
#include "Fred/Config/instructions.h"
#include "Parser/processmessage.h"

using namespace std;

class IC
{
public:
    static vector<string> generateMessage(Instructions::Instruction& instructions, vector<string>& outputPattern, vector<string>& pollPattern, ProcessMessage* processMessage);
    static void checkIntegrity(const string& request, const string& response);
    static vector<vector<unsigned long> > readbackValues(const string& message, const vector<string>& outputPattern, Instructions::Instruction& instructions);
    static uint32_t getReturnWidth();
};

#endif // IC_H
