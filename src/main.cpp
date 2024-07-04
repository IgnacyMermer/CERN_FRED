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

/*
int atlas_ids[210];
float atlas_arr[10];
*/

int main(int argc, char** argv)
{
	bool parseOnly = Fred::commandLineArguments(argc, argv);
	Fred fred(parseOnly, Fred::readConfigFile(), "./sections");
	Print::PrintInfo("test");
	//TypeTCM TCM;
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


		//DimServer DIMserver;
		


        /*connect(this, &FITelectronics::resetFinished, this, [=]() {
            if (TCM.services.isEmpty()) createDIMservices();
            readCountersDirectly(false);
            apply_COUNTERS_UPD_RATE(TCM.set.COUNTERS_UPD_RATE);
        });

        qRegisterMetaType<DimCommand *>("DIMcommandPointer");
        connect(this, &FITelectronics::DIMcommandReceived, this, &FITelectronics::executeDIMcommand);
        serverStatus.service = new DimService("/SERVER_STATUS", serverStatus.string);
        allCommands.insert(new DimCommand(q"/STOP_SERVER", "C:1", this), [=](void * ) { qApp->exit(); });
	*/

			
			
			//DIMserver.setDnsNode("localhost");
			
			
			
			//DIMserver.setDnsNode("server.dyndns.cern.ch");
			//DIMserver.start("_DIM_SERVER");
			//dis_set_debug_on();
			
			
			//DimServer::start();
			//Print::PrintInfo("FT0_DIM started");

			int i, id/*, big_ids[20]*/, xlong_id, int_id;
		char aux[160];
		char name[84]/*, name1[132]*/;
		char srvname[64];
	/*
		int on = 0;
	*/
		dim_long dnsid = 0;
	//	dim_long dnsid1 = 0;
	//	char extra_dns[128];
	//	int new_dns = 0;
		int index = 0;
	/*
		dim_set_write_timeout(1);
	*/
	/*
		int buf_sz, buf_sz1;
	*/
	/*
	dis_set_debug_on();
	*/
	/*
		int status;
		regex_t re;

		if(regcomp(&re, "abc*",REG_EXTENDED|REG_NOSUB) != 0)
			printf("regcomp error\n");
		status = regexec(&re,"abcdef", (size_t)0, NULL, 0);
		regfree(&re);
		printf("result = %d\n", status); 
	*/
		dim_print_date_time();
		printf("Dim Server Starting up...\n");
		
			//fflush(stdout);
	//if(argc){}


////	new_dns = dim_get_env_var("EXTRA_DNS_NODE", extra_dns, sizeof(extra_dns));
////	if(new_dns)
////			dnsid = dis_add_dns(extra_dns, 0);
//	dnsid = dis_add_dns("pclhcb161",2506);
//	printf("Adding DNS %lx\n", dnsid);
//	dnsid1 = dis_add_dns("pclhcb161", 2507);
//	printf("Adding DNS %lx\n", dnsid1);
//	if(dnsid){}
/*
	buf_sz = dim_get_write_buffer_size();
	dim_set_write_buffer_size(10000000);
	buf_sz1 = dim_get_write_buffer_size();
printf("socket buffer size = %d, after = %d\n",buf_sz, buf_sz1);
*/


	//dis_add_exit_handler(exit_cmnd);
	//dis_add_client_exit_handler(client_exited);

	//strcpy(srvname, argv[1]);
//	if(!strcmp(srvname,"xx1"))
//		strcpy(srvname,"xx");
	
	
	/*for(i = 0; i< 10; i++)
	{
 	        snprintf(str[i],sizeof(str[i]),"%s/Service_%03d",srvname,i);
		dis_add_service( str[i], "C", str[i], (int)strlen(str[i])+1, 
			(void *)0, 0 );
		snprintf(str1[i], sizeof(str1[i]), "%s-1/Service_%03d", srvname, i);
		dis_add_service_dns(dnsid, str1[i], "C", str1[i], (int)strlen(str1[i]) + 1,
			(void *)0, 0);
	}
	t.i = 123;
	t.j = 456;
	t.k = 789;
	t.d = 56.78;
	t.s = 12;
	t.t = 12;
	t.c = 'a';
	t.f = (float)4.56;
	strcpy(t.str,"hello world");

	snprintf(aux, sizeof(aux),"%s/TEST_SWAP",srvname);
	id = dis_add_service( aux, "l:3;d:1;s:1;c:1;s:1;f:1;c:20", &t, sizeof(t), 
		(void *)0, 0 );


	*/


//	if(!strcmp(argv[1],"xx1"))
//	{


//		snprintf(aux, sizeof(aux),"%s/TEST_SWAP1",srvname);
//		id = dis_add_service( aux, "l:3;d:1;s:1;c:1;s:1;f:1;c:20", &t, sizeof(t), 
//			(void *)0, 0 );


//	}


/*
	if(id){}
	snprintf(aux, sizeof(aux),"%s/TEST_CMD",srvname);
	dis_add_cmnd(aux,"l:3;d:1;s:1;c:1;s:1;f:1;c:20",cmnd_rout, 0);

	snprintf(aux, sizeof(aux), "%s/Service_DimInt", srvname);
	int_id = dis_add_service(aux, "I", &testDimInt, sizeof(testDimInt),
		(void *)0, 0);
	snprintf(aux, sizeof(aux), "%s/Service_DimLong", srvname);
	dis_add_service(aux, "L", &testDimLong, sizeof(testDimLong),
		(void *)0, 0);
	snprintf(aux, sizeof(aux), "%s/Service_DimXLong", srvname);
	xlong_id = dis_add_service(aux, "X", &testDimXLong, sizeof(testDimXLong),
		(void *)0, 0);


*/


/*
	printf("Dim X size long %zd\n", sizeof(long));
	printf("Dim X size pointer %zd\n", sizeof(void *));
	printf("Dim X size dim_long %zd\n", sizeof(dim_long));
	printf("Dim X size longlong %zd\n", sizeof(longlong));
	printf("Dim X size long long %zd\n", sizeof(long long));
*/


//	snprintf(aux, sizeof(aux), "%s/TEST_XCMD", srvname);
//	dis_add_cmnd(aux, "X", cmnd_rout_long, 0);



	/*

		big_buff[0] = 1;
		for(i = 0; i < 20; i++)
		{
			snprintf(aux, sizeof(aux),"%s/TestMem_%d",argv[1], i);
			big_ids[i] = dis_add_service( aux, "I", big_buff, 1024*sizeof(int), 
				(void *)0, 0 );
		}
	*/

	/*
		for(i = 1; i <= 200; i++)
		{
			snprintf(aux, sizeof(aux),"%s/ATLAS_Service%d",argv[1],i);
			atlas_ids[i] = dis_add_service( aux, "F", atlas_arr, 10*sizeof(float), 
				(void *)0, 0 );
		}
	*/
		printf("Call Start Serving %s\n", argv[1]);
		dis_start_serving(argv[1]);
	//	printf("Call Start Serving %s %xl\n", argv[1], dnsid);
	//	dis_start_serving_dns(dnsid, argv[1]);
	//	printf("Call Start Serving %s %xl\n", argv[1], dnsid1);
	//	dis_start_serving_dns(dnsid1, argv[1]);

		
		//if(dis_get_client(name))
		//{
		//	printf("client %s\n",name);
		//}



	/*
		for(i = 0; i < 5; i++)
		{
			sleep(10);

		}
		dis_stop_serving();
		sleep(59);
	*/
		
		
		/*while(1)
		{
			index++;
			testDimXLong++;
			dis_update_service(xlong_id);
			testDimInt = -index;
			dis_update_service(int_id);
*/


	/*
			for(i = 0; i < 20; i++)
			{
				index++;
				big_buff[0] = index;
				dis_update_service(big_ids[i]);
			}
			sleep(1);
	*/
	/*
			pause();
			*/
			sleep(1);
		//}


	}
	catch(...){
		Print::PrintInfo("error");
	}
	

	fred.Start();

