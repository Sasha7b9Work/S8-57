#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "FPGA/FPGATypes.h"
#include "Hardware/FSMC.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Settings::Load()
{
    Range::LoadBoth();
    RShift::Load(Chan::A);
    RShift::Load(Chan::B);
    Trig::Input::Load();
    Trig::Level::Load();
    TBase::Load();
    TShift::Load();
    FPGA::Settings::LoadCalibratorMode();
    LoadHoldfOff();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::LoadHoldfOff()
{
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_ENABLE, TRIG_HOLDOFF_ENABLED ? 1U : 0U);

    uint value = (uint)(0 - TRIG_HOLDOFF + 1);

    BitSet32 bs(value);

    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_LOW, bs.byte0);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_MID, bs.byte1);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_HI, bs.byte2);
}
