#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "device.h"
#include "FPGA/FPGA_Osci.h"
#include "Osci/Osci.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Init()
{
    Settings::Range::LoadBoth();
    Settings::RShift::Load(Chan::A);
    Settings::RShift::Load(Chan::B);
    Settings::Trig::Input::Load();
    Settings::Trig::Level::Load();
    Settings::TBase::Load();
    Settings::TShift::Load();
    FPGA::Settings::LoadCalibratorMode();
    Settings::LoadHoldfOff();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::DeInit()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Update()
{
    if (!Device::State::InModeOsci())
    {
        return;
    }

    FPGA::Osci::Update();
}
