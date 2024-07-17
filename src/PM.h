#ifndef PM_H
#define PM_H

#include <cstring>
#include "./FITDetector.h"
#include <vector>
#include <cmath>
#include "./TCM.h"

struct TypePM {
    struct ActualValues{
        uint32_t OR_GATE            : 8, //┐
                PairedChannelsMode : 1, //│00
                                   :23; //⌡
        struct TimeAlignment {          //┐
            int32_t                      //│
                value              :12, //│
                blockTriggers      : 1, //│01-0C
                                   :19; //│
        }      timeAlignment[12]      ; //⌡
        uint32_t ADC_BASELINE[12][2]   , //]0D-24 //[Ch][0] for ADC0, [Ch][1] for ADC1
                ADC_RANGE   [12][2]   , //]25-3C
                TRGchargeLevelHi   :12, //┐
                TRGchargeLevelLo   : 4, //│3D
                                   :16; //⌡
        int32_t  TDC1tuning         : 8, //┐
                TDC2tuning         : 8, //│3E
                                   :16, //⌡
                TDC3tuning         : 8, //┐
                                   :24; //┘3F
        uint8_t  RAW_TDC_DATA[12][4]   ; //]40-4B //[Ch][0] for val1, [Ch][1] for val2
        uint32_t DISPERSION  [12][2]   ; //]4C-63
        int16_t  MEANAMPL    [12][2][2]; //]64-7B //[Ch][0][0] for ADC0, [Ch][1][0] for ADC1
        uint32_t CH_MASK_DATA          , //]7C
                CH_BASELINES_NOK      , //]7D
                _reservedSpace0       , //]7E
                mainPLLlocked      : 1, //┐
                TDC1PLLlocked      : 1, //│
                TDC2PLLlocked      : 1, //│
                TDC3PLLlocked      : 1, //│
                GBTRxReady         : 1, //│
                GBTRxError         : 1, //│
                TDC1syncError      : 1, //│
                TDC2syncError      : 1, //│
                TDC3syncError      : 1, //│7F
                RESET_COUNTERS     : 1, //│
                TRGcountMode       : 1, //│
                restartDetected    : 1, //│
                GBTRxPhaseError    : 1, //│
                BCIDsyncLost       : 1, //│
                droppingHits       : 1, //│
                                   :17; //⌡
        struct ChannelSettings {        //┐
            uint32_t CFD_THRESHOLD  :16, //│
                                   :16; //│
            int32_t  CFD_ZERO       :16, //│
                                   :16, //│
                    ADC_ZERO       :16, //│80-AF
                                   :16; //│
            uint32_t ADC_DELAY      :16, //│
                                   :16; //│
        } Ch[12];                       //⌡
        uint32_t THRESHOLD_CALIBR[12]  , //]B0-BB
                boardTemperature   :16, //┐
                                   :16; //┘BC
        uint32_t boardType          : 2, //┐
                                   : 6, //│
                SERIAL_NUM         : 8, //│BD
                                   :16, //⌡
                restartReasonCode  : 2, //┐
                                   :30, //┘BE
                _reservedSpace1[0xD8 - 0xBE - 1];
        //GBTunit GBT;                    //]D8-F1
        uint32_t _reservedSpace2[0xF7 - 0xF1 - 1];
        Timestamp FW_TIME_MCU;      //]F7
        uint32_t _reservedSpace3[0xFC - 0xF7 - 1];
        uint32_t FPGAtemperature,        //]FC
                voltage1,               //]FD
                voltage1_8;             //]FE
        Timestamp FW_TIME_FPGA;         //]FF
        uint32_t *registers = (uint32_t *)this;
        /*static const inline std::vector<regblock> regblocks {{0x00, 0x7D}, //block0     , 126 registers
                                                         {0x7F, 0xBE}, //block1     ,  64 registers
                                                         {0xD8, 0xE4}, //GBTcontrol ,  13 registers
                                                         {0xE8, 0xF1}, //GBTstatus  ,  10 registers
                                                         {0xF7, 0xF7}, //FW_TIME_MCU
                                                         {0xFC, 0xFF}};//block2     ,   4 registers*/
        float //calculable values
            TEMP_BOARD = 20.0F,
            TEMP_FPGA  = 20.0F,
            VOLTAGE_1V   = 1.0F,
            VOLTAGE_1_8V = 1.8F,
            RMS_Ch[12][2]; //[Ch][0] for ADC0, [Ch][1] for ADC1
        int16_t
            TIME_ALIGN[12]  ,
            TRG_CNT_MODE    ;
        uint32_t CH_MASK_TRG;
        char BOARD_TYPE[4] = {0};
        void calculateValues() {
            TEMP_BOARD   = boardTemperature / 10.;
            TEMP_FPGA    = FPGAtemperature  * 503.975 / 65536 - 273.15;
            VOLTAGE_1V   = voltage1         *   3.    / 65536;
            VOLTAGE_1_8V = voltage1_8       *   3.    / 65536;
            CH_MASK_TRG = 0;
            for (int iCh=0; iCh<12; ++iCh) {
                RMS_Ch[iCh][0] = std::sqrt(DISPERSION[iCh][0]);
                RMS_Ch[iCh][1] = std::sqrt(DISPERSION[iCh][1]);
                TIME_ALIGN[iCh] = timeAlignment[iCh].value;
                CH_MASK_TRG |= !timeAlignment[iCh].blockTriggers << iCh;
                TRG_CNT_MODE = TRGcountMode;
            }
            memcpy(BOARD_TYPE, FIT[boardType].name, 4);
        }
    } act;

