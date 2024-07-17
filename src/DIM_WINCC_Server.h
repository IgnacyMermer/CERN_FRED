#ifndef DIM_WINCC_SERVER_H
#define DIM_WINCC_SERVER_H

#include "./FITDetector.h"
#include <stdio.h>
#include <string.h>
#include "./PM.h"
#include "./TCM.h"
#include <cstring>
#include "./FITDetector.h"
#include "./IPbusControlPacket.h"

class DIM_WINCC_Server {
public:
    TypeFITDetector detectorType;
    //DimServer dimServer;
    struct TypeServerStatus {
        char string[1024] = "offline";
        //DimService *service;
        void update(const char* st) {
            strcpy(string, st);
            //service->updateService();
        }
    } serverStatus;

    TypeTCM TCM;
    //IPbusControlPacket ipbus;
    //IPbusControlPacket p; 
    //connect(&p, &IPbusControlPacket::error, this, &IPbusTarget::error);
    //foreach(regblock b, TCM.act.regblocks) p.addTransaction(read, b.addr, TCM.act.registers + b.addr, b.size());

    TypePM allPMs[20] = {
        TypePM(0x0200, "A0", TCM.act.TRG_SYNC_A[0]),
        TypePM(0x0400, "A1", TCM.act.TRG_SYNC_A[1]),
        TypePM(0x0600, "A2", TCM.act.TRG_SYNC_A[2]),
        TypePM(0x0800, "A3", TCM.act.TRG_SYNC_A[3]),
        TypePM(0x0A00, "A4", TCM.act.TRG_SYNC_A[4]),
        TypePM(0x0C00, "A5", TCM.act.TRG_SYNC_A[5]),
        TypePM(0x0E00, "A6", TCM.act.TRG_SYNC_A[6]),
        TypePM(0x1000, "A7", TCM.act.TRG_SYNC_A[7]),
        TypePM(0x1200, "A8", TCM.act.TRG_SYNC_A[8]),
        TypePM(0x1400, "A9", TCM.act.TRG_SYNC_A[9]),
        TypePM(0x1600, "C0", TCM.act.TRG_SYNC_C[0]),
        TypePM(0x1800, "C1", TCM.act.TRG_SYNC_C[1]),
        TypePM(0x1A00, "C2", TCM.act.TRG_SYNC_C[2]),
        TypePM(0x1C00, "C3", TCM.act.TRG_SYNC_C[3]),
        TypePM(0x1E00, "C4", TCM.act.TRG_SYNC_C[4]),
        TypePM(0x2000, "C5", TCM.act.TRG_SYNC_C[5]),
        TypePM(0x2200, "C6", TCM.act.TRG_SYNC_C[6]),
        TypePM(0x2400, "C7", TCM.act.TRG_SYNC_C[7]),
        TypePM(0x2600, "C8", TCM.act.TRG_SYNC_C[8]),
        TypePM(0x2800, "C9", TCM.act.TRG_SYNC_C[9]),
    };

    TypePM pm = TypePM(0x0200, "A0", TCM.act.TRG_SYNC_A[0]);


    DIM_WINCC_Server(TypeFITDetector detector): detectorType(detector){
        int a=0;
        //createPMServices(allPMs);
        createDIMServices();
    }

