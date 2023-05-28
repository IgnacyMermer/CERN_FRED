#ifndef SWT_H
#define SWT_H

#include <vector>
#include <string>
#include "Fred/Config/instructions.h"
#include "Parser/processmessage.h"

using namespace std;

const int SWT_LOW_WIDTH = 32 / 4; //SWT low hexadecimal width
const int SWT_MIDDLE_WIDTH = 64 / 4; //SWT middle hexadecimal width
const int SWT_HIGH_WIDTH = 76 / 4; //SWT low hexadecimal width
const int SWT_RETURN_WIDTH = 4; //Width of the SWT OUT_VARs
const int SWT_RETURN_WIDTH_HIGH = 19;
class SWT
{
private:
    static const string swtResetWord_v0;
    static const string swtResetWord_v1;

public:
    static vector<string> generateMessage(Instructions::Instruction& instructions, vector<string>& outputPattern, vector<string>& pollPattern, ProcessMessage* processMessage);
    static void checkIntegrity(const string& request, const string& response);
    template <typename T>
    static vector<vector<T> > readbackValues(const string& message, const vector<string>& outputPattern, Instructions::Instruction& instructions);
    static uint32_t getReturnWidth(bool highWord=false);
    static void SWTpad(string& line);
};

#endif // SWT_H
