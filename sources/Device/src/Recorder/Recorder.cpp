#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/Memory.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Storage.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


using namespace HAL::ADDRESSES::FPGA;

using HAL::FSMC;


/// Состояние осциллографа перед переходом в режим регистратора
struct StateOsci
{
    /// Смещение по вертикали для первого и второго каналов
    uint16 storedRShift[2];
};



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



void Recorder::Init()
{
    StoreOsciSettings();

    FPGA::HAL::LoadRegUPR();
    Range::LoadBoth();
    TrigInput::Load();
    RecorderScaleX::Load();
    TShift::Load();
    Osci::LoadHoldfOff();

    running = false;

    Stop();

    initialized = true;
}


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


void Recorder::Update()
{

}


void Recorder::ReadPoint()
{
    if (!IsRunning())
    {
        return;
    }

    if(::HAL::PIO::Read(::HAL::PIO::Port::_G, ::HAL::PIO::Pin::_1))
    {
        if (Recorder::Storage::CurrentFrame()->FreeMemory() > 4)
        {
            BitSet16 dataA(FSMC::ReadFromFPGA(RD::DATA_A), FSMC::ReadFromFPGA(RD::DATA_A + 1));
            BitSet16 dataB(FSMC::ReadFromFPGA(RD::DATA_B), FSMC::ReadFromFPGA(RD::DATA_B + 1));

            Recorder::Storage::CurrentFrame()->AddPoint(dataA, dataB);
        }
        else
        {
            Stop();
        }
    }
}


void Recorder::Start()
{
    Region(150, 50).DrawBounded(85, 100, Color::BACK, Color::FILL);
    Text("Подготовка к записи").DrawInCenterRect(85, 100, 150, 50, Color::FILL);
    Painter::EndScene();

    RShift::Set(Chan::A, RShift::ZERO);
    RShift::Set(Chan::B, RShift::ZERO);

    Memory::EraseSector(ADDR_SECTOR_RECORDER_1);

    Storage::CreateNewFrame();

    FSMC::WriteToFPGA16(WR::PRED_LO, 0); //-V525
    FSMC::WriteToFPGA16(WR::POST_LO, 0);
    FSMC::WriteToFPGA8(WR::START, 0xff);

    running = true;
}


void Recorder::Stop()
{
    running = false;
}


static void StoreOsciSettings()
{
    osci.storedRShift[Chan::A] = SET_RSHIFT(Chan::A);
    osci.storedRShift[Chan::B] = SET_RSHIFT(Chan::B);
}


static void RestoreOsciSettings()
{
    RShift::Set(Chan::A, osci.storedRShift[Chan::A]);
    RShift::Set(Chan::B, osci.storedRShift[Chan::B]);
}


void Recorder::OnPressStart()
{
    if (Menu::OpenedItem() == (Item *)PageRecorder::self)
    {
        if (running)
        {
            Stop();
            Keyboard::Unlock();
        }
        else
        {
            static const Key::E keys[] = { Key::F4, Key::Start, Key::None };
            Start();
            Keyboard::Lock(keys);
        }
    }
    else
    {
        Display::FuncOnWaitStart("Перейдите на страницу РЕГИСТРАТОР", false);

        Timer::PauseOnTime(2000);

        Display::FuncOnWaitStop();
    }
}


bool Recorder::IsRunning()
{
    return running;
}


void RecorderScaleX::Change(int delta)
{
    if (Recorder::IsRunning())
    {
        return;
    }

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&set.rec.scaleX), (uint8)(RecorderScaleX::Size - 1));
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)(&set.rec.scaleX), 0);
    }

    Load();
}


RecorderScaleX &RecorderScaleX::Current()
{
    return set.rec.scaleX;
}


pString RecorderScaleX::ToString() const
{
    static const struct StructScaleX
    {
        const char *name;
        StructScaleX(pString nRU)
        {
            name = nRU;
        };
    }
    scales[RecorderScaleX::Size] =
    {
        StructScaleX("0.1\x10с"),
        StructScaleX("0.2\x10с"),
        StructScaleX("0.5\x10с"),
        StructScaleX("1\x10с"),
        StructScaleX("2\x10с"),
        StructScaleX("5\x10с"),
        StructScaleX("10\x10с")
    };

    return scales[value].name;
}


uint RecorderScaleX::BytesToSec() const
{
    static const struct StructBytes
    {
        uint value;
        StructBytes(uint v) : value(v) {};
    }
    bytes[RecorderScaleX::Size] =
    {
        800,
        400,
        160,
        80,
        40,
        16,
        8
    };

    return bytes[value].value;
}


uint RecorderScaleX::TimeForPointMS() const
{
    static const struct StructTime
    {
        uint value;
        StructTime(uint v) : value(v) {};
    }
    bytes[RecorderScaleX::Size] =
    {
        5,
        10,
        25,
        50,
        100,
        250,
        500
    };

    return bytes[value].value;
}

#ifdef WIN32
#pragma warning(disable:4310)
#endif


void RecorderScaleX::Load()
{
    static const uint8 values[RecorderScaleX::Size] =
    {
        BIN_U8(01010110),  // -V2501  // 100ms  
        BIN_U8(01010111),  // -V2501  // 200ms  
        BIN_U8(01011001),  // -V2501  // 500ms  
        BIN_U8(01011010),  // -V2501  // 1s     
        BIN_U8(01011011),  // -V2501  // 2s     
        BIN_U8(01011101),  // -V2501  // 5s     
        BIN_U8(01011110)   // -V2501  // 10s
    };

    FSMC::WriteToFPGA8(WR::TBASE, values[RecorderScaleX::Current().value]);

    if (Recorder::IsRunning())
    {
        Recorder::Stop();
        Recorder::Start();
    }
}
