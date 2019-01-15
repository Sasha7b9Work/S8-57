#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#endif


using namespace FPGA::SET;


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
    LoadCalibratorMode();
    LoadHoldfOff();
}
