#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "Fred/fred.h"
#include "Alfred/print.h"
#ifdef USE_MAPI
#include "mapifactory.h"
#endif
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "./DIM_WINCC_Server.h"

DimServer dimServer;


int main(int argc, char** argv){
	bool parseOnly = Fred::commandLineArguments(argc, argv);
	Fred fred(parseOnly, Fred::readConfigFile(), "./sections");
#ifdef USE_MAPI
	MapiFactory* mapiFactory;
	try
	{
		mapiFactory = new MapiFactory(&fred);
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


	int run = 10, run2=100, score=0;
    DimService runNumber("DELPHI/RUN_NUMBER",run);
	DimService runNumber2("DELPHI/RUN_NUMBER2",run2);
	DimService scoreService("CALCULATOR/SCORE",score);
    //dimServer.start("RUN_INFO");
	DimCommand runCalculatorCmd("CALCULATOR/OPERATION","C");
	DIM_WINCC_Server dimWinccServer(FT0);
	while(1){
		run++;
		runNumber.updateService();
		sleep(1);
		while(runCalculatorCmd.getNext()) {
			try{
				char *commandCalc = runCalculatorCmd.getString();
				Print::PrintInfo(commandCalc);
				std::string command(commandCalc);
				std::vector<std::string> elements;
				std::stringstream ss(command);
				std::string element;
				std::string delimeter = "_";
				while (std::getline(ss, element, '_')) {
					elements.push_back(element);
				}
				Print::PrintInfo(command);
				Print::PrintInfo(elements[1]);
				int firstNumber, secondNumber;
				std::istringstream(elements[1])>>firstNumber;
				std::istringstream(elements[2])>>secondNumber;
				if(elements[0]=="ADD"){
					score=firstNumber+secondNumber;
				}
				else if(elements[0]=="REMOVE"){
					score=firstNumber-secondNumber;
				}
				else if(elements[0]=="MULT"){
					score=firstNumber*secondNumber;
				}
				else if(elements[0]=="DIVIDE"){
					if(secondNumber==0){
						
					}
					score=firstNumber/secondNumber;
				}
				Print::PrintInfo(std::to_string(score));
				scoreService.updateService();
				
			}
			catch(...){

			}
		}
    }

	return 0;
}