    void createPMServices(TypePM *myPM){
        
        string prefix = std::string(FIT[1].name)+"/PM/"/*+std::to_string(pm->name)*/;

        for(int i=0; i<1; i++){
            myPM->services.push_back(new DimService((prefix+"status/TEMP_BOARD").c_str(), "F", static_cast<void*>(&pm.act.TEMP_BOARD), 4));
            myPM->services.push_back(new DimService((prefix+"status/TEMP_FPGA").c_str(), "F", static_cast<void*>(&pm.act.TEMP_FPGA), 4));
            myPM->services.push_back(new DimService((prefix+"status/VOLTAGE_1V").c_str(), "F", static_cast<void*>(&pm.act.VOLTAGE_1V), 4));
            myPM->services.push_back(new DimService((prefix+"status/VOLTAGE_1_8V").c_str(), "F", static_cast<void*>(&pm.act.VOLTAGE_1_8V), 4));
            myPM->services.push_back(new DimService((prefix+"status/BOARD_TYPE").c_str(), "C:4", static_cast<void*>(&pm.act.BOARD_TYPE), 4));
            myPM->services.push_back(new DimService((prefix+"status/FW_TIME_MCU").c_str(), "I", static_cast<void*>(&pm.act.FW_TIME_MCU), 4));
            myPM->services.push_back(new DimService((prefix+"status/FW_TIME_FPGA").c_str(), "I", static_cast<void*>(&pm.act.FW_TIME_FPGA), 4));
            myPM->services.push_back(new DimService((prefix+"status/CH_BASELINES_NOK").c_str(), "I", static_cast<void*>(&pm.act.CH_BASELINES_NOK), 4));
            myPM->services.push_back(new DimService((prefix+"status/SERIAL_NUM").c_str(), "S", static_cast<void*>((char*)&pm.act.registers[0xBD]+1), 2));
            myPM->services.push_back(new DimService((prefix+"status/CH_MASK_DATA").c_str(), "I", static_cast<void*>(&pm.act.CH_MASK_DATA), 4));
            myPM->services.push_back(new DimService((prefix+"status/CH_MASK_TRG").c_str(), "I", static_cast<void*>(&pm.act.CH_MASK_TRG), 4));
        }
    }

