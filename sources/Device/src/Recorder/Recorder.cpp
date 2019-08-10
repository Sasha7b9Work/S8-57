#include "defines.h"
#include "Display/Display.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/HAL/HAL.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Settings.h"
#include "Recorder/Recorder_Storage.h"

#include "Hardware/HAL/HAL_PIO.h"
#include "Data/Heap.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Hardware/Memory.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"


using namespace Display::Primitives;
using namespace HAL::ADDRESSES::FPGA;

using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Состояние осциллографа перед переходом в режим регистратора
struct StateOsci
{
    /// Смещение по вертикали для первого и второго каналов
    uint16 storedRShift[2];
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Если true - идёт чтение точек
static bool running = false;
/// true, если регистратор был инициализирован
static bool initialized = false;
/// Сюда сохраним состояние осциллографа в момент перехода в режим регистратора
static StateOsci osci;

/// Сохранить установленные настройки осциллографа
static void StoreOsciSettings();
/// Восстановить ранее сохранённые настройки осциллорафа
static void RestoreOsciSettings();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Init()
{
    StoreOsciSettings();

    FPGA::HAL::LoadRegUPR();
    Osci::Settings::Range::LoadBoth();
    Osci::Settings::Trig::Input::Load();
    Settings::ScaleX::Load();
    Osci::Settings::TShift::Load();
    Osci::Settings::LoadHoldfOff();

    running = false;

    Stop();

    initialized = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::DeInit()
{
    if (!initialized)
    {
        return;
    }

    Stop();

    RestoreOsciSettings();

    initialized = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Update()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::ReadPoint()
{
    if (!IsRunning())
    {
        return;
    }

    if(::HAL::PIO::Read(::HAL::PIO::Port::_G, ::HAL::PIO::Pin::_1))
    {
        BitSet16 dataA(FSMC::ReadFromFPGA(RD::DATA_A), FSMC::ReadFromFPGA(RD::DATA_A + 1)); //-V821
        BitSet16 dataB(FSMC::ReadFromFPGA(RD::DATA_B), FSMC::ReadFromFPGA(RD::DATA_B + 1)); //-V821

        if (Recorder::Storage::CurrentFrame()->FreeMemory() > 4)
        {
            Recorder::Storage::CurrentFrame()->AddPoint(dataA, dataB);
        }
        else
        {
            Stop();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Start()
{
    Region(150, 50).DrawBounded(85, 100, Color::BACK, Color::FILL);
    Text("Подготовка к записи").DrawInCenterRect(85, 100, 150, 50, Color::FILL);
    Painter::EndScene();

    Osci::Settings::RShift::Set(Chan::A, Osci::Settings::RShift::ZERO);
    Osci::Settings::RShift::Set(Chan::B, Osci::Settings::RShift::ZERO);

    Memory::EraseSector(ADDR_SECTOR_RECORDER_1);

    Storage::CreateNewFrame();

    FSMC::WriteToFPGA16(WR::PRED_LO, 0); //-V525
    FSMC::WriteToFPGA16(WR::POST_LO, 0);
    FSMC::WriteToFPGA8(WR::START, 0xff);

    running = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Stop()
{
    running = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void StoreOsciSettings()
{
    osci.storedRShift[Chan::A] = SET_RSHIFT(Chan::A);
    osci.storedRShift[Chan::B] = SET_RSHIFT(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RestoreOsciSettings()
{
    Osci::Settings::RShift::Set(Chan::A, osci.storedRShift[Chan::A]);
    Osci::Settings::RShift::Set(Chan::B, osci.storedRShift[Chan::B]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::OnPressStart()
{
    if (Menu::OpenedItem() != (Control *)PageFunction::PageRecorder::self) //-V1027
    {
        Display::FuncOnWaitStart("Перейдите на страницу РЕГИСТРАТОР", false);

        Timer::PauseOnTime(2000);

        Display::FuncOnWaitStop();

        return;
    }

    return running ? Stop() : Start();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Recorder::IsRunning()
{
    return running;
}
