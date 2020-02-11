#include "defines.h"
#include "Settings/SettingsOsci.h"
#include "Settings/SettingsNRST.h"


SettingsNRST setNRST;


SettingsNRST defaultNRST =
{
    0,
    {0, 0},
    0,                                      // numAveForRand
    0,                                      // numSmoothForRand
    0,                                      // correctionTime
    0,                                      // enum_gate_max
    0,                                      // enum_gate_min
    { BalanceADC::Disabled, {0,    0}    }, // balanceADC
    { ShiftADC::Disable,    {{0},  {0}}  }, // shiftADC
    { StretchADC::Disabled, {1.0F, 1.0F} }  // stretchADCtype
};


void SettingsNRST::Init()
{

}


void SettingsNRST::Save()
{

}
