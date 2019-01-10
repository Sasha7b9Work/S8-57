#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Hardware/Timer.h"
#include "Keyboard/DecoderDevice.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Hardware/Sound.h"
#include "Hardware/VCP.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stdlib.h>
#include <cmath>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Device::Mode::E currentMode = Device::Mode::Osci;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Device::Init()
{
    Hardware::Init();

    VCP::Init();

    Tester::Init();

    Display::Init();

    PAUSE_ON_MS(500);

    FPGA::Init();

    Settings::Load();

    FrequencyCounter::Init();

    FPGA::LoadSettings();

    Menu::Init();

    FPGA::OnPressStart();

    Multimeter::Init();

    FDrive::Init();

    if (Menu::OpenedItem() == (Page *)PageFunction::PageMultimeter::pointer)
    {
        SetMode(Mode::Multimeter);
    }

    if (Menu::OpenedItem() == (Page *)PageFunction::PageTester::pointer)
    {
        SetMode(Mode::Tester);
    }
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

    FDrive::Update();

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::InModeTester()
{
    return (currentMode == Device::Mode::Tester);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::InModeMultimeter()
{
    return (currentMode == Device::Mode::Multimeter);
}