#ifdef USE_MAPI
	delete mapiFactory;
#endif

	return 0;
}

/*void log(QString st) { logStream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") + st << Qt::endl; logStream.flush(); }

void addCommand(QList<DimCommand *> &list, QString name, const char* format, std::function<void(void *)> function) {
	DimCommand *command = new DimCommand(qPrintable(name), format, this);
	list.append(command);
	allCommands.insert(command, function);
}
void addArrayCommand(QString parameter) { //channels parameters only
	if (!PMparameters.contains(parameter)) {
		emit error("No such PM parameter: " + parameter, logicError);
		return;
	}
	const Parameter par = PMparameters[parameter];
	if (par.interval == 0) emit error("Not an array parameter!", logicError);
	addCommand(commands, QString(FIT[subdetector].name) + "/" + parameter + "/apply", "I", [=](void *d) {
		int &id = ((int *)d)[0], *V = (int *)d + 1;
		if (id == -1) {//all channels at once
			for(int iPM=0; iPM<20; ++iPM) for (int iCh=0; iCh<12; ++iCh) allPMs[iPM].setParameter(parameter, V[20*iCh + iPM], iCh);
			
		} else if (id < 240) {
			int iPM = id % 20, iCh = id / 20;
			if ((1 << iPM & TCM.act.PM_MASK_SPI) == 0) return;
			TypePM *pm = allPMs + iPM;
			pm->setParameter(parameter, *V, iCh);
			writeParameter(parameter, *V, pm->FEEid, iCh);
		}
	});
}

void createDIMservices() { //+ system services
	QString pfx = QString::asprintf("%s/TCM/", FIT[subdetector].name);
	TCM.services.append(new DimService(qP(pfx+"status/TEMP_BOARD"  ), "F", &TCM.act.TEMP_BOARD                   , 4));
	TCM.services.append(new DimService(qP(pfx+"status/TEMP_FPGA"   ), "F", &TCM.act.TEMP_FPGA                    , 4));
	TCM.services.append(new DimService(qP(pfx+"status/VOLTAGE_1V"  ), "F", &TCM.act.VOLTAGE_1V                   , 4));
	TCM.services.append(new DimService(qP(pfx+"status/VOLTAGE_1_8V"), "F", &TCM.act.VOLTAGE_1_8V                 , 4));
	TCM.services.append(new DimService(qP(pfx+"status/SERIAL_NUM"  ), "S", (char *)&TCM.act.registers[0x7] + 1   , 2));
	TCM.services.append(new DimService(qP(pfx+"status/BOARD_TYPE"  ), "C:4",TCM.act.BOARD_TYPE                   , 4));
	TCM.services.append(new DimService(qP(pfx+"status/FW_TIME_MCU" ), "I", &TCM.act.FW_TIME_MCU                  , 4));
	TCM.services.append(new DimService(qP(pfx+"status/FW_TIME_FPGA"), "I", &TCM.act.FW_TIME_FPGA                 , 4));
	TCM.services.append(new DimService(qP(pfx+"status/PM_MASK_SPI" ), "I", &TCM.act.PM_MASK_SPI                  , 4));
	for (int iPM=0; iPM<10; ++iPM) {
		TCM.services.append(new DimService(qPf("%s/TCM/status/TRG_SYNC/%s", FIT[subdetector].name, allPMs[iPM   ].name), "I", TCM.act.TRG_SYNC_A + iPM, 4));
		TCM.services.append(new DimService(qPf("%s/TCM/status/TRG_SYNC/%s", FIT[subdetector].name, allPMs[iPM+10].name), "I", TCM.act.TRG_SYNC_C + iPM, 4));
	}

	for (int i=0; i<5; ++i) {
		TCM.staticServices.append(new DimService( qP(pfx+"Trigger"+QString::number(i+1)+"/NAME"     ),  const_cast<char   *>( FIT[subdetector].triggers[i].name     ) ));
		TCM.staticServices.append(new DimService( qP(pfx+"Trigger"+QString::number(i+1)+"/SIGNATURE"), *const_cast<int *>(&FIT[subdetector].triggers[i].signature) ));
	}
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd0/NAME"), (char *)("NoiseA: A-side out-of-gate hits AND NOT OrA"        ) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd1/NAME"), (char *)("NoiseC: C-side out-of-gate hits AND NOT OrC"        ) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd2/NAME"), (char *)("Total noise: NoiseA OR NoiseC"                      ) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd3/NAME"), (char *)("OrA in CB: bunch in both beams AND OrA"             ) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd4/NAME"), (char *)("OrC in CB: bunch in both beams AND OrC"             ) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd5/NAME"), (char *)("Interaction: both sides Or (OrA AND OrC)"           ) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd6/NAME"), (char *)("CB-Interaction: bunch in both beams AND Interaction") ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd7/NAME"), (char *)qP(QString("CB-%1: bunch in both beams AND %1").arg(FIT[subdetector].triggers[2].name)) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd8/NAME"), (char *)("Background A: bunch ONLY in beam1 AND OrC"          ) ));
	TCM.staticServices.append(new DimService( qP(pfx+"Bkgrnd9/NAME"), (char *)("Background C: bunch ONLY in beam2 AND OrA"          ) ));

	TCM.counters.services.append(new DimService(qP(pfx+"Trigger1/CNT"), "I", &TCM.counters.CNT_T1, 4));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger2/CNT"), "I", &TCM.counters.CNT_T2, 4));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger3/CNT"), "I", &TCM.counters.CNT_T3, 4));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger4/CNT"), "I", &TCM.counters.CNT_T4, 4));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger5/CNT"), "I", &TCM.counters.CNT_T5, 4));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger1/CNT_RATE"), "D", TCM.counters.rate + 3, 8));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger2/CNT_RATE"), "D", TCM.counters.rate + 2, 8));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger3/CNT_RATE"), "D", TCM.counters.rate + 4, 8));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger4/CNT_RATE"), "D", TCM.counters.rate + 1, 8));
	TCM.counters.services.append(new DimService(qP(pfx+"Trigger5/CNT_RATE"), "D", TCM.counters.rate    , 8));
	for (int i=5; i<15; ++i) {
		TCM.counters.services.append(new DimService(qP(pfx+"Bkgrnd"+QString::number(i-5)+"/CNT"), "I", TCM.counters.New + i, 4));
		TCM.counters.services.append(new DimService(qP(pfx+"Bkgrnd"+QString::number(i-5)+"/CNT_RATE"), "D", TCM.counters.rate + i, 8));
	}
	countsTriggers = new AdvancedDIMservice(qP(pfx+"CNT"), "I:15", 15*4, [=](void *d) {
		((int *)d)[0] = TCM.counters.CNT_T1;
		((int *)d)[1] = TCM.counters.CNT_T2;
		((int *)d)[2] = TCM.counters.CNT_T3;
		((int *)d)[3] = TCM.counters.CNT_T4;
		((int *)d)[4] = TCM.counters.CNT_T5;
		for (int i=5; i<15; ++i) ((int *)d)[i] = TCM.counters.New[i];
	});
	countRatesTriggers = new AdvancedDIMservice(qP(pfx+"CNT_RATE"), "D:15", 15*8, [=](void *d) {
		((double  *)d)[0] = TCM.counters.rate[3];
		((double  *)d)[1] = TCM.counters.rate[2];
		((double  *)d)[2] = TCM.counters.rate[4];
		((double  *)d)[3] = TCM.counters.rate[1];
		((double  *)d)[4] = TCM.counters.rate[0];
		for (int i=5; i<15; ++i) ((double  *)d)[i] = TCM.counters.rate[i];
	});
	//TCM.services.append(new DimService(qP(pfx+"Trigger1/OUTPUT_ENABLED"), "S", TCM.counters.rate    , 8));

	addCommand(TCM.commands, pfx+"control/ORBIT_FILL_MASK/set"  , "I:223", [=](void *d) { memcpy(TCM.ORBIT_FILL_MASK, d, 223*wordSize); apply_ORBIT_FILL_MASK(); }); //223 = ceil( 0xDEC * 2 / 32 )
	addCommand(TCM.commands, pfx+"control/ORBIT_FILL_MASK/apply", "I:223", [=](void *d) { memcpy(TCM.ORBIT_FILL_MASK, d, 223*wordSize); apply_ORBIT_FILL_MASK(); });

	pfx = QString(FIT[subdetector].name) + "/"; //for system services
	services.append(new AdvancedDIMservice(qP(pfx+"BOARDS_OK"), "I", 4, {}, &BOARDS_OK));
	services.append(new AdvancedDIMservice(qP(pfx+"LASER_ENABLED"      "/actual"), "S", 2, [=](void *d) { *(int *)d = TCM.act.LASER_ENABLED        ; }));
	services.append(new AdvancedDIMservice(qP(pfx+"LASER_SOURCE"       "/actual"), "S", 2, [=](void *d) { *(int *)d = TCM.act.LASER_SOURCE         ; }));
	services.append(new AdvancedDIMservice(qP(pfx+"LASER_DIVIDER"      "/actual"), "I", 4, [=](void *d) { *(int *)d = TCM.act.LASER_DIVIDER        ; }));
	services.append(new AdvancedDIMservice(qP(pfx+"LASER_FREQUENCY_Hz" "/actual"), "F", 4,              { },             &TCM.act.laserFrequency_Hz       ));
	services.append(new AdvancedDIMservice(qP(pfx+"LASER_DELAY_ns"     "/actual"), "F", 4,              { },             &TCM.act.delayLaser_ns           ));
	services.append(new AdvancedDIMservice(qP(pfx+"LASER_PATTERN"      "/actual"), "X", 8,              { },             &TCM.act.LASER_PATTERN           ));
	services.append(new AdvancedDIMservice(qP(pfx+"LSR_TRG_SUPPR_DUR"  "/actual"), "I", 4, [=](void *d) { *(int *)d = TCM.act.lsrTrgSupprDur       ; }));
	services.append(new AdvancedDIMservice(qP(pfx+"LSR_TRG_SUPPR_DELAY""/actual"), "I", 4, [=](void *d) { *(int *)d = TCM.act.lsrTrgSupprDelay     ; }));
	services.append(new AdvancedDIMservice(qP(pfx+"ATTEN_STEPS"        "/actual"), "I", 4, [=](void *d) { *(int *)d = TCM.act.attenSteps           ; }));
	services.append(new AdvancedDIMservice(qP(pfx+"ATTEN_STATUS"                ), "I", 4, [=](void *d) { *(int *)d = TCM.act.registers[0x3] >> 14 ; }));

	addCommand(commands, pfx+"LOAD_CONFIG"   , "C"  , [=](void *d) { QString name((char *)d); fileRead(name, true); });
	addCommand(commands, pfx+"CLEAR_ERRORS"  , "C:1", [=](void * ) { apply_RESET_ERRORS(); });
	addCommand(commands, pfx+"RECONNECT"     , "C:1", [=](void * ) { reconnect(); });
	addCommand(commands, pfx+"RESTART_SYSTEM", "C:1", [=](void * ) { apply_RESET_SYSTEM(false); });

	addCommand(commands, pfx+"LASER_ENABLED"      "/apply", "S", [=](void *d) { apply_LASER_ENABLED            (*(bool    *)d); });
	addCommand(commands, pfx+"LASER_SOURCE"       "/apply", "S", [=](void *d) { apply_LASER_SOURCE             (*(bool    *)d); });
	addCommand(commands, pfx+"LASER_FREQUENCY_Hz" "/apply", "F", [=](void *d) { TCM.set.calculate_LASER_DIVIDER(*(float   *)d); apply_LASER_DIVIDER      (); });
	addCommand(commands, pfx+"LASER_DELAY_ns"     "/apply", "F", [=](void *d) { TCM.set.calculate_LASER_DELAY  (*(float   *)d); apply_LASER_DELAY        (); });
	addCommand(commands, pfx+"LASER_PATTERN"      "/apply", "X", [=](void *d) { TCM.set.LASER_PATTERN         = *(quint64 *)d ; apply_LASER_PATTERN      (); });
	addCommand(commands, pfx+"LSR_TRG_SUPPR_DUR"  "/apply", "I", [=](void *d) { TCM.set.lsrTrgSupprDur        = *(int *)d ; apply_LSR_TRG_SUPPR_DUR  (); });
	addCommand(commands, pfx+"LSR_TRG_SUPPR_DELAY""/apply", "I", [=](void *d) { TCM.set.lsrTrgSupprDelay      = *(int *)d ; apply_LSR_TRG_SUPPR_DELAY(); });
	addCommand(commands, pfx+"LASER_DIVIDER"      "/apply", "I", [=](void *d) { TCM.set.LASER_DIVIDER         = *(int *)d ; apply_LASER_DIVIDER      (); });
	addCommand(commands, pfx+"ATTEN_STEPS"        "/apply", "I", [=](void *d) { TCM.set.attenSteps            = *(int *)d ; apply_attenSteps         (); });

	services.append(new AdvancedDIMservice(qP(pfx+"ADC_RMS"                ), "F:480", 2*12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh) {
		((float   *)d)[  0 + 20*iCh + iPM] = pm->act.RMS_Ch[iCh][0];
		((float   *)d)[240 + 20*iCh + iPM] = pm->act.RMS_Ch[iCh][1];                                         } }));
	services.append(new AdvancedDIMservice(qP(pfx+"ADC_MEANAMPL"           ), "I:480", 2*12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh) {
		(( int *)d)[  0 + 20*iCh + iPM] = pm->act.MEANAMPL[iCh][0][0];
		(( int *)d)[240 + 20*iCh + iPM] = pm->act.MEANAMPL[iCh][1][0];                                    } }));
	services.append(new AdvancedDIMservice(qP(pfx+"ADC_BASELINE"           ), "I:480", 2*12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh) {
		((int *)d)[  0 + 20*iCh + iPM] = pm->act.ADC_BASELINE[iCh][0];
		((int *)d)[240 + 20*iCh + iPM] = pm->act.ADC_BASELINE[iCh][1];                                   } }));
	services.append(new AdvancedDIMservice(qP(pfx+"ADC_ZERO"      "/actual"), "I:240",   12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh)
		(( int *)d)[  0 + 20*iCh + iPM] = pm->act.Ch[iCh].ADC_ZERO;                                         }));
	services.append(new AdvancedDIMservice(qP(pfx+"ADC_DELAY"     "/actual"), "I:240",   12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh)
		((int *)d)[  0 + 20*iCh + iPM] = pm->act.Ch[iCh].ADC_DELAY;                                        }));
	services.append(new AdvancedDIMservice(qP(pfx+"ADC_RANGE"     "/actual"), "I:480", 2*12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh) {
		((int *)d)[  0 + 20*iCh + iPM] = pm->act.ADC_RANGE[iCh][0];
		((int *)d)[240 + 20*iCh + iPM] = pm->act.ADC_RANGE[iCh][1];                                      } }));
	services.append(new AdvancedDIMservice(qP(pfx+"TIME_ALIGN"    "/actual"), "I:240",   12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh)
		(( int *)d)[  0 + 20*iCh + iPM] = pm->act.timeAlignment[iCh].value;                                 }));
	services.append(new AdvancedDIMservice(qP(pfx+"CFD_ZERO"      "/actual"), "I:240",   12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh)
		(( int *)d)[  0 + 20*iCh + iPM] = pm->act.Ch[iCh].CFD_ZERO;                                         }));
	services.append(new AdvancedDIMservice(qP(pfx+"CFD_THRESHOLD" "/actual"), "I:240",   12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh)
		((int *)d)[  0 + 20*iCh + iPM] = pm->act.Ch[iCh].CFD_THRESHOLD;                                    }));
	services.append(new AdvancedDIMservice(qP(pfx+"THRESHOLD_CALIBR/actual"), "I:240",   12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh)
		((int *)d)[  0 + 20*iCh + iPM] = pm->act.THRESHOLD_CALIBR[iCh];                                    }));

	countRatesChannels = new AdvancedDIMservice(qP(pfx+"CNT_RATE_CH"       ), "F:480", 2*12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh) {
		((float   *)d)[  0 + 20*iCh + iPM] = pm->counters.rateCh[iCh].CFD;
		((float   *)d)[240 + 20*iCh + iPM] = pm->counters.rateCh[iCh].TRG;                                   } });
	countsChannels     = new AdvancedDIMservice(qP(pfx+"CNT_CH"            ), "I:480", 2*12*20*4, [=](void *d) { foreach(TypePM *pm, PM) for(int iCh=0, iPM=pm-allPMs; iCh<12; ++iCh) {
		((int *)d)[  0 + 20*iCh + iPM] = pm->counters.Ch[iCh].CFD;
		((int *)d)[240 + 20*iCh + iPM] = pm->counters.Ch[iCh].TRG;                                       } });

	services.append(new AdvancedDIMservice(qP(pfx+"CH_MASK_DATA"  "/actual"), "I:20" ,      20*4, [=](void *d) { foreach(TypePM *pm, PM) ((int *)d)[pm-allPMs] = pm->act.CH_MASK_DATA; }));
	services.append(new AdvancedDIMservice(qP(pfx+"CH_MASK_TRG"   "/actual"), "I:20" ,      20*4, [=](void *d) { foreach(TypePM *pm, PM) ((int *)d)[pm-allPMs] = pm->act.CH_MASK_TRG ; }));

	services.append(new AdvancedDIMservice(qP(pfx+"GBT/RX_PHASE"           ), "I:21" , 4     *21, [=](void *d) {
		for(TypePM *pm=allPMs, *e=pm+20; pm<e; ++pm) ((int *)d)[pm-allPMs] = (PM.contains(pm->FEEid) ? pm->act.GBT.Status.RX_PHASE : -1);
														((int *)d)[20]        =                           TCM.act.GBT.Status.RX_PHASE;
	}));

	addArrayCommand("THRESHOLD_CALIBR");
	addArrayCommand("ADC_ZERO"        );
	addArrayCommand("ADC_DELAY"       );
	addArrayCommand("TIME_ALIGN"      );
	addArrayCommand("CFD_ZERO"        );
	addArrayCommand("CFD_THRESHOLD"   );
	addCommand(commands, pfx+"ADC_RANGE/apply", "I", [=](void *d) {
		int &id = ((int *)d)[0], *V = (int *)d + 1;
		if (id == -1) {
			for(int iPM=0; iPM<20; ++iPM) for (int iCh=0; iCh<12; ++iCh) { allPMs[iPM].set.ADC_RANGE[iCh][0] = V[20*iCh + iPM]; allPMs[iPM].set.ADC_RANGE[iCh][1] = V[240 + 20*iCh + iPM]; }
			
		} else if (id < 480) {
			int iPM = id % 20, iCh = id / 20 % 12, iADC = id / 240;
			if ((1 << iPM & TCM.act.PM_MASK_SPI) == 0) return;
			TypePM *pm = allPMs + iPM;
			pm->set.ADC_RANGE[iCh][iADC] = *V;
			writeRegister(pm->baseAddress + PMparameters["ADC0_RANGE"].address + 2*iCh + iADC, *V);
		}
	});
	addCommand(commands, pfx+"CH_MASK_DATA/apply", "I", [=](void *d) {
		int &id = ((int *)d)[0], *V = (int *)d + 1;
		if (id == -1) {
			for(int iPM=0; iPM<20; ++iPM) { allPMs[iPM].set.CH_MASK_DATA = V[iPM]; }
		} else if (id < 240) {
			int iPM = id % 20, iCh = id / 20;
			if ((1 << iPM & TCM.act.PM_MASK_SPI) == 0) return;
			TypePM *pm = allPMs + iPM;
			pm->set.CH_MASK_DATA = changeNbits(pm->set.CH_MASK_DATA, 1, iCh, *V);
			*V ? setBit(iCh, pm->baseAddress + PMparameters["CH_MASK_DATA"].address) : clearBit(iCh, pm->baseAddress + PMparameters["CH_MASK_DATA"].address);
		}
	});
	addCommand(commands, pfx+"CH_MASK_TRG/apply", "I", [=](void *d) {
		int &id = ((int *)d)[0], *V = (int *)d + 1;
		if (id == -1) {
			for(int iPM=0; iPM<20; ++iPM) for (int iCh=0; iCh<12; ++iCh) {
				bool enableTrigger = V[iPM] & 1<<iCh;
				allPMs[iPM].setParameter("noTriggerMode", !enableTrigger, iCh);
				allPMs[iPM].act.timeAlignment[iCh].blockTriggers = !enableTrigger;
			}
		} else if (id < 240) {
			int iPM = id % 20, iCh = id / 20;
			bool enableTrigger = *V;
			if ((1 << iPM & TCM.act.PM_MASK_SPI) == 0) return;
			TypePM *pm = allPMs + iPM;
			pm->setParameter("noTriggerMode", !enableTrigger, iCh);
			writeParameter("noTriggerMode", !enableTrigger, pm->FEEid, iCh);
		}
	});

	addCommand(commands, pfx+"RESET_COUNTS", "I", [=](void *d) {
		int id = *(int *)d;
		if (id > 20 || id < -2) return;
		//resetCounts(id >= 0 ? (id == 20 ? TCMid : allPMs[id].FEEid) : id);
	});
	addCommand(commands, pfx+"GBT/SUPPRESS_ERROR_REPORTS", "I", [=](void *d) { switchGBTerrorReports(!*(int *)d); });
}*/