    void createDIMServices(){
        
        string prefix = std::string(FIT[1].name)+"/TCM/"/*+std::to_string(pm->name)*/;

        for(int i=0; i<1; i++){
            TCM.services.push_back(new DimService((prefix+"status/TEMP_BOARD").c_str(), "F", static_cast<void*>(&TCM.act.TEMP_BOARD), 4));
            TCM.services.push_back(new DimService((prefix+"status/TEMP_FPGA").c_str(), "F", static_cast<void*>(&TCM.act.TEMP_FPGA), 4));
            TCM.services.push_back(new DimService((prefix+"status/VOLTAGE_1V").c_str(), "F", static_cast<void*>(&TCM.act.VOLTAGE_1V), 4));
            TCM.services.push_back(new DimService((prefix+"status/VOLTAGE_1_8V").c_str(), "F", static_cast<void*>(&TCM.act.VOLTAGE_1_8V), 4));
            TCM.services.push_back(new DimService((prefix+"status/BOARD_TYPE").c_str(), "C:4", static_cast<void*>(&TCM.act.BOARD_TYPE), 4));
            TCM.services.push_back(new DimService((prefix+"status/FW_TIME_MCU").c_str(), "I", static_cast<void*>(&TCM.act.FW_TIME_MCU), 4));
            TCM.services.push_back(new DimService((prefix+"status/FW_TIME_FPGA").c_str(), "I", static_cast<void*>(&TCM.act.FW_TIME_FPGA), 4));
            //TCM.services.push_back(new DimService((prefix+"status/CH_BASELINES_NOK").c_str(), "I", static_cast<void*>(&TCM.act.CH_BASELINES_NOK), 4));
            TCM.services.push_back(new DimService((prefix+"status/SERIAL_NUM").c_str(), "S", static_cast<void*>((char*)&TCM.act.registers[0xBD]+1), 2));
            //TCM.services.push_back(new DimService((prefix+"status/CH_MASK_DATA").c_str(), "I", static_cast<void*>(&TCM.act.CH_MASK_DATA), 4));
            //TCM.services.push_back(new DimService((prefix+"status/CH_MASK_TRG").c_str(), "I", static_cast<void*>(&TCM.act.CH_MASK_TRG), 4));
            TCM.services.push_back(new DimService((prefix+"status/PM_MASK_SPI").c_str(), "I", static_cast<void*>(&TCM.act.PM_MASK_SPI), 4));
        }

        for (int iPM=0; iPM<10; ++iPM) {
            TCM.services.push_back(new DimService((prefix+"status/TRG_SYNC/"+allPMs[iPM].name).c_str(), "I", static_cast<void*>(TCM.act.TRG_SYNC_A + iPM), 4));
            TCM.services.push_back(new DimService((prefix+"status/TRG_SYNC/"+allPMs[iPM+10].name).c_str(), "I", static_cast<void*>(TCM.act.TRG_SYNC_C + iPM), 4));
        }

        for (int i=0; i<5; ++i) {
            TCM.staticServices.push_back(new DimService((prefix+"Trigger"+std::to_string(i+1)+"/NAME").c_str(), const_cast<char*>(FIT[1].triggers[i].name)));
            TCM.staticServices.push_back(new DimService((prefix+"Trigger"+std::to_string(i+1)+"/SIGNATURE").c_str(), *const_cast<int*>(&FIT[1].triggers[i].signature)));
        }
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd0/NAME").c_str(), (char *)("NoiseA: A-side out-of-gate hits AND NOT OrA")));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd1/NAME").c_str(), (char *)("NoiseC: C-side out-of-gate hits AND NOT OrC") ));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd2/NAME").c_str(), (char *)("Total noise: NoiseA OR NoiseC")));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd3/NAME").c_str(), (char *)("OrA in CB: bunch in both beams AND OrA")));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd4/NAME").c_str(), (char *)("OrC in CB: bunch in both beams AND OrC")));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd5/NAME").c_str(), (char *)("Interaction: both sides Or (OrA AND OrC)")));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd6/NAME").c_str(), (char *)("CB-Interaction: bunch in both beams AND Interaction") ));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd7/NAME").c_str(), (char *)(("CB-"+std::string(FIT[1].triggers[2].name)+": bunch in both beams AND "+std::string(FIT[1].triggers[2].name)).c_str())));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd8/NAME").c_str(), (char *)("Background A: bunch ONLY in beam1 AND OrC")));
        TCM.staticServices.push_back(new DimService((prefix+"Bkgrnd9/NAME").c_str(), (char *)("Background C: bunch ONLY in beam2 AND OrA")));

        TCM.counters.services.push_back(new DimService((prefix+"Trigger1/CNT").c_str(), "I", static_cast<void*>(&TCM.counters.CNT_T1), 4));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger2/CNT").c_str(), "I", static_cast<void*>(&TCM.counters.CNT_T2), 4));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger3/CNT").c_str(), "I", static_cast<void*>(&TCM.counters.CNT_T3), 4));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger4/CNT").c_str(), "I", static_cast<void*>(&TCM.counters.CNT_T4), 4));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger5/CNT").c_str(), "I", static_cast<void*>(&TCM.counters.CNT_T5), 4));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger1/CNT_RATE").c_str(), "D", static_cast<void*>(TCM.counters.rate + 3), 8));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger2/CNT_RATE").c_str(), "D", static_cast<void*>(TCM.counters.rate + 2), 8));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger3/CNT_RATE").c_str(), "D", static_cast<void*>(TCM.counters.rate + 4), 8));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger4/CNT_RATE").c_str(), "D", static_cast<void*>(TCM.counters.rate + 1), 8));
        TCM.counters.services.push_back(new DimService((prefix+"Trigger5/CNT_RATE").c_str(), "D", static_cast<void*>(TCM.counters.rate), 8));
        for (int i=5; i<15; ++i) {
            TCM.counters.services.push_back(new DimService((prefix+"Bkgrnd"+std::to_string(i-5)+"/CNT").c_str(), "I", static_cast<void*>(TCM.counters.New + i), 4));
            TCM.counters.services.push_back(new DimService((prefix+"Bkgrnd"+std::to_string(i-5)+"/CNT_RATE").c_str(), "D", static_cast<void*>(TCM.counters.rate + i), 8));
        }
    }


};

#endif // DIM_WINCC_SERVER_H
