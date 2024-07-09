#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <dis.h>
#include <fstream>
#include "Fred/fred.h"
#include "Alfred/print.h"

#ifdef USE_MAPI
#include "mapifactory.h"
#endif

#include "../dim/dim/dic.hxx"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

DimServer DIMserver;

char str[10][80];
char str1[10][80];

typedef struct {
	int i;
	int j;
	int k;
	double d;
	short s;
	char c;
	short t;
	float f;
	char str[20];
}TT;

TT t;


int big_buff[1024];

int testDimInt = 0;
int testDimLong = 0;
longlong testDimXLong = 0;

void client_exited(int *tag)
{
	char name[84];

	if(dis_get_client(name))
		printf("Client %s (%d) exited\n", name, *tag);
	else
		printf("Client %d exited\n", *tag);
}

void exit_cmnd(int *code)
{
	printf("Exit_cmnd %d\n", *code);
	exit(*code);
}

int NewData;
int NewIds[11];

int more_ids[1024];
int curr_more_index = 0;
char more_str[1024][80];

int main(int argc, char** argv){
	/*bool parseOnly = Fred::commandLineArguments(argc, argv);
	Fred fred(parseOnly, Fred::readConfigFile(), "./sections");
	Print::PrintInfo("test");
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

	try{

		int i, id, xlong_id, int_id;
		char aux[160];
		char name[84];
		char srvname[64];
		dim_long dnsid = 0;
		int index = 0;
		dim_print_date_time();
		printf("Dim Server Starting up...\n");

		int run =24 ;
		DimService runNumber("DELPHI/RUN_NUMBER",run);
		DimServer::start("DelphiServer");
		//DimInfo runNumber2("DELPHI/RUN_NUMBER",5,-1);

		Print::PrintInfo("DelphiServer started and run service with updating number");

		for(int i=0; i<10; i++){
			run+=9;
			runNumber.updateService();
			Print::PrintInfo("Run Number ");
			//Print::PrintInfo(std::to_string(runNumber2.getInt()));
			sleep(5);
		}
		
		
		//}


	}
	catch(...){
		Print::PrintInfo("error");
	}
	


	fred.Start();

#ifdef USE_MAPI
	delete mapiFactory;
#endif*/
	/*int run = 123;
	DimService runNumber("DELPHI/RUN_NUMBER",run);
	DimServer::start("DelphiServer");
	run++;
	runNumber.updateService();*/

	int run = 0, run2=100, score=0;
    DimService runNumber("DELPHI/RUN_NUMBER",run);
	DimService runNumber2("DELPHI/RUN_NUMBER2",run2);
	DimService scoreService("CALCULATOR/SCORE",score);
	//DIMserver.setDnsNode("localhost");
    DIMserver.start("RUN_INFO");
	DIMserver.start("DelphiServer");
	std::cout<<'\n'<<DIMserver.getDnsNode()<<DIMserver.getDnsPort()<<'\n';
	DimCommand runCmnd("DELPHI/TEST/CMND","C");
	DimCommand runCmnd2("PVSS_SIMPLE_COMMAND","C");
	DimCommand runCmnd3("CALCULATOR/OPERATION","C");
	DimCommand runCmnd4("PVSS_RPC","C");
	//bool parseOnly = Fred::commandLineArguments(argc, argv);
	//Fred fred(parseOnly, Fred::readConfigFile(), "./sections");
	//fred.Start();
    while(1){
        run++;
		//run2+=2;
        runNumber.updateService();
		//runNumber2.updateService();
		while(runCmnd.getNext()) {
			char *cmnd = runCmnd.getString();
			Print::PrintInfo(std::string(cmnd)+"\n");
		}
		while(runCmnd2.getNext()) {
			//char *cmnd2 = runCmnd2.getString();
			//Print::PrintInfo(std::string(cmnd2)+"\n");
		}
		while(runCmnd3.getNext()) {
			char *cmnd3 = runCmnd3.getString();
			std::string command(cmnd3);
			std::vector<std::string> elements;
			std::stringstream ss(command);
			std::string element;
			std::string delimeter = "_";
			while (std::getline(ss, element, '_')) {
				elements.push_back(element);
			}
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

			//Print::PrintInfo(std::string(cmnd3)+"\n");
		}
		sleep(5);
		

    }

	return 0;
}
