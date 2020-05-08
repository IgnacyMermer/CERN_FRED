#include <cctype>
#include <math.h>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>
#include "Parser/processmessage.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "Fred/Config/instructions.h"
#include "Fred/Protocols/SCA.h"

string SCA::generateMessage(Instructions::Instruction& instructions, vector<string>& outputPattern, ProcessMessage* processMessage)
{
    string message;
    bool parseInVar = instructions.inVars.size() > 0;

    int32_t multiplicity = processMessage->getMultiplicity();
    size_t messageSize = instructions.message.size();

    for (int32_t m = 0; m < multiplicity; m++)
    {
        for (size_t i = 0; i < messageSize; i++)
        {
            string outVars;
            stringstream ss;
            string line = instructions.message[i];

            if (parseInVar) processMessage->parseInputVariables(line, instructions.inVars, m); //parse IN_VARs

            size_t atPos = line.find('@');

            if (atPos != string::npos)
            {
                outVars = line.substr(atPos + 1);
                line.erase(atPos); //remove @OUT_VAR
            }

            size_t comma = line.find(',');
            if (comma != string::npos)
            {
                unsigned long command, data;
                command = stol(line.substr(0, comma), 0, 16);
                data = stol(line.substr(comma + 1), 0, 16);

                if (command > 0xffffffff || data > 0xffffffff) throw runtime_error("SCA 16 bits exceeded!");

                ss << "0x" << setw(8) << setfill('0') << hex << command << "," << "0x" << setw(8) << setfill('0') << hex << data;
                line = ss.str();
            }
            else throw runtime_error("SCA comma is missing!");

            outputPattern.push_back(outVars); //push_back outvar name, empty string if not present
            message += line + "\n";
        }
    }

    message.erase(message.size() - 1);
    return message;
}

void SCA::checkIntegrity(const string& request, const string& response)
{
    for (size_t i = 0; i < response.size(); i++)
    {
        if (!(isxdigit(response[i]) || response[i] == '\n' || response[i] == ',' || response[i] == 'x'))
        {
            throw runtime_error("SCA: Invalid character received in RPC data:\n" + response + "\n");
        }
    }

    vector<string> reqVec = Utility::splitString(request, "\n");
    vector<string> resVec = Utility::splitString(response, "\n");

    if (reqVec.size() != resVec.size())
    {
        throw runtime_error("SCA: Invalid number of lines received!"); 
    }

    for (size_t i = 0; i < resVec.size(); i++)
    {
        transform(reqVec[i].begin(), reqVec[i].end(), reqVec[i].begin(), ::tolower);
        transform(resVec[i].begin(), resVec[i].end(), resVec[i].begin(), ::tolower);

        if (resVec[i].length() != 21 || (resVec[i].substr(0, resVec[i].find(",")) != reqVec[i].substr(0, reqVec[i].find(","))))
        {
            throw runtime_error("SCA: Integrity check of received message failed!");
        }
    }
}

/*
 * Return VARs values (as vector because of the multiplicity) ordered top to bottom as in the sequence 
 */
vector<vector<unsigned long> > SCA::readbackValues(const string& message, const vector<string>& outputPattern, Instructions::Instruction& instructions)
{
    vector<string>& vars = instructions.vars;
    vector<string> splitted = Utility::splitString(message, "\n");

    vector<unsigned long> values;
    for (size_t i = 0; i < splitted.size(); i++)
    {
        size_t pos = splitted[i].find(",");
        if (pos != string::npos)
        {
            values.push_back(stoul(splitted[i].substr(pos + 1), NULL, 16)); //all 32 bits payloads
        }
    }

    vector<vector<unsigned long> > results(vars.size(), vector<unsigned long>());
    for (size_t i = 0; i < values.size(); i++)
    {
        if (outputPattern[i] != "") //if there is a VAR in the request line
        {
            size_t id = distance(vars.begin(), find(vars.begin(), vars.end(), outputPattern[i])); //Order values as VARs
            results[id].push_back(values[i]);
        }
    }

    return results;
}

uint32_t SCA::getReturnWidth()
{
    return SCA_RETURN_WIDTH;
}
