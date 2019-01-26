#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/FSMC.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Keyboard/DecoderDevice.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stdlib.h>
#include <cmath>

#include "Utils/Debug.h"


using Hardware::Timer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Device
{
    /// Настроить устройство в соответствии с установленным режимом
    static void SetCurrentMode();
    /// Установить режим работы mode, если открыта страница page или в числе предков открытой страницы есть page
    static bool SetCurrentMode(const PageBase *page, Mode::E mode);

    static Mode::E currentMode = Mode::Osci;
}



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

    Osci::Init();

    Menu::Init();

    Multimeter::Init();

    FDrive::Init();

    SetCurrentMode();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Device::SetCurrentMode()
{
    if (!SetCurrentMode(PageFunction::PageMultimeter::pointer, Device::Mode::Multimeter))
    {
        if (!SetCurrentMode(PageFunction::PageTester::pointer, Device::Mode::Tester))
        {
            if (!SetCurrentMode(PageFunction::PageRecorder::pointer, Device::Mode::Recorder))
            {
                State::SetMode(Mode::Osci);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool Device::SetCurrentMode(const PageBase *page, Device::Mode::E mode)
{
    Control *opened = Menu::OpenedItem();

    if (opened == (Page *)page || opened->ExistKeeper(page))
    {
        State::SetMode(mode);
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Update()
{
    START_MULTI_MEASUREMENT();

    //LOG_WRITE("");

    Display::Update();

    Osci::Update();

    Menu::SaveSettings();

    Recorder::Update();

    Tester::Update();

    Multimeter::Update();

    Menu::Update();

    FDrive::Update();

    Decoder::Update();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Device::Mode::E Device::State::CurrentMode()
{
    return currentMode;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Device::State::SetMode(Mode::E mode)
{
    if (mode != currentMode)
    {
        currentMode = mode;

        Osci::DeInit();
        Tester::DeInit();
        Multimeter::DeInit();
        Recorder::DeInit();

        if (InModeOsci())
        {
            Osci::Init();
        }
        else if (InModeTester())
        {
            Tester::Init();
        }
        else if (InModeMultimeter())
        {
            Multimeter::Init();
        }
        else if (InModeRecorder())
        {
            Recorder::Init();
        }
        else
        {
            // здесь ничего нету - все варианты проверены
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::State::InModeTester()
{
    return (CurrentMode() == Device::Mode::Tester);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::State::InModeMultimeter()
{
    return (CurrentMode() == Device::Mode::Multimeter);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::State::InModeOsci()
{
    return (CurrentMode() == Device::Mode::Osci);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Device::State::InModeRecorder()
{
    return (CurrentMode() == Device::Mode::Recorder);
}