    struct Settings {
        uint32_t OR_GATE            : 8, //┐
                PairedChannelsMode : 1, //│00
                                   :23; //⌡
        struct TimeAlignment {          //┐
            int32_t                      //│
                value              :12, //│
                blockTriggers      : 1, //│01-0C
                                   :19; //│
        } TIME_ALIGN           [12]   ;  //⌡
        uint32_t _reservedSpace0[0x25 - 0x0C - 1],
                ADC_RANGE      [12][2], //]25-3C
                TRGchargeLevelHi   :12, //┐
                TRGchargeLevelLo   : 4, //│3D
                                   :16, //⌡
                _reservedSpace1[0x7C - 0x3D - 1],
                CH_MASK_DATA          , //]7C
                _reservedSpace2[0x80 - 0x7C - 1];
        struct ChannelSettings {        //┐
            uint32_t CFD_THRESHOLD  :16, //│
                                   :16; //│
            int32_t  CFD_ZERO       :16, //│
                                   :16, //│80-AF
                    ADC_ZERO       :16, //│
                                   :16; //│
            uint32_t ADC_DELAY      :16, //│
                                   :16; //│
        } Ch[12];                       //⌡
        uint32_t  THRESHOLD_CALIBR[12] , //]B0-BB
                _reservedSpace3[0xD8 - 0xBB - 1];
        //GBTunit::ControlData GBT;       //]D8-E7

        uint32_t *registers = (uint32_t *)this;
        /*static const inline std::vector<regblock> regblocks {{0x00, 0x0C}, //block0     , 13 registers
                                                         {0x25, 0x3D}, //block1     , 25 registers
                                                         {0x7C, 0x7C}, //CH_MASK_DATA
                                                         {0x80, 0xBB}, //block2     , 60 registers
                                                         {0xD8, 0xE4}};//GBT control, 13 registers*/
    } set;

    struct Counters {
        static const uint16_t
            addressFIFO     = 0x100,
            addressFIFOload = 0x101;
        static const uint8_t
            number = 24,
            addressDirect   =  0xC0;
        uint32_t FIFOload;
        //QDateTime newTime, oldTime = QDateTime::currentDateTime();
        union {
            uint32_t New[number] = {0};
            struct {
                uint32_t CFD,
                        TRG;
            } Ch[12];
        };
        uint32_t Old[number] = {0};
        union {
            float rate[number] = {-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1};
            struct {
                float CFD,
                      TRG;
            } rateCh[12];
        };
        //GBTcounters GBT;
    } counters;

    std::list<DimService *> services;
    std::list<DimCommand *> commands;
    int FEEid;
    const uint16_t baseAddress;
    const char *name;
    std::string fullName() const { return std::string("PM") + std::string(name); }
    TRGsyncStatus &TRGsync;
    /*struct { uint32_t
        BCsyncLostInRun : 1 = 0,
        earlyHeader     : 1 = 0;
    } errorsLogged;*/

    bool isOK() { return
         act.mainPLLlocked &&
         act.TDC1PLLlocked &&
         act.TDC2PLLlocked &&
         act.TDC3PLLlocked &&
        !act.TDC1syncError &&
        !act.TDC2syncError &&
        !act.TDC3syncError &&
         //TRGsync.linkOK    &&
        //!TRGsync.syncError &&
         act.restartReasonCode != 2 ; //not by PLL relock
    }

    /*bool GBTisOK() const { return
        act.GBT.isOK() &&
        act.GBTRxReady;
    }*/

    bool setParameter(string parameterName, int value, int iCh = 0) {
        //if (!PMparameters.contains(parameterName)) return false;
        //Parameter par = PMparameters[parameterName];
        //int &reg = set.registers[par.address + iCh * par.interval];
        //reg = changeNbits(reg, par.bitwidth, par.bitshift, value);
        return true;
    }

    TypePM(int addr, const char *PMname, TRGsyncStatus &TRGsyncRef) : baseAddress(addr), name(PMname), TRGsync(TRGsyncRef) {}
};

#endif // PM_H