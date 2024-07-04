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
		DimServer DIMserver;
		
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

		DIMserver.setDnsNode("localhost");
		//DIMserver.setDnsNode("server.dyndns.cern.ch");
		//DIMserver.start("_DIM_SERVER");
		//dis_set_debug_on();
		DimServer::start();
		Print::PrintInfo("DIM started");
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