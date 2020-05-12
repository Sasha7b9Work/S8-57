#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/ContextRecorder.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/Recorder.h"
#include "Recorder/Sensor.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


// Состояние осциллографа перед переходом в режим регистратора
struct StateOsci
{
    // Смещение по вертикали для первого и второго каналов
    int16 storedRShift[2];
};


struct Mode
{
    enum E
    {
        Listening,      // Режим "прослушивания" входов, при котором на экране мы видим то, что подаётся на входы, без сохранения в память  
        Recording,      // Режим записи, в котором пользователь наблюдает за записываемым сигналом
        Review          // Режим просмотра
    };
};

static Mode::E mode = Mode::Listening;
static bool initialized = false;        // true, если регистратор был инициализирован
static StateOsci osci;                  // Сюда сохраним состояние осциллографа в момент перехода в режим регистратора

// Сохранить установленные настройки осциллографа
static void StoreOsciSettings();

// Восстановить ранее сохранённые настройки осциллорафа
static void RestoreOsciSettings();


void Recorder::Init()
{
    StoreOsciSettings();

    ContextRecorder::LoadRegUPR();
    Range::LoadBoth();
    TrigInput().Load();
    Recorder::ScaleX::Load();
    TShift::Load();
    Osci::LoadHoldfOff();
    StorageRecorder::Init();

    mode = Mode::Listening;

    initialized = true;

    StorageRecorder::CreateNewRecord();

    StorageRecorder::LastRecord()->maxPoints = 320;

    FPGA::GiveStart(0, 0);

    FPGA::ForcedStart();
}


void Recorder::DeInit()
{
    if (initialized)
    {
        Stop();

        RestoreOsciSettings();

        initialized = false;
    }
}


void Recorder::Update()
{
    Sensor::Update();
}


void Recorder::ReadPoint()
{
    if(HAL_PIO::Read(PIN_P2P))
    {
        if (StorageRecorder::LastRecord()->FreeMemory() > 4)
        {
            HAL_BUS::FPGA::SetAddrData(RD::DATA_A, RD::DATA_A + 1);
            BitSet16 dataA(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());

            HAL_BUS::FPGA::SetAddrData(RD::DATA_B, RD::DATA_B + 1);
            BitSet16 dataB(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());

            StorageRecorder::LastRecord()->AddPoints(dataA, dataB);
        }
        else
        {
            Stop();
        }
    }
}


void Recorder::Start()
{
    StorageRecorder::CreateNewRecord();

    mode = Mode::Recording;
}


void Recorder::Stop()
{
    mode = Mode::Review;
}


static void StoreOsciSettings()
{
    osci.storedRShift[ChanA] = S_RSHIFT_A;
    osci.storedRShift[ChanB] = S_RSHIFT_B;
}


static void RestoreOsciSettings()
{
    RShift::Set(ChanA, osci.storedRShift[ChanA]);
    RShift::Set(ChanB, osci.storedRShift[ChanB]);
}


void Recorder::OnPressStart()
{
    if(Menu::OpenedPage() != PageRecorder::self)
    {
        return;
    }

    if(!S_REC_ENABLED_A && !S_REC_ENABLED_B && !S_REC_ENABLED_SENSOR)
    {
        Display::ShowWarning("Нужно выбрать хотя бы один источник записи");
        return;
    }

    if (Menu::OpenedItem() == const_cast<Page *>(PageRecorder::self))
    {
        if (mode == Mode::Recording)
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
        Display::ShowWarning("Перейдите на страницу РЕГИСТРАТОР");
    }
}


bool Recorder::InRecordingMode()
{
    return (mode == Mode::Recording);
}


void Recorder::ScaleX::Change(int delta)
{
    if (!Recorder::InRecordingMode())
    {
        if (delta > 0)
        {
            ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&S_REC_SCALE_X), static_cast<uint8>(Recorder::ScaleX::Count - 1));
        }
        else
        {
            ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&S_REC_SCALE_X), 0);
        }

        Load();
    }
}


pString Recorder::ScaleX::ToString()
{
    static const struct StructScaleX
    {
        const char *name;
        StructScaleX(pString nRU)
        {
            name = nRU;
        };
    }
    scales[Count] =
    {
        StructScaleX("0.1\x10с"),
        StructScaleX("0.2\x10с"),
        StructScaleX("0.5\x10с"),
        StructScaleX("1\x10с"),
        StructScaleX("2\x10с"),
        StructScaleX("5\x10с"),
        StructScaleX("10\x10с")
    };

    return scales[S_REC_SCALE_X].name;
}


int Recorder::ScaleX::BytesToSec()
{
    static const struct StructBytes
    {
        int value;
        StructBytes(int v) : value(v) {};
    }
    bytes[Count] =
    {
        800,
        400,
        160,
        80,
        40,
        16,
        8
    };

    return bytes[S_REC_SCALE_X].value;
}


int Recorder::ScaleX::TimeForPointMS()
{
    static const struct StructTime
    {
        int value;
        StructTime(int v) : value(v) {};
    }
    bytes[Count] =
    {
        5,
        10,
        25,
        50,
        100,
        250,
        500
    };

    return bytes[S_REC_SCALE_X].value;
}

#ifdef WIN32
#pragma warning(disable:4310)
#endif


void Recorder::ScaleX::Load()
{
    static const uint8 values[Count] =
    {
        BIN_U8(01010110),  // -V2501  // 100ms  
        BIN_U8(01010111),  // -V2501  // 200ms  
        BIN_U8(01011001),  // -V2501  // 500ms  
        BIN_U8(01011010),  // -V2501  // 1s     
        BIN_U8(01011011),  // -V2501  // 2s     
        BIN_U8(01011101),  // -V2501  // 5s     
        BIN_U8(01011110)   // -V2501  // 10s
    };

    HAL_BUS::FPGA::Write8(WR::TBASE, values[S_REC_SCALE_X]);

    if (Recorder::InRecordingMode())
    {
        Recorder::Stop();
        Recorder::Start();
    }
}
