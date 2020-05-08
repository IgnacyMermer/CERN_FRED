#ifndef IC_H
#define IC_H

#include <vector>
#include <string>
#include "Fred/Config/instructions.h"

using namespace std;

class IC
{
public:
    static string generateMessage(Instructions::Instruction& instructions, vector<string>& outputPattern, ProcessMessage* processMessage);
    static void checkIntegrity(const string& request, const string& response);
    static vector<vector<unsigned long> > readbackValues(const string& message, const vector<string>& outputPattern, Instructions::Instruction& instructions);
    static uint32_t getReturnWidth();
};

#endif // IC_H
