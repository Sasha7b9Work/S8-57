#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Hardware/Timer.h"
#include "Keyboard/Decoder.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Hardware/Sound.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stdlib.h>
#include <cmath>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Device::Mode::E Device::currentMode = Device::Mode::Osci;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Device::Init()
{
#ifndef WIN32

    Hardware::Init();

    Tester::Init();

#endif

    Display::Init();

    PAUSE_ON_MS(500);

    FPGA::Init();

    Settings::Load();

#ifndef WIN32

    FrequencyCounter::Init();

    FPGA::LoadSettings();

    Menu::Init();

    FPGA::OnPressStart();

    Multimeter::Init();

    if (Menu::OpenedItem() == (Page *)PageFunction::PageMultimeter::pointer)
    {
        SetMode(Mode::Multimeter);
    }

#endif
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Update()
{
    START_MULTI_MEASUREMENT();

    Display::Update();

    FPGA::Update();

    Menu::SaveSettings();

    Tester::Update();

    Multimeter::Update();

    Menu::Update();

    Decoder::Update();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Device::Mode::E Device::CurrentMode()
{
    return currentMode;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::SetMode(Mode::E mode)
{
    currentMode = mode;
    if(currentMode == Mode::Tester)
    {
        Tester::Enable();
    }
    else
    {
        Tester::Disable();
    }
}
