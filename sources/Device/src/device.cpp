#include "defines.h"
#include "device.h"
#include "common/Decoder_d.h"
#include "Display/Console.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Battery.h"
#include "Hardware/Beeper.h"
#include "Hardware/PowerBreaker.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/BufferButtons.h"
#include "Menu/MenuItems.h"
#include "Recorder/Recorder.h"
#include "Recorder/Sensor.h"
#include "SCPI/SCPI.h"
#include "Settings/SettingsNRST.h"
#include <cstdlib>


// Настроить устройство в соответствии с установленным режимом
static void SetCurrentMode();
// Установить режим работы mode, если открыта страница page или в числе предков открытой страницы есть page
static bool SetCurrentMode(const Page *page, Device::Mode::E mode);

static Device::Mode::E currentMode = Device::Mode::Osci;



void Device::Init()
{
    HAL::Init();

    Console::Init();

    Timer::Init();

    PAUSE_ON_MS(500);

    Beeper::Init();

    Battery::Init();

    HAL_RTC::Init();

    VCP::Init();

    Tester::Init();

    FPGA::Init();

    ROM::Init();

    Settings::Load();

    setNRST.Init();

    Display::Init();

    FreqMeter::Init();

    Osci::Init();

    Menu::Init();

    Multimeter::Init();

    FDrive::Init();

    //Sensor::Init();

    SetCurrentMode();

    HAL_TIM5::Init();
}


void SetCurrentMode()
{
    if (!SetCurrentMode(PageMultimeter::self, Device::Mode::Multimeter))
    {
        if (!SetCurrentMode(PageTester::self, Device::Mode::Tester))
        {
            if (!SetCurrentMode(PageRecorder::self, Device::Mode::Recorder))
            {
                Device::SetMode(Device::Mode::Osci);
            }
        }
    }
}


bool SetCurrentMode(const Page *page, Device::Mode::E mode)
{
    Item *opened = Menu::OpenedItem();

    if (opened &&
        (opened == const_cast<Page *>(page) || opened->ExistKeeper(page)))
    {
        Device::SetMode(mode);
        return true;
    }

    return false;
}


void Device::Update()
{
    Timer::StartMultiMeasurement();

    Osci::Update();

    Display::Update();

    Tester::Update();

    Recorder::Update();

    FDrive::Update();

    //Sensor::Update();

    while (HAL_BUS::Panel::Receive())
    {
    };

    SCPI::Update();

    DDecoder::Update();

    Menu::Update();

    PowerBreaker::Update();
}


Device::Mode::E Device::CurrentMode()
{
    return currentMode;
}


void Device::SetMode(Mode::E mode)
{
    if (mode != currentMode)
    {
        currentMode = mode;

        Osci::DeInit();
        Tester::Disable();
        Multimeter::DeInit();
        Recorder::DeInit();

        switch (CurrentMode())
        {
        case Mode::Osci:
            Keyboard::Unlock();
            Osci::Init();
            break;

        case Mode::Tester:
        {
            static const Key::E keys[] =
            {
                Key::RangeMoreA, Key::RangeLessA, Key::RShiftMoreA, Key::RShiftLessA,
                Key::RangeMoreB, Key::RangeLessB, Key::RShiftMoreB, Key::RShiftLessB,
                Key::F1, Key::F2, Key::F3, Key::F4, Key::F5,
                Key::Left, Key::Right, Key::Up, Key::Down,
                Key::Start, Key::Enter, Key::Memory, Key::None
            };

            Keyboard::Lock(keys);
            Tester::Init();
            Tester::Enable();
            break;
        }

        case Mode::Multimeter:
        {
            static const Key::E keys[] =
            {
                Key::F1, Key::F2, Key::F3, Key::F4, Key::F5,
                Key::Left, Key::Right, Key::Up, Key::Down,
                Key::Enter, Key::Memory, Key::None
            };

            Keyboard::Lock(keys);
            Multimeter::Init();
            break;
        }

        case Mode::Recorder:
            Keyboard::Unlock();
            Recorder::Init();
            break;

        case Mode::Count:
            break;
        }
    }
}


bool Device::InModeTester()
{
    return (CurrentMode() == Device::Mode::Tester);
}


bool Device::InModeMultimeter()
{
    return (CurrentMode() == Device::Mode::Multimeter);
}


bool Device::InModeOsci()
{
    return (CurrentMode() == Device::Mode::Osci);
}


bool Device::InModeRecorder()
{
    return (CurrentMode() == Device::Mode::Recorder);
}


void Device::Reset()
{
    HAL_NVIC::SystemReset();
}
