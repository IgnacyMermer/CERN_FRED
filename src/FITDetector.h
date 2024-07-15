#ifndef FITDETECTOR_H
#define FITDETECTOR_H


enum TypeFITDetector {_0_=0, FT0=1, FV0=2, FDD=3};
inline TypeFITDetector getDetectorName (std::string s){
    if(s=="FT0") return FT0;
    if(s=="FV0") return FV0;
    if(s=="FDD") return FDD;
    return _0_;
}


const struct {char name[4]; int TCMid, PMA0id, PMC0id; int systemID; struct {const char *name; int signature;} triggers[5];} FIT[4] = { //global static constants
             {       "???",        0xFFFF, 0x0000, 0x000A,               0,        { {    "Trigger1",              75},
                                                                                     {    "Trigger2",              76},
                                                                                     {    "Trigger3",              77},
                                                                                     {    "Trigger4",              78},
                                                                                     {    "Trigger5",              79} }             },
             {       "FT0",        0xFA00, 0xFAA0, 0xFAF0,              34,        { {     "Central",              70},
                                                                                     {"Semi Central",              71},
                                                                                     {      "Vertex",              72},
                                                                                     {         "OrC",              73},
                                                                                     {         "OrA",              74} }             },
             {       "FV0",        0x55F0, 0x55A0, 0x55AA,              35,        { {      "Charge",              40},
                                                                                     {   "Nchannels",              41},
                                                                                     {  "InnerRings",              42},
                                                                                     {  "OuterRings",              43},
                                                                                     {          "Or",              44} }             },
             {       "FDD",        0x0FA0, 0x0FAA, 0x0FFF,              33,        { {     "Central",             110},
                                                                                     {"Semi Central",             111},
                                                                                     {      "Vertex",             112},
                                                                                     {         "OrC",             113},
                                                                                     {         "OrA",             114} }             }
};


#endif // FITDETECTOR_